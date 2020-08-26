package com.uls.multifacetrackerlib.utils;

import android.app.Activity;
import android.view.Surface;

import static com.uls.multifacetrackerlib.utils.PointUtils.getRoatePoints;

/**
 * ================================================
 * 作    者：aguai（吴红斌）Github地址：https://github.com/aguai1
 * 版    本：1.0
 * 创建日期：18-4-19
 * 描    述：
 * ================================================
 */
public class PointConvertUtils {
    public static double[] convertPoints(Activity activity, double[] origin, int width, int height, boolean isFront) {
        if (origin == null) {
            return null;
        }
        int rotation = activity.getWindowManager().getDefaultDisplay().getRotation();
        int degrees = 0;
        switch (rotation) {
            case Surface.ROTATION_0:
                degrees = 90;
                break;
            case Surface.ROTATION_90:
                degrees = 0;
                break;
            case Surface.ROTATION_180:
                degrees = 270;
                break;
            case Surface.ROTATION_270:
                degrees = 180;
                break;
        }
        return convertFaceInfo(degrees, origin, width, height, isFront);
    }

    public static double[] convertFaceInfo(int degrees, double[] originalShape, int width, int height, boolean isFront) {
//        if (originalShape == null) {
//            return null;
//        } else {
//            float[] flipPoints = PointUtils.getFlipPoints(PointUtils.doubleToFloat(originalShape), isFront, width, true);
//            flipPoints = getRoatePoints(flipPoints, degrees, width, height);
//            return PointUtils.floatToDouble(flipPoints);
//        }
        return null;
    }
}
