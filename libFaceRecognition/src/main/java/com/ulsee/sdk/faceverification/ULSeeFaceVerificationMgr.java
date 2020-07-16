package com.ulsee.sdk.faceverification;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.os.Handler;
import android.os.HandlerThread;
import android.util.Log;

import com.uls.multifacetrackerlib.ULSeeTrackerManager;
import com.uls.multifacetrackerlib.bean.FaceInfo;
import com.uls.multifacetrackerlib.utils.DeviceUtil;
import com.uls.multifacetrackerlib.utils.L;
import com.uls.multifacetrackerlib.utils.PointUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * 人脸识别管理器
 */
public class ULSeeFaceVerificationMgr {
    private static final String TAG = "ULSeeFaceVerification";

    static {
        System.loadLibrary("faceverification");
    }

    private boolean detectionRunning;
    private HandlerThread faceRecognitionThread;
    private Handler faceRecognitionHandler;

    private ULSeeTrackerManager ulSeeTrackerManager;
    private long extractorHandle;
    private long detectorHandle;
    private Object cacheLock = new Object();
    private List<FaceInfo> cacheFaceInfos;
    private boolean initTrackerKeySucceed;
    private boolean initFaceVerKeySucceed;

    private int mMaxTrackers = 5;
    private float thresholdFirst = 0.35f;
    private float thresholdLast = 0.2f;
    private int minFaceSize = 120;
    private int imageDataType = ImageDataType.NV21.getValue();
    private Context context;

    private ExtractCallback extractCallback;
    private NativeDataCallBack nativeDataCallBack;
    public static boolean debugMode;

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

    public ULSeeFaceVerificationMgr(Context context) {
        this.context = context;
    }

    /**
     * 最小人脸检测大小设置，默认120
     * 静态图片推荐60左右,动态视频流推荐100左右
     * 请在init之前调用
     * @param minFaceSize 最小人脸检测大小
     * @return this
     */
    public ULSeeFaceVerificationMgr setMinFaceSize(int minFaceSize) {
        this.minFaceSize = minFaceSize;
        return this;
    }

    /**
     * 设置视频流的数据格式,默认是ImageDataType.NV21
     * @param imageDataType 视频流的数据格式
     * @return this
     */
    public ULSeeFaceVerificationMgr setImageDataType(int imageDataType) {
        this.imageDataType = imageDataType;
        return this;
    }

    /**
     * 用于JNI向Java层传递float数据
     * 数组第一个元素是人脸特征提取算法的耗时 单位是毫秒;
     * 如果某个元素的取值是-1 表示没有值;
     * @param data
     */
    private void onNativeFloatDataAvailable(float[] data) {
      if (data != null && data.length > 0){
          if (nativeDataCallBack != null) {
              float extractTime = data[0];
              nativeDataCallBack.onNativeFloatDataAvailable(new float[]{extractTime});
          }

      }
    }

    /**
     * 最多跟踪多少个人脸设置，默认5
     * 请在init之前调用
     * @param maxTrackers 最多跟踪人脸数
     * @return this
     */
    public ULSeeFaceVerificationMgr setMaxTrackers(int maxTrackers) {
        this.mMaxTrackers = maxTrackers;
        return this;
    }

    /**
     * 初始化
     * @param keyString
     * @return ULSeeError.SUCCESSED:成功 其它:失败
     */
    public int init(String keyString){
        String key = "";
        String secret = "";
        if (keyString != null){
            String[] keyArray = keyString.split("\\$\\$\\$");
            if (keyArray != null && keyArray.length >= 2){
                key = keyArray[0];
                secret = keyArray[1];
            }
        }
        return init(key, secret);
    }

    /**
     * 初始化
     * @param key
     * @param keySecret
     * @return ULSeeError.SUCCESSED:成功 其它:失败
     */
    public int init(String key, String keySecret) {
        AssetManager assetManager = context.getAssets();
        detectorHandle = nativeInitDetector(assetManager, minFaceSize);
        extractorHandle = nativeInitExtractor(assetManager, key, keySecret,DeviceUtil.getMacAddress(context));
        if (extractorHandle > 0 && extractorHandle < 100) { //错误码的范围 (0,100), 如果是内存地址， 不会在这个范围内
            return (int) extractorHandle;
        } else {
            initFaceVerKeySucceed = true;
        }
        openFaceRecognitionThread();
        ulSeeTrackerManager = new ULSeeTrackerManager(context)
                .setMaxTrackers(mMaxTrackers)
                .setThresholdFirst(thresholdFirst)
                .setThresholdLast(thresholdLast)
                .setMinFaceSize(minFaceSize)
                .setImageDataType(imageDataType)
                .checkPerfect(true);
        int initTrackerStatus = ulSeeTrackerManager.init(key + "$$$" + keySecret);
        initTrackerKeySucceed = initTrackerStatus == ULSeeError.SUCCESSED;
        if(!initTrackerKeySucceed) {
            return initTrackerStatus;
        }

        return ULSeeError.SUCCESSED;
    }

