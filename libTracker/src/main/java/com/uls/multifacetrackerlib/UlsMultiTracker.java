package com.uls.multifacetrackerlib;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.Rect;
import android.graphics.RectF;

import com.uls.multifacetrackerlib.bean.FaceInfo;
import com.uls.multifacetrackerlib.utils.DeviceUtil;
import com.uls.multifacetrackerlib.utils.L;

import java.util.Arrays;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLContext;

/**
 * 多人脸跟踪类，native接口对应tvstrackerlib.cpp
 */
public class UlsMultiTracker {
    static private final String TAG = "UlsMultiTracker";
    private Context context;

    public enum UlsTrackerInterfaceType { OPENGL_TEXTURE, BYTEARRAY };

    // Modified.
    public enum ImageDataType {
        NV21(0),  BGR(1);// ARGB in Java is RGBA in C++

        private final int miValue;

        ImageDataType(int value) {
            miValue = value;
        }

        public int getValue() {
            return miValue;
        }
    };

    private EGL10 mEGL = null;
    private int mSrcTextureName = -1;

    private UlsTrackerInterfaceType mAPI;

    private int mTrackerCount;
    private boolean[] mAlive;

    private boolean mInitialised = false;

    private boolean mPredictPupils = true, mPoseEnabled = true;
    private boolean mHighPrecision = false, mSticky = true;
    private UlsTrackerMode mTrackMode = UlsTrackerMode.TRACK_COMBINED;

    // Modified.
    private final boolean DO_POSE_STABLIZATION = true;

    public void setTrackerConfidenceThreshold(float first, float others) {
        naSetThreshold(first, others);
    }

    private UlsMultiTracker(final Context context, int trackerCount, int minSize, UlsTrackerInterfaceType
            apiType) {

        mAPI = apiType;
        if (mAPI == UlsTrackerInterfaceType.BYTEARRAY) mInitialised = true;
        this.context = context;

        L.i(TAG, "External folder: " + context.getExternalFilesDir(null));
//        context.getExternalFilesDir("/").mkdirs();

        mTrackerCount = Math.max(trackerCount, 1);
        if (mTrackerCount != trackerCount) {
            L.i(TAG, "Max number of trackers is " + mTrackerCount);
        }

        mShapePointCount = new int[mTrackerCount];
        mAlive = new boolean[mTrackerCount];
        Arrays.fill(mShapePointCount, 0);
        Arrays.fill(mAlive, false);

        boolean ok = naMultiInitialiseFromAssets(context, mTrackerCount, mAPI == UlsTrackerInterfaceType
                        .OPENGL_TEXTURE, context.getAssets(), minSize,
                context.getCacheDir().getAbsolutePath());
        if (!ok) {
            throw new RuntimeException("Can't initialise trackers");
        }
        createVariables(mTrackerCount);

        // Modified.
//        UlsPoseStablization.initialise(mTrackerCount);
    }

    private UlsMultiTracker(final Context context, final String path, int trackerCount, UlsTrackerInterfaceType apiType) {

        mAPI = apiType;
        if (mAPI == UlsTrackerInterfaceType.BYTEARRAY) mInitialised = true;
        this.context = context;
        mTrackerCount = Math.max(trackerCount, 2);
        if (mTrackerCount != trackerCount) {
            L.i(TAG, "Max number of trackers is " + mTrackerCount);
        }

        mShapePointCount = new int[mTrackerCount];
        mAlive = new boolean[mTrackerCount];
        Arrays.fill(mShapePointCount, 0);
        Arrays.fill(mAlive, false);

        boolean ok = naMultiInitialiseFromPath(context, mTrackerCount,
                mAPI == UlsTrackerInterfaceType.OPENGL_TEXTURE, path);
        if (!ok) {
            throw new RuntimeException("Can't initialise trackers from path " + path);
        }

        createVariables(mTrackerCount);

        // Modified.
//        UlsPoseStablization.initialise(mTrackerCount);
    }

