package com.uls.multifacetrackerlib;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.RectF;
import android.os.Handler;
import android.os.HandlerThread;

import com.uls.multifacetrackerlib.bean.Euler;
import com.uls.multifacetrackerlib.bean.FaceInfo;
import com.uls.multifacetrackerlib.utils.FaceQualityUtils;
import com.uls.multifacetrackerlib.utils.L;
import com.uls.multifacetrackerlib.utils.PointUtils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * 人脸跟踪管理器
 */
public class ULSeeTrackerManager {

    private static final String TAG = "ULSeeTrackerManager";

    private int mMaxTrackers = 5;
    private float thresholdFirst = 0.35f;
    private float thresholdLast = 0.2f;
    //检测的人脸mat(即抠过的带背景的图,比人脸的范围稍大), columns或者rows需要至少有一边近似或者大于这个数(经测试,设置成500时,需要有一边大于等于450)
    private int minFaceSize = 120;
    private Context context;
    private boolean checkPerfect;
    private UlsMultiTracker mTracker;
    private int facesAlive;
    private long lastDetectionDoneTime;
    private boolean detectionRunning;
    private HandlerThread trackerThread;
    private Handler trackerHandler;
    private boolean isFinish;
    public static boolean debugMode;
    private int imageDataType = UlsMultiTracker.ImageDataType.NV21.getValue(); //默认NV21

    // 通过trackerState计算人脸id
    // length和mMaxTrackers一样.数值含义:-1 人脸已经消失; 0 刚检测到人脸; 1;这次是第N次检测到人脸
    private int[] trackerState;
    private int[] trackerId;

    private int distributiveAbleId = 0;
    private boolean initKeySucceed;

    public ULSeeTrackerManager(Context context) {
        this.context = context;
    }

    public ULSeeTrackerManager setThresholdFirst(float thresholdFirst) {
        this.thresholdFirst = thresholdFirst;
        return this;
    }

    public ULSeeTrackerManager setThresholdLast(float thresholdLast) {
        this.thresholdLast = thresholdLast;
        return this;
    }

    /**
     * 最小人脸检测大小设置，默认120
     * 请在init之前调用
     * @param minFaceSize 最小人脸检测大小
     * @return this
     */
    public ULSeeTrackerManager setMinFaceSize(int minFaceSize) {
        this.minFaceSize = minFaceSize;
        return this;
    }

    /**
     * 最多跟踪多少个人脸设置，默认5
     * 请在init之前调用
     * @param maxTrackers 最多跟踪人脸数
     * @return this
     */
    public ULSeeTrackerManager setMaxTrackers(int maxTrackers) {
        this.mMaxTrackers = maxTrackers;
        return this;
    }

    public int getmMaxTrackers() {
        return mMaxTrackers;
    }

    public ULSeeTrackerManager checkPerfect(boolean perfect) {
        this.checkPerfect = perfect;
        return this;
    }

    /**
     * 初始化
     * @param keyString
     * @return 初始化的状态码
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
     * @param secret
     * @return 初始化的状态码
     */
    public int init(String key, String secret) {
        mTracker = new UlsMultiTracker(context, mMaxTrackers, minFaceSize);
        int status = mTracker.activate(key, secret);
        if (status != ULSeeError.SUCCESSED) {
            L.e(TAG, "验证key失败");
            mTracker.dispose();
            initKeySucceed = false;
        } else {
            mTracker.initialise();
            mTracker.setTrackerConfidenceThreshold(thresholdFirst, thresholdLast);
            this.mMaxTrackers = mTracker.getTrackerCount();
            trackerState = new int[this.mMaxTrackers];
            Arrays.fill(trackerState, -1);
            trackerId = new int[this.mMaxTrackers];
            Arrays.fill(trackerId, -1);
            initKeySucceed = true;
            openTrackerThread();
        }

        return status;
    }

    private void openTrackerThread() {
        if (trackerThread != null) {
            trackerThread.quit();
        }
        trackerThread = new HandlerThread("Tracker Thread");
        trackerThread.start();
        trackerHandler = new Handler(trackerThread.getLooper());
    }

    public int[] getTrackerState() {
        return trackerState;
    }