    private void openFaceRecognitionThread() {
        if (faceRecognitionThread != null) {
            faceRecognitionThread.quit();
        }
        faceRecognitionThread = new HandlerThread("FaceRecognition Thread");
        faceRecognitionThread.start();
        faceRecognitionHandler = new Handler(faceRecognitionThread.getLooper());
    }

    /**
     * 人脸识别-图片版本
     * @param bitmap 输入图片，格式Bitmap.Config.ARGB_8888或者Bitmap.Config.ARGB_565
     * @return 人脸信息
     */
    public List<FaceInfo> extractFeature(Bitmap bitmap) {
        List<FaceInfo> faceInfos = new ArrayList<>();
        if (!initFaceVerKeySucceed) {
            L.e(TAG, "请初始化key");
            return faceInfos;
        }
        faceInfos = ulSeeTrackerManager.trackerBitmap(bitmap);
        float[][] multiFacePoint = getPointsFromFaceInfos(faceInfos);
        if(multiFacePoint == null || multiFacePoint.length == 0 || multiFacePoint[0] == null || multiFacePoint[0].length == 0){
            return faceInfos;
        }
        float[][] features = nativeExtactFeatureBitmap(extractorHandle, bitmap,multiFacePoint);
        if (faceInfos != null && features != null && faceInfos.size() == features.length) {
            FaceInfo faceInfo;
            for (int i = 0; i < faceInfos.size(); i++) {
                faceInfo = faceInfos.get(i);
                faceInfo.setFeatures(features[i]);
                faceInfo.id = i;
            }
        }
        return faceInfos;
    }

    /**
     * 人脸识别-视频流版本 算法跑在异步线程提取特征值完毕后ExtractCallback回调
     * @param data 图片数据，格式nv21
     * @param width 图片的宽
     * @param height 图片的高
     * @param imageRoate 逆时针旋转图片至人脸朝上，所需旋转的角度（0/90/180/270）
     * @return 人脸信息 由于内部是异步跑特征值提取，
     * 前几帧返回的FaceInfo features为null，
     * 建议在ExtractCallback回调时处理特征值比对逻辑
     */
    public List<FaceInfo> updateFrame(final byte[] data, final int width, final int height, int imageRoate) {
        return updateFrame(data, width, height, imageRoate, getFaceInfo(data, width, height,imageRoate));
    }

    /**
     *
     * @param data 图片数据，格式nv21
     * @param width 图片的宽
     * @param height 图片的高
     * @param imageRoate 逆时针旋转图片至人脸朝上，所需旋转的角度（0/90/180/270）
     * @return 基本的人脸信息, 不做识别,识别请调用updateFrame
     */
    public List<FaceInfo> getFaceInfo(final byte[] data, final int width, final int height, int imageRoate) {
        if (!initTrackerKeySucceed) {
            L.e(TAG, "TrackerKey还未初始化");
            return new ArrayList<>();
        }
        if (!initFaceVerKeySucceed) {
            L.e(TAG, "FaceVerificationKey还未初始化");
            return new ArrayList<>();
        }
        return ulSeeTrackerManager.updateFrame(data, width, height, imageRoate);
    }