    public UlsMultiTracker(final Context context, int trackerCount, int minSize) {
        this(context, trackerCount, minSize, UlsTrackerInterfaceType.BYTEARRAY);
    }
    public UlsMultiTracker(final Context context, final String path, int trackerCount) {
        this(context, path, trackerCount, UlsTrackerInterfaceType.BYTEARRAY);
    }



    private void createVariables(int count) {
        mShape = new float[count][];
        mShape3D = new float[count][];
        mConfidence = new float[count][];
        mPose = new float[count][];
        mEulerAngles = new float[count][];
        mPupils = new float[count][];
        mGaze = new float[count][];
        mPoseQuality = new float[count];

        for (int i = 0; i < count; i++) {
            mShape[i] = new float[2 * mShapePointCount[i]];
            mShape3D[i] = new float[3 * mShapePointCount[i]];
            mConfidence[i] = new float[mShapePointCount[i]];
            mPose[i] = new float[6];
            mPose[i][0] = -1000;
            mEulerAngles[i] = new float[3];
            //mPoseQuality = 0;
            mPupils[i] = new float[4];
            mPupils[i][0] = -1000;
            mGaze[i] = new float[6];
            mGaze[i][0] = -1000;
        }
    }

    /**
     * Initialise the trackers with a previously created OpenGL texture
     *
     * <p> The current EGLContext will be used to initialise some OpenGL objects inside the tracker.
     * This EGLContext must be the current context when the {@link #update update} and
     * {@link #addFaces(RectF[], int[]) resetWithFaceRectangles} methods are called.
     *
     * @param srcOpenGLTexture A valid OpenGL texture name in the current EGLContext
     * @param inputImageWidth Width of the input texture
     * @param inputImageHeight Height of the input texture
     * @return true if the initialisation succeeded.
     */
    public boolean initialise(int srcOpenGLTexture, int inputImageWidth, int inputImageHeight) {
        if (mAPI != UlsTrackerInterfaceType.OPENGL_TEXTURE) {
            throw new RuntimeException("Wrong initialisation function for the chosen API.");
        }
        mEGL = (EGL10) EGLContext.getEGL();
        EGLContext context = mEGL.eglGetCurrentContext();
        if (context == EGL10.EGL_NO_CONTEXT || srcOpenGLTexture < 0) {
            throw new RuntimeException("No current EGL context, or texture < 0");
        }
        mSrcTextureName = srcOpenGLTexture;
        mInitialised = naMultiSetupOpenGL(mSrcTextureName, inputImageWidth, inputImageHeight);

        return mInitialised;
    }

    /**
     * Initialise the tracker when selected input is byte array NV21/ARGB/BGR
     * @return true on success
     */
    public boolean initialise() {

        if (mAPI != UlsTrackerInterfaceType.BYTEARRAY) {
            throw new RuntimeException("Wrong initialisation function for the chosen API.");
        }
        mInitialised = naMultiSetupByteArray();

        return mInitialised;
    }

    /**
     * Releases the native objects
     */
    public void dispose() {
        naMultiDispose();
    }

    public void finalize() throws Throwable {
        super.finalize();

        if (nativeTrackerPtr != 0) {
            L.i(TAG, "You can also release the native object by calling dispose().");
            naMultiDispose();
        }

        // Modified.
//        UlsPoseStablization.release();
    }

    /**
     * Activates the tracker with a key
     * @param key A string containing a valid key
     * @return true if the key is valid, false otherwise.
     */
    public int activate(String key, String keySecret) {
        mInitialised = false;
        return naMultiActivate(key, keySecret,DeviceUtil.getMacAddress(context));
    }

    /**
     * Signal the tracker that the Activity is being paused, and the EGLContext is expected
     * to become invalid.
     *
     * <p>A call to {@link #initialise(int, int, int)} must be
     * made before using the tracker again</p>
     */
    public void onPause() {
        if (mAPI == UlsTrackerInterfaceType.OPENGL_TEXTURE) {
            mInitialised = false;
            naMultiEGLContextInvalidated();
        }
        Arrays.fill(mAlive, false);
    }


    public int getSrcTextureName() {
        return mSrcTextureName;
    }

    public int getNumberOfPoints(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        return mShapePointCount[index];
    }

