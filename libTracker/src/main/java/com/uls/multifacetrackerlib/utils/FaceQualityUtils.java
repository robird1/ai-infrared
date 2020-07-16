package com.uls.multifacetrackerlib.utils;

import com.uls.multifacetrackerlib.bean.FaceInfo;

/**
 * Created by zhangchao on 18-1-12.
 */

public class FaceQualityUtils {

    public static void goodFaceQuality(FaceInfo faceInfo) {
        float[] confidances = faceInfo.getConfidance();
        float sum = 0f;
        for (float confidance : confidances) {
            sum += confidance;
        }
        float avg = sum / confidances.length;
        int yawDegrees = (int) Math.toDegrees(faceInfo.euler.yaw);
        int pitchDegrees = (int) Math.toDegrees(faceInfo.euler.pitch);
        faceInfo.setIsPerfect(Float.compare(avg, 0.4f) > 0 &&
                (Math.abs(yawDegrees) < 35)  &&
                pitchDegrees < 30 && pitchDegrees > -15);
    }

}
