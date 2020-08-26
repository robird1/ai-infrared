package com.uls.multifacetrackerlib.utils;

/**
 * Created by ian on 2016/11/3.
 */
public class ForeheadAdder106pts {
    private static final float CLOSE_THRESHOLD = 0.85f;
    private static float tempX, tempY;

    public static float[] addForehead(float[] inputShape) {
        float[] kShape = new float[inputShape.length + 7 * 2];
        System.arraycopy(inputShape, 0, kShape, 0, inputShape.length);
        float vectorX = getX(inputShape, 43) - getX(inputShape, 46);
        float vectorY = getY(inputShape, 43) - getY(inputShape, 46);
        set(kShape, 67, (float) (getX(inputShape, 33) + 0.45 * vectorX), (float) (getY(inputShape, 33) + 0.45 * vectorY));
        set(kShape, 68, (float) (getX(inputShape, 34) + 0.35 * vectorX), (float) (getY(inputShape, 34) + 0.35 * vectorY));
        set(kShape, 104, (float) (getX(inputShape, 35) + 0.40 * vectorX), (float) (getY(inputShape, 35) + 0.40 * vectorY));
        set(kShape, 105, (float) (getX(inputShape, 36) + 0.45 * vectorX), (float) (getY(inputShape, 36) + 0.45 * vectorY));
        set(kShape, 106, (float) (getX(inputShape, 37) + 0.6 * vectorX), (float) (getY(inputShape, 37) + 0.6 * vectorY));
        float tempX = (getX(inputShape, 37) + getX(inputShape, 38)) / 2;
        float tempY = (getY(inputShape, 37) + getY(inputShape, 38)) / 2;
        set(kShape, 107, (float) (tempX + 0.6 * vectorX), (float) (tempY + 0.6 * vectorY));
        set(kShape, 108, (float) (getX(inputShape, 38) + 0.6 * vectorX), (float) (getY(inputShape, 38) + 0.6 * vectorY));
        set(kShape, 109, (float) (getX(inputShape, 39) + 0.45 * vectorX), (float) (getY(inputShape, 39) + 0.45 * vectorY));
        set(kShape, 110, (float) (getX(inputShape, 40) + 0.40 * vectorX), (float) (getY(inputShape, 40) + 0.40 * vectorY));
        set(kShape, 111, (float) (getX(inputShape, 41) + 0.35 * vectorX), (float) (getY(inputShape, 41) + 0.35 * vectorY));
        set(kShape, 112, (float) (getX(inputShape, 42) + 0.45 * vectorX), (float) (getY(inputShape, 42) + 0.45 * vectorY));
        return kShape;
    }

    private static float getX(float[] inputShape, int index) {
        return inputShape[index * 2];
    }

    private static float getY(float[] inputShape, int index) {
        return inputShape[index * 2 + 1];
    }

    private static void set(float[] shape, int index, float x, float y) {
        shape[index * 2] = x;
        shape[index * 2 + 1] = y;
    }

    /**
     * @param shape
     * @return true 闭眼
     * false 睁眼
     */
    public static boolean checkBlink(float[][] shape) {
        if (shape == null) {
            return false;
        } else {
            for (int i = 0; i < shape.length; ++i) {
                float[] floats = shape[i];
                if (floats != null) {
                    float leftEye_1 = PointUtils.angleComputation(getX(floats, 52), getY(floats, 52), getX(floats, 53), getY(floats, 53), getX(floats, 57), getY(floats, 57));
                    float leftEye_2 = PointUtils.angleComputation(getX(floats, 55), getY(floats, 55), getX(floats, 54), getY(floats, 54), getX(floats, 56), getY(floats, 56));

                    float rightEye_1 = PointUtils.angleComputation(getX(floats, 58), getY(floats, 58), getX(floats, 59), getY(floats, 59), getX(floats, 63), getY(floats, 63));
                    float rightEye_2 = PointUtils.angleComputation(getX(floats, 61), getY(floats, 61), getX(floats, 60), getY(floats, 60), getX(floats, 62), getY(floats, 62));

                    if (leftEye_1 > CLOSE_THRESHOLD && leftEye_2 > CLOSE_THRESHOLD && rightEye_1 > CLOSE_THRESHOLD && rightEye_2 > CLOSE_THRESHOLD) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    public static void switchPoints(float[] inShape) {
        if (inShape.length < 106 * 2) {
            return;
        }
        for (int i = 0; i < 16; i++) {
            switchPoints(inShape, i, 32 - i);
        }
        for (int i = 33; i < 38; i++) {
            switchPoints(inShape, i, 75 - i);
        }
        for (int i = 64; i < 68; i++) {
            switchPoints(inShape, i, 135 - i);
        }
        switchPoints(inShape, 78, 79);
        switchPoints(inShape, 80, 81);
        switchPoints(inShape, 82, 83);
        switchPoints(inShape, 47, 51);
        switchPoints(inShape, 48, 50);
        for (int i = 52; i < 56; i++) {
            switchPoints(inShape, i, 113 - i);
        }
        switchPoints(inShape, 56, 63);
        switchPoints(inShape, 57, 62);
        switchPoints(inShape, 72, 75);
        switchPoints(inShape, 73, 76);
        switchPoints(inShape, 74, 77);

        for (int i = 84; i < 87; i++) {
            switchPoints(inShape, i, 174 - i);
        }
        for (int i = 96; i < 98; i++) {
            switchPoints(inShape, i, 196 - i);
        }
        switchPoints(inShape, 101, 103);
        switchPoints(inShape, 95, 91);
        switchPoints(inShape, 94, 92);

        switchPoints(inShape, 104, 105);
    }

    private static void switchPoints(float[] inShape, int index1, int index2) {
        tempX = inShape[index1 * 2 + 0];
        tempY = inShape[index1 * 2 + 1];
        inShape[index1 * 2 + 0] = inShape[index2 * 2 + 0];
        inShape[index1 * 2 + 1] = inShape[index2 * 2 + 1];
        inShape[index2 * 2 + 0] = tempX;
        inShape[index2 * 2 + 1] = tempY;
    }
}