    public float[] getShape(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return null;
        if (mAlive[index]) {
            // Attempt to fix lower lip feature points when yaw exceed a specific threshold.
            if (mConfidence[index] != null && mEulerAngles[index] != null) {
                float yaw = mEulerAngles[index][1];
                float conf = mConfidence[index][57];
                if (yaw > 0.25 /*&& conf < 0.65*/) {
                    // yaw weight    : 0 - 0.35 /// 0.25 - 0.50+
                    double factor_yaw = Math.pow(4 * ( 0.50 - Math.min(Math.abs(yaw), 0.50)) - 1, 2) * 0.25;
                    // quality weight: 0 - 0.15 /// 0.00 - 0.65
                    double factor_qual = Math.pow(100 / 65 * (conf - 0.65), 2) * 0.10;
                    float factor = 1 - (float)factor_yaw - (float)factor_qual;

                    // 55...59 align 49...53
                    for (int i=55; i<60; i++) {
                        mShape[index][i*2+0] = mShape[index][(108-i)*2+0] * (1-factor)
                                + mShape[index][i*2+0] * factor;
                        mShape[index][i*2+1] = mShape[index][(108-i)*2+1] * (1-factor)
                                + mShape[index][i*2+1] * factor;
                    }

                    // 63...65 align 60...62 heavily
                    for (int i=63; i<66; i++) {
                        mShape[index][i*2+0] = mShape[index][(125-i)*2+0] * (1-factor)
                                + mShape[index][i*2+0] * factor;
                        mShape[index][i*2+1] = mShape[index][(125-i)*2+1] * (1-factor)
                                + mShape[index][i*2+1] * factor;
                    }
                }
                return mShape[index];
            } else {
                return mShape[index];
            }
        }
        else return null;
    }

    public float[] getShape3D(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return null;
        if (mAlive[index]) return mShape3D[index];
        else return null;
    }
    public float[] getConfidence(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return null;
        if (mAlive[index]) return mConfidence[index];
        else return null;
    }
    /**
     * Rotation angles
     * @return A float[3] array with {pitch, yaw, roll} angles, or null if not available
     */
    public float[] getRotationAngles(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return null;
        if (mAlive[index] && mPoseEnabled)
            return Arrays.copyOf(mEulerAngles[index], mEulerAngles[index].length);
        return null;
    }

    /**
     * X- and Y- translation in the image
     * @return a float[2] vector, or null if not available
     */
    public float[] getTranslationInImage(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return null;
        if (mAlive[index] && mPoseEnabled) return Arrays.copyOfRange(mPose[index], 3, 5);
        return null;
    }

    // Modified.
//    public float[] getTranslation(int index, float focalLength, float imageCentreX, float imageCentreY)
//    {
//        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
//        if (!mInitialised)
//            return null;
//        if (!mAlive[index])
//            return null;
//
//        MatOfPoint3f pts3d = new MatOfPoint3f();
//        pts3d.alloc(mShapePointCount[index]);
//        MatOfPoint2f pts = new MatOfPoint2f();
//        pts.alloc(mShapePointCount[index]);
//
//        for (int i = 0; i < mShapePointCount[index]; i++) {
//            pts3d.put(i, 0, mShape3D[index][3*i] * 3, mShape3D[index][3*i+1] * 3, mShape3D[index][3*i+2] * 3);
//            pts.put(i, 0, mShape[index][2*i], mShape[index][2*i+1]);
//        }
//
//        Mat c = Mat.eye(3, 3, CvType.CV_32F);
//        c.put(0, 0, focalLength);
//        c.put(0, 1, 0);
//        c.put(0, 2, imageCentreX);
//        c.put(1, 0, 0);
//        c.put(1, 1, focalLength);
//        c.put(1, 2, imageCentreY);
//        c.put(2, 0, 0);
//        c.put(2, 1, 0);
//        c.put(2, 2, 1);
//
//        Mat r = new Mat(),
//                t = new Mat();
//        Calib3d.solvePnP(pts3d, pts, c, new MatOfDouble(), r, t, false, Calib3d.CV_EPNP);
//
//        float[] centerXYZ = new float[3];
//        centerXYZ[0] = (float) t.get(0, 0)[0];
//        centerXYZ[1] = (float) t.get(1, 0)[0];
//        centerXYZ[2] = (float) t.get(2, 0)[0];
//        return centerXYZ;
//    }