    public List<FaceInfo> updateFrame(final byte[] data, final int width, final int height, final int imageRoate, List<FaceInfo> faceInfos) {
        if (!initTrackerKeySucceed) {
            L.e(TAG, "TrackerKey还未初始化");
            return new ArrayList<>();
        }
        if (!initFaceVerKeySucceed) {
            L.e(TAG, "FaceVerificationKey还未初始化");
            return new ArrayList<>();
        }
        if (faceInfos != null || faceInfos.size() == 0) {
            final List<FaceInfo> paramFaceInfos = new ArrayList<>();
            for (FaceInfo faceInfo : faceInfos) {
                paramFaceInfos.add(faceInfo.clone());
            }
            if (!detectionRunning) {
                detectionRunning=true;
                final ExtractCallback fpListener = extractCallback;
                faceRecognitionHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        detectionRunning = true;
                        try {
                            if(paramFaceInfos != null && paramFaceInfos.size() >0 ){
                                if (fpListener != null) {
                                    fpListener.onExtractStart(data, width, height, paramFaceInfos);
                                }
                                delFaceVer(data, width, height,imageRoate, paramFaceInfos);
                                if (fpListener != null) {
                                    fpListener.onExtractComplete(data, width, height, paramFaceInfos);
                                }
                            }
                            synchronized (cacheLock) {
                                cacheFaceInfos = paramFaceInfos;
                            }
                        }catch (Exception e){
                            e.printStackTrace();
                        }finally {
                            detectionRunning=false;
                        }
                    }
                });
            }
            synchronized (cacheLock) {
                if (cacheFaceInfos != null) {
                    for (FaceInfo faceInfo : faceInfos) {
                        for (FaceInfo cacheFaceInfo : cacheFaceInfos) {
                            if (faceInfo.id == cacheFaceInfo.id) {
                                // faceInfo内的其他信息是本帧的结果，cacheFaceInfo.getFeatures() 是上一次算法结果
                                faceInfo.setFeatures(cacheFaceInfo.getFeatures());
                            }
                        }
                    }
                }
            }
        }
        return faceInfos;
    }

    /**
     * 人脸识别-视频流版本 同步提取特征值接口
     * @param data 图片数据，格式nv21
     * @param width 图片的宽
     * @param height 图片的高
     * @param imageRoate 逆时针旋转图片至人脸朝上，所需旋转的角度（0/90/180/270）
     * @return 人脸信息
     */
    public List<FaceInfo> updateFrameSync(final byte[] data, final int width, final int height, final int imageRoate) {
        List<FaceInfo> faceInfos = new ArrayList<>();
        if (!initFaceVerKeySucceed) {
            L.e(TAG, "FaceVerificationKey还未初始化");
            return faceInfos;
        }
        FaceInfo[] faceInfosArray = nativeExtactFeatureDataSync(detectorHandle, extractorHandle, data, width, height, imageRoate, imageDataType);
        if (faceInfosArray != null) {
            for (int i = 0; i < faceInfosArray.length; i++) {
                faceInfos.add(faceInfosArray[i]);
                faceInfosArray[i].id = i;
            }
        }
        return faceInfos;
    }

    /**
     * 根据faceinfo同步调用
     *
     * @param data
     * @param width
     * @param height
     * @param faceInfos
     * @return
     */
    public List<FaceInfo> updateFrameSync(final byte[] data, final int width, final int height, final int imageRoate, List<FaceInfo> faceInfos) {
        if (!initFaceVerKeySucceed) {
            L.e(TAG, "FaceVerificationKey还未初始化");
            return faceInfos;
        }
        delFaceVer(data, width, height, imageRoate, faceInfos);
        return faceInfos;
    }

    private void delFaceVer(byte[] data, int width, int height, int imageRoate, List<FaceInfo> paramFaceInfos) {
        if(paramFaceInfos == null || paramFaceInfos.size() == 0){
            return;
        }
/*        if(paramFaceInfos.size() == 1){
            //如果只有一张人脸, 使用更精确的detetcor取5点的方式.
            // 虽然目前测试了三张图片, 两种方式最后的比对分数并无差异
            //如果多张人脸,因为分别抠图和detect取5点, 会增加耗时, 就用原先的方式
            //人脸识别暂时不用detector取5点, 因为bitmap录底库的时候用detector取5点, 然后multiTracker在人脸多的时候用Tracker的5点, 估计效果会匹配不上.
            delFaceVerUsingDetector(data, width, height,imageRoate, paramFaceInfos);
            return;
        }*/
        float[][] multFacePoints = getPointsFromFaceInfos(paramFaceInfos);
        long t0 = System.currentTimeMillis();
        float[][] features = nativeExtactFeatureData(extractorHandle, data, width, height, multFacePoints, imageDataType);
        long t1 = System.currentTimeMillis();
        L.d(TAG, String.format("nativeExtactFeatureData duration=%d priviewSize=%dx%d", (t1 - t0), width, height));
        if (features != null && features.length == paramFaceInfos.size()) {
            for (int j = 0; j < features.length; j++) {
                paramFaceInfos.get(j).setFeatures(features[j]);
            }
        }
    }

    /**
     * todo 还有bug, 人脸在屏幕边缘时获取feature为null, 待处理
     * 在三星手机上, detector需要200ms, 提取feature依旧是100ms, 所以耗时为原来的三倍
     * @param data
     * @param width
     * @param height
     * @param imageRoate
     * @param faceInfos
     * @return
     */
    private void delFaceVerUsingDetector(byte[] data, int width, int height, final int imageRoate, List<FaceInfo> faceInfos) {
        if (faceInfos != null && faceInfos.size() > 0) {
            float[][] pointsSixSix = new float[faceInfos.size()][];
            for (int i = 0; i < faceInfos.size(); i++) {
                pointsSixSix[i] = Arrays.copyOf(faceInfos.get(i).points, faceInfos.get(i).points.length);
            }
            Log.d("delFaceVerUsingDetector", "start");
            float[][] features = nativeExtactFeatureDataUsingDetector(detectorHandle, extractorHandle, data, width, height, imageRoate, pointsSixSix,imageDataType);
            Log.d("delFaceVerUsingDetector", "end");
            if (features != null && features.length == faceInfos.size()) {
                for (int j = 0; j < features.length; j++) {
                    faceInfos.get(j).setFeatures(features[j]);
                }
            }
        }
    }

    /**
     * 根据66点计算得出五点信息
     * @param faceInfos
     * @return
     */
    private float[][] getPointsFromFaceInfos(List<FaceInfo> faceInfos){
        if (faceInfos == null){
            return new float[0][];
        }
        int faceNum = faceInfos.size();
        float[][] multFacePoints = new float[faceNum][];
        for (int i = 0; i < faceNum; i++) {
            FaceInfo faceInfo = faceInfos.get(i);
            float[] face5point = PointUtils.cal5Point(faceInfo.getPoints());
            multFacePoints[i] = face5point;
        }
        return multFacePoints;
    }

    /**
     * 算两个人脸特征的相似度
     * @param features1 第一个特征值
     * @param features2 第二个特征值
     * @return 相似度分数
     */
    public static float caculateSimilarity( float[] features1, float[] features2) {
        if (features1 == null || features2 == null || features1.length != features2.length) {
            return 0.f;
        }

        final int feature_length = features1.length;
        double ab_mult_add = 0.0f;
        double a_len = 0.0;
        double b_len = 0.0;

        for (int i = 0; i < feature_length; i++) {
            ab_mult_add += (features1[i] * features2[i]);
            a_len += features1[i] * features1[i];
            b_len += features2[i] * features2[i];
        }
        float vcos = (float) (1 - ab_mult_add / Math.sqrt(a_len * b_len));
        float score = (float) (1 / (Math.exp(15 * (vcos - 0.5)) + 1));

        return score;

    }

    public void reset() {
        if (ulSeeTrackerManager != null) {
            ulSeeTrackerManager.reset();
        }
        if(faceRecognitionHandler != null){
            faceRecognitionHandler.removeCallbacksAndMessages(null);
        }
        detectionRunning = false;
    }

    /**
     * 销毁
     */
    public void destroy() {
        if (initTrackerKeySucceed){
            ulSeeTrackerManager.destroy();
            initTrackerKeySucceed = false;
        }
        if (initFaceVerKeySucceed){
            nativeReleaseDetector(detectorHandle);
            nativeReleaseExtractor(extractorHandle);
            detectorHandle = 0;
            extractorHandle = 0;
            initFaceVerKeySucceed = false;
        }
       if (faceRecognitionThread != null){
           faceRecognitionThread.quitSafely();
       }
       detectionRunning = false;
    }


    /**
     * 异步特征值提取回调
     * @param extractCallback
     */
    public void setExtractCallback(ExtractCallback extractCallback) {
        // 切换到不同界面时reset一下
        reset();
        this.extractCallback = extractCallback;
    }

    public void setNativeDataCallback(NativeDataCallBack nativeDataCallback) {
        // 切换到不同界面时reset一下
        this.nativeDataCallBack = nativeDataCallback;
    }



    public static String getVersion() {
        return BuildConfig.VERSION_NAME;
    }

    public interface ExtractCallback {
        void onExtractStart(byte[] data, int width, int height, List<FaceInfo> faceInfos);
        void onExtractComplete(byte[] data, int width, int height, List<FaceInfo> faceInfos);


    }

    /**
     *  处理过程中某些数据的回调
     */
    public interface NativeDataCallBack {

        /**
         * float数据的回调
         * 数组第一个元素是人脸特征提取算法的耗时 单位是毫秒;
         * 如果某个元素的取值是-1 表示没有值;
         * @param data
         */
        void onNativeFloatDataAvailable(float[] data);
    }

    private  native long nativeInitDetector(AssetManager assetManager, float minSize);

    private native long nativeInitExtractor(AssetManager assetManager, String key, String keySecret, String macString);

    /**
     *
     * @param extractorHandle
     * @param image
     * @param points 顺序: [x0,y0,x1,y1,x2,y2,x3,y3,x4,y4]
     * @return
     */
    private native float[][] nativeExtactFeatureBitmap(long extractorHandle, Bitmap image, float[][] points);

    //points[i]: [x0,y0,x1,y1,x2,y2,x3,y3,x4,y4]
    private native float[][] nativeExtactFeatureData(long extractorHandle, byte[] data, int width, int height, float[][] points, int imageDataType);

    // 适合单人脸,多人脸还需调试和测试
    private native float[][] nativeExtactFeatureDataUsingDetector(long detectorHandle, long extractorHandle,
                                                                  byte[] data, int width, int height, int imageRoate, float[][] pointsSixSix, int imageDataType);

    private native FaceInfo[] nativeExtactFeatureDataSync(long detectHandle, long extractorHandle, byte[] data, int width, int height, int imageRotate, int imageDataType);

    private native void nativeReleaseDetector(long handle);

    private native void nativeReleaseExtractor(long handle);

}