    private List<FaceInfo> getFaceInfosFromTracker() {
        List<FaceInfo> faceInfos = new ArrayList<>();
        for (int i = 0; i < mMaxTrackers; i++) {
            float[] originalShape = mTracker.getShape(i);
            int state = mTracker.getTrackerState(i);
            // 首先将C层的tracker state更新到java层
            if (trackerState[i] != state) {
                L.d("zhangc", String.format("_state[%d] %d->%d", i, trackerState[i], state));
                trackerState[i] = state;
            }
            // 如果tracker state是-1，那么说明人脸已经丢失，trackerId重置为-1
            if (trackerState[i] == -1) {
                trackerId[i] = -1;
            }
            if (originalShape != null && trackerState[i] != -1) {
                FaceInfo faceInfo = new FaceInfo();
                faceInfo.id = calcFaceId(i);
                faceInfo.index = i;
                faceInfo.setScale(mTracker.getScaleInImage(i));
                faceInfo.setPoints(originalShape);
                faceInfo.setPupil(mTracker.getPupils(i));
                faceInfo.setPoint3Ds(mTracker.getShape3D(i));
                faceInfo.setTranslationInImages(mTracker.getTranslationInImage(i));
                RectF rectF = PointUtils.points2rect(originalShape, faceInfo.getScale());
                faceInfo.setRect(rectF);
                float[] rotationAngles = mTracker.getRotationAngles(i);
                float[] confidance = mTracker.getConfidence(i);

                faceInfo.setConfidance(confidance);
                faceInfo.setRotationAngles(rotationAngles);
                Euler euler = new Euler();
                euler.roll = rotationAngles[2];
                euler.pitch = rotationAngles[0];
                euler.yaw = rotationAngles[1];
                faceInfo.setEuler(euler);
                if (checkPerfect) {
                    FaceQualityUtils.goodFaceQuality(faceInfo);
                }
                faceInfos.add(faceInfo);

            }
        }
        return faceInfos;
    }

    /**
     * 计算i位置的人脸信息当前帧的face id
     * @param i 索引
     * @return 当前帧的face id
     */
    private int calcFaceId(int i) {
        int id = -1;
        int state = trackerState[i];
        switch (state) {
            case 0:
                id = distributiveAbleId++;
                trackerId[i] = id;
                if (distributiveAbleId == Integer.MAX_VALUE) {
                    distributiveAbleId = 0;
                }
                break;
            case 1:
                id = trackerId[i];
                break;
        }

        return id;
    }

    /**
     * 人脸跟踪-视频流版本
     * 建议在UI线程调用此方法
     * @param data 图片数据，格式 NV21/ARGB/BGR
     * @param width 图片的宽
     * @param height 图片的高
     * @param imageRoate 逆时针旋转图片至人脸朝上，所需旋转的角度（0/90/180/270）
     * @return
     */
    public List<FaceInfo> updateFrame(final byte[] data, final int width, final int height, final int imageRoate) {
        if (isFinish) {
            return null;
        }
        if (!initKeySucceed) {
            L.e(TAG, "key验证失败，或未初始化");
            return new ArrayList<>();
        }
        final int finalImageRoate = (imageRoate % 360);
        long currentMills = System.currentTimeMillis();
        L.d(TAG, "updateFrame currentMills:" + currentMills);


        boolean detectFace;
        //如果tracking的人脸数未达到最大人脸数， 并且上一次人脸检测已经间隔500ms, 并且不是正在检测人脸,那么就去做人脸检测算法
        //因为tracker底层的逻辑是, 如果人脸数==mMaxTrackers, 就不会使用detector的数据
        //间隔500m是为了防止没人时, 设备一直跑detector导致CPU占用一直很高, 进而导致设备发热
        detectFace = facesAlive < mMaxTrackers && (currentMills - lastDetectionDoneTime > 500) && !detectionRunning;
        if (detectFace) {
            detectionRunning = true;
            if (trackerHandler != null) {
                trackerHandler.post(new Runnable() {
                    @Override
                    public void run() {
                        L.d(TAG, "start findFace");
                        // 运行人脸检测算法，并将检测到人人脸信息加入到缓存中，以供tracker update时使用
                        boolean findFace = mTracker.findFacesAndAdd(data, width, height, finalImageRoate, imageDataType);
                        // naMultiGetFaceRect是获取底层人脸检测结果，下标0的位置为检测到人脸数量，后续为脸框数据4个一组
                        int[] faceRects = mTracker.naMultiGetFaceRect();
                        if (faceRects != null && faceRects.length > 0) {
                            L.d(TAG, "findFace count:" + faceRects[0]);
                        }
                        lastDetectionDoneTime = System.currentTimeMillis();
                        L.d(TAG, "findFace:" + findFace + ",lastDetectionDoneTime=" + lastDetectionDoneTime);
                        detectionRunning = false;
                    }
                });
            }
        }
        // 更新当前这一帧的人脸跟踪结果
        facesAlive = mTracker.update(data, width, height, imageDataType);
        L.d(TAG, "facesAlive:" + facesAlive + ", " + "imageRoate:" + imageRoate);
        // 封装人脸信息并返回
        return getFaceInfosFromTracker();
    }