    /**
     * Weak-perspective camera model's scale parameter
     * @return scale in image, or -1.0f if the pose is not available.
     */
    public float getScaleInImage(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return 0.0f;
        if (mAlive[index] && mPoseEnabled) return mPose[index][5];
        return 0.0f;
    }

    public float getPoseQuality(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return 0.0f;
        if (mAlive[index] && mPoseEnabled) return mPoseQuality[index];
        return 0.0f;
    }
    public float[] getPupils(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return null;
        if (mAlive[index] && mPredictPupils) {
            if (mPupils[index][0] <= -100) return null;
            return mPupils[index];
        }
        return null;
    }

    /**
     * Gaze values
     * @return A float[6] array (x,y,z)(x,y,z), or null.
     */
    public float[] getGaze(int index) {
        if (index >= mTrackerCount) throw new RuntimeException("Invalid tracker index");
        if (!mInitialised)
            return null;
        if (mAlive[index] && mPredictPupils) {
            if (mGaze[index][0] <= -100) return null;
            return mGaze[index];
        }
        return null;
    }

    public void setTrackMode(UlsTrackerMode mode) {
        mTrackMode = mode;
        switch (mode) {
            case TRACK_COMBINED:
                mPredictPupils = true;
                mPoseEnabled = true;
                break;
            case TRACK_FACE_AND_PUPILS:
                mPredictPupils = true;
                mPoseEnabled = false;
                break;
            case TRACK_FACE:
                mPoseEnabled = mPredictPupils = false;
                break;
            case TRACK_FACE_AND_POSE:
                mPredictPupils = false;
                mPoseEnabled = true;
                break;
        }
    }
    public UlsTrackerMode getTrackMode() { return mTrackMode; }
    public void setHighPrecision(boolean highPrecision) {
        mHighPrecision = highPrecision;
    }
    public boolean getHighPrecision() { return mHighPrecision; }
    public void setSticky(boolean sticky) {
        mSticky = sticky;
    }
    public boolean getSticky() { return mSticky; }
//endregion


    /**
     * Resets the tracker(s), invalidating the current status
     * @param index Index of the tracker to reset. Use -1 to reset all trackers
     */
    public void resetTracker(int index) {
        if (index < 0) {
            for (int i = 0; i < index; i++) {
                if (mAlive[i]) {
                    if (naMultiResetTracker(i)) L.d(TAG, "Tracker "+i+" reset");
                    else L.w(TAG, "Failed to reset tracker " + i);
                    mAlive[i] = false;
                }
            }
        } else if (index < mTrackerCount) {
            naMultiResetTracker(index);
            mAlive[index] = false;
        } else {
            L.e(TAG, "Error, bad tracker index " + index + " (max: "+ mTrackerCount + ")");
        }
    }

    /**
     * Adds a number of detected faces that may be used to reset the trackers. Resetting only takes
     * pace if the trackers are not tracking, call {@link #resetTracker(int)} to reset them
     *
     * @param rectangles An array of RectF with the locations of the faces
     * @param rotations An array of rotations, of the same length as the rectangles
     * @return False if there's an error
     */
    public boolean addFaces(RectF[] rectangles, int[] rotations) {
        if (!mInitialised) {
            return false;
        }
        if (rectangles.length == 0) return false;
        if (rectangles.length != rotations.length)
            throw new RuntimeException("Face rectangle and rotation arrays should be " +
                "of the same length!");
        //put everything in an array of rectangles with the rotation as an extra parameter
        int[] rect = new int[5 * rectangles.length];
        for (int i = 0; i < rectangles.length; i++) {
            rect[i * 5] = (int)rectangles[i].left;
            rect[i * 5 + 1] = (int)rectangles[i].top;
            rect[i * 5 + 2] = (int)rectangles[i].width();
            rect[i * 5 + 3] = (int)rectangles[i].height();
            rect[i * 5 + 4] = rotations[i];
        }
        naMultiAddFaces(rect);
        return true;
    }

    /**
     * Add faces to the tracker by searching the image using OpenCV's face detector
     *
     * @param nv21 Image data in nv21 format
     * @param cameraWidth Width of the image
     * @param cameraHeight Height of the image
     * @param rotation Rotation of the camera. Either 0, 90, 180 or 270.
     * @return True if at least a face is found, false otherwise
     */
    public boolean findFacesAndAdd(byte[] nv21, int cameraWidth, int cameraHeight, int rotation, int imageDataType) {

        if (!mInitialised)
            return false;
        if (mAPI != UlsTrackerInterfaceType.BYTEARRAY) {
            throw new RuntimeException("Wrong findFacesAndAdd function, OpenGL interface was " +
                    "selected in constructor");
        }

        return naMultiFindFacesAndAddByte(nv21, cameraWidth, cameraHeight, rotation, imageDataType);
    }

    /**
     * Update the shape location with the NV21 byte array data
     * @param nv21 Image data in nv21 format
     * @param width Width of the image
     * @param height Height of the image
     * @return number of trackers currently in use
     */
    public int update(byte[] nv21, int width, int height, int imageDataType) {
        if (!mInitialised)
            return 0;
        if (mAPI != UlsTrackerInterfaceType.BYTEARRAY) {
            throw new RuntimeException("Wrong update function, OpenGL interface was selected in " +
                    "constructor");
        }
        int alive = naMultiUpdateShapesByte(nv21, width, height, mPredictPupils, mHighPrecision,
                mSticky, imageDataType);
        if (alive < 0) L.e(TAG, "Error in update() shapes");

        // Modified. Do pose stablization.
//        if (DO_POSE_STABLIZATION) {
//            if (alive > 0) {
//                for (int i = 0; i < mTrackerCount; i++) {
//                    if (mAlive[i]) {
//                        // Prepare an RGB image Mat.
//                        Mat matRGBImg = new Mat((int) (height * 1.5), width, CvType.CV_8UC1);
//                        matRGBImg.put(0, 0, nv21);
//                        Imgproc.cvtColor(matRGBImg, matRGBImg, Imgproc.COLOR_YUV2BGR_NV21);
//                        float[] stablePitchRollYawScaleXYZ = calculateStablePose(alive, i, matRGBImg, width, height);
//                        matRGBImg.release();
//
//                        // Write stable pose data.
//                        if (stablePitchRollYawScaleXYZ != null) {
//							mEulerAngles[i][0] = stablePitchRollYawScaleXYZ[2];
//							mEulerAngles[i][1] = stablePitchRollYawScaleXYZ[0];
//							mEulerAngles[i][2] = stablePitchRollYawScaleXYZ[1];
//							mPose[i][5] = stablePitchRollYawScaleXYZ[3];
//						}
//                    }
//                }
//            }
//        }

        return alive;
    }

    public int trackerBitmap(Bitmap bitmap) {
        return naMultiTrackerPic(bitmap, mPredictPupils, mHighPrecision, mSticky);
    }

    public boolean checkBlur(Bitmap bitmap,float firstparam,float secondparam, Rect rect) {
        return naCheckBlur(bitmap, firstparam, secondparam,rect.left, rect.top, rect.right, rect.bottom);
    }

    /**
     *
     * @param bitmap
     * @return FaceInfo[] 五点信息顺序: x0,y0,x1,y1...
     */
    public FaceInfo[] processImageByDetector(Bitmap bitmap) {
        return nativeProcessDetectorImage(bitmap);
    }

    public Bitmap crop(Bitmap srcBmp, Rect rect) {
        Bitmap cropBitmap = Bitmap.createBitmap(rect.width(), rect.height(), Bitmap.Config.ARGB_8888);
        naCrop(srcBmp, cropBitmap, rect.left, rect.top, rect.right, rect.bottom);
        return cropBitmap;
    }