    public void reset() {
        if (!initKeySucceed) {
            L.e(TAG, "key验证失败，或未初始化");
            return;
        }
        try {
            for (int i = 0; i < mMaxTrackers; i++) {
                mTracker.resetTracker(i);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 销毁
     */
    public void destroy() {
        if (!initKeySucceed) {
            L.e(TAG, "key验证失败，或未初始化");
            return;
        }
        isFinish = true;
        if (trackerHandler != null) {
            trackerHandler.post(new Runnable() {
                @Override
                public void run() {
                    mTracker.dispose();
                }
            });
        } else {
            mTracker.dispose();
        }
    }

    /**
     * 人脸跟踪-图片版本
     * @param bitmap 输入图片，格式Bitmap.Config.ARGB_8888或者Bitmap.Config.ARGB_565
     * @return 人脸信息 66点
     */
    public List<FaceInfo> trackerBitmap(Bitmap bitmap) {
        if (!initKeySucceed) {
            L.e(TAG, "key验证失败，或未初始化");
            return new ArrayList<>();
        }

        if (bitmap == null || bitmap.isRecycled()) {
            return new ArrayList<>();
        }

        // mTracker.trackerBitmap之前需要调用reset,因为为了在视频流track时平滑绘制，会保存上一帧的数据，
        // 所以track bitmap时,需要清理掉上一次的tracker结果
        reset();
        mTracker.trackerBitmap(bitmap);
        // 封装人脸信息并返回
        return getFaceInfosFromTracker();
    }

    /**
     * 检测图片模糊程度, 模糊因子请根据需求调节
     * @param bitmap 要检测模糊的图片，格式Bitmap.Config.ARGB_8888或者Bitmap.Config.ARGB_565
     * @param firstparam 模糊因子1,取值范围[0,1], 建议值0.968;数值越小,通过测试的图片越清晰
     * @param secondparam 模糊因子2,建议值6;数值越大,通过测试的图片越清晰
     * @param rect 裁剪范围 如不裁剪,传null
     * @return 如果通过测试(图片清晰),则返回true;未通过测试,返回false
     */
    public boolean checkBlur(Bitmap bitmap, float firstparam, float secondparam, Rect rect) {
        //新建对象以避免修改入参对象
        Rect bound;
        if(rect == null){
            bound = new Rect(0,0,bitmap.getWidth(), bitmap.getHeight());
        }else {
            bound = new Rect();
            //检查边界
            bound.left = Math.max( rect.left, 0);
            bound.top = Math.max(rect.top, 0);
            bound.right = Math.min(rect.right, bitmap.getWidth());
            bound.bottom = Math.min(rect.bottom, bitmap.getHeight());
        }
        return mTracker.checkBlur(bitmap, firstparam, secondparam,bound);
    }

    /**
     * 检测人脸信息质量
     * @param faceInfo
     * @param confidence 检测置信度, 范围[0,1],值越大越严格,推荐值0.5
     * @param yawD 检测yaw角度, 必须为正数, 值越小越严格,推荐值30,
     * @param pitchD 检测pitch角度  必须为正数, 值越小越严格,推荐值20
     * @return 如果通过测试,返回true, 未通过返回false
     */
    public static boolean checkFaceInfoQuality(FaceInfo faceInfo, float confidence, float yawD, float pitchD) {
        float[] confidances = faceInfo.getConfidance();
        float sum = 0f;
        for (float confidance : confidances) {
            sum += confidance;
        }
        float avg = sum / confidances.length;
        int yawDegrees = (int) Math.toDegrees(faceInfo.euler.yaw);
        int pitchDegrees = (int) Math.toDegrees(faceInfo.euler.pitch);
        return Float.compare(avg, confidence) > 0 && (Math.abs(yawDegrees) < yawD) && (Math.abs(pitchDegrees) < pitchD);
    }

    /**
     * @param bitmap
     * @return FaceInfo[] 五点信息顺序: x0,y0,x1,y1...
     */
    public FaceInfo[] processImageByDetector(Bitmap bitmap){
        return mTracker.processImageByDetector(bitmap);
    }

   public Bitmap crop(Bitmap srcBmp, Rect rect) {
       return mTracker.crop(srcBmp,rect);
   }

    public int getImageDataType() {
        return imageDataType;
    }

    public ULSeeTrackerManager setImageDataType(int imageDataType) {
        this.imageDataType = imageDataType;
        return this;
    }
}