    public void setScaleDownFactor(int scaleDownFactor) {
        naSetScaleDownFactor(scaleDownFactor);
    }

    public void enableAutoScaleDown() {
        naEnableAutoScaleDown();
    }

    // Modified.
//    public float[] calculateStablePose(int aliveFaceNum, int index, Mat matRGBImage, int cameraWidth, int cameraHeight) {
//        float[] headCenter3D = getTranslation(index, 1000, cameraWidth/2, cameraHeight/2);
//
//        return UlsPoseStablization.calculateStablePose(index, aliveFaceNum, matRGBImage.getNativeObjAddr(),
//                getNumberOfPoints(index), mShape[index], mConfidence[index], mEulerAngles[index][1], mEulerAngles[index][2], mEulerAngles[index][0], mPose[index][5],
//                headCenter3D[0], headCenter3D[1], headCenter3D[2]);
//    }

    private float[][] mShape, mShape3D, mConfidence;
    private float[][] mPose, mEulerAngles; //mPose is Rodrigues + translation
    private float[][] mPupils, mGaze;
    private float[] mPoseQuality;
    private int[] mShapePointCount;
    private long nativeTrackerPtr = 0;

    public int getTrackerCount() {
        return mTrackerCount;
    }

    public int getTrackerState(int index) {
        return naGetTrackerState(index);
    }

    private native boolean naMultiInitialiseFromAssets(Context context, int count, boolean useOGL,
                                                       AssetManager manager, int minSize,
                                                       String cacheDir);
    private native boolean naMultiInitialiseFromPath(Context context, int count, boolean useOGL, String path);

    private native void naMultiDispose();

    private native int naMultiActivate(String key, String keySecret, String macString);

    private native boolean naMultiSetupByteArray();
    private native boolean naMultiSetupOpenGL(int srcTextureName, int width, int height);
    private native void naMultiEGLContextInvalidated();

    public native boolean naMultiFindFacesAndAdd(int rotation);
    public native boolean naMultiFindFacesAndAddByte(byte[] nv21, int width, int height,
                                                      int rotation, int imageDataType);
    private native boolean naMultiAddFaces(int[] faces);

    public native int naMultiUpdateShapes(boolean predictPupils, boolean highPrecision,
                                           boolean smooth);
    private native int naMultiUpdateShapesByte(byte[] nv21, int width, int height,
                                               boolean predictPupils,
                                               boolean highPrecision,
                                               boolean smooth, int imageDataType);

    private native int naMultiTrackerPic(Bitmap bitmap,
                                         boolean predictPupils,
                                         boolean highPrecision,
                                         boolean smooth);

    private native boolean naMultiResetTracker(int index);

    private native void naSetThreshold(float first, float others);
    public native int[] naMultiGetFaceRect();
    private native void naSetScaleDownFactor(int scaleDownFactor);
    private native void naEnableAutoScaleDown();
    private native int naGetTrackerState(int index);

    // ULSee Dense-106-Points APIs.
    public native void naUlsDensePtsInitialize();
    public native float[] naUlsDensePtsDenseShape(float[] pts2d, int pointNum);
    public native float[] naUlsDensePtsJawline(float[] pts2d, int pointNum);
    public native float[] naUlsDensePtsDenseEyebrow(float[] pts2d, int pointNum);
    public native float[] naUlsDensePtsDenseNose(float[] pts2d, int pointNum);
    public native float[] naUlsDensePtsDenseEye(float[] pts2d, int pointNum);
    public native float[] naUlsDensePtsDenseMouthShape1(float[] pts2d, int pointNum);
    public native float[] naUlsDensePtsDenseMouthShape2(float[] pts2d, int pointNum);
    public native void naUlsDensePtsRelease();

    public native boolean naCheckBlur(Bitmap bitmap,float firstparam,float secondparam,int left, int top, int right, int bottom);

    private native FaceInfo[] nativeProcessDetectorImage(Bitmap image);

    private static native void naCrop(Bitmap srcBitmap, Bitmap outBitmap, int left, int top, int right, int bottom);

    static {
        System.loadLibrary("ulsTracker_native");
    }
}
