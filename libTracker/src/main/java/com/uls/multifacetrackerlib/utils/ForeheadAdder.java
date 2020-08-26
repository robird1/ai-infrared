package com.uls.multifacetrackerlib.utils;

/**
 * Created by ian on 2016/11/3.
 */
public class ForeheadAdder {
    private static final float CLOSE_THRESHOLD = 0.85f;
    private static int[][] lastDirection = new int[5][3];
    private static float[][] trackedShape = new float[5][8];
    private static float[][] outputShape = new float[5][198];
    private static float tempX, tempY;

    public static float[] addForehead(float[] inputShape) {
        float[] kShape = new float[198];

        for (int i = 0; i < 132; i++) {
            kShape[i] = inputShape[i];
        }
        float vectorX = getX(inputShape, 27) - getX(inputShape, 30);
        float vectorY = getY(inputShape, 27) - getY(inputShape, 30);
        set(kShape, 86, (float) (getX(inputShape, 17) + 0.45 * vectorX), (float) (getY(inputShape, 17) + 0.45 * vectorY));
        set(kShape, 87, (float) (getX(inputShape, 18) + 0.35 * vectorX), (float) (getY(inputShape, 18) + 0.35 * vectorY));
        set(kShape, 88, (float) (getX(inputShape, 19) + 0.45 * vectorX), (float) (getY(inputShape, 19) + 0.45 * vectorY));
        set(kShape, 89, (float) (getX(inputShape, 20) + 0.5 * vectorX), (float) (getY(inputShape, 20) + 0.5 * vectorY));
        set(kShape, 90, (float) (getX(inputShape, 21) + 0.6 * vectorX), (float) (getY(inputShape, 21) + 0.6 * vectorY));
        float tempX = (getX(inputShape, 21) + getX(inputShape, 22)) / 2;
        float tempY = (getY(inputShape, 21) + getY(inputShape, 22)) / 2;
        set(kShape, 91, (float) (tempX + 0.6 * vectorX), (float) (tempY + 0.6 * vectorY));
        set(kShape, 92, (float) (getX(inputShape, 22) + 0.6 * vectorX), (float) (getY(inputShape, 22) + 0.6 * vectorY));
        set(kShape, 93, (float) (getX(inputShape, 23) + 0.5 * vectorX), (float) (getY(inputShape, 23) + 0.5 * vectorY));
        set(kShape, 94, (float) (getX(inputShape, 24) + 0.45 * vectorX), (float) (getY(inputShape, 24) + 0.45 * vectorY));
        set(kShape, 95, (float) (getX(inputShape, 25) + 0.35 * vectorX), (float) (getY(inputShape, 25) + 0.35 * vectorY));
        set(kShape, 96, (float) (getX(inputShape, 26) + 0.45 * vectorX), (float) (getY(inputShape, 26) + 0.45 * vectorY));
//        set(outputShape, 97, (float) (getX(inputShape,0)+0.45*vectorX), (float) (getY(inputShape,0)+0.45*vectorY));
//        set(outputShape, 98, (float) (getX(inputShape,16)+0.45*vectorX), (float) (getY(inputShape,16)+0.45*vectorY));
        tempX = (float) ((getX(kShape, 0) + getX(kShape, 86)) / 2 + 0.2 * vectorX);
        tempY = (float) ((getY(kShape, 0) + getY(kShape, 86)) / 2 + 0.2 * vectorY);
        set(kShape, 97, tempX, tempY);
        tempX = (float) ((getX(kShape, 16) + getX(kShape, 96)) / 2 + 0.2 * vectorX);
        tempY = (float) ((getY(kShape, 16) + getY(kShape, 96)) / 2 + 0.2 * vectorY);
        set(kShape, 98, tempX, tempY);
        return kShape;
    }

    public static float[] addForehead(float[] inputShape, float scale, int index) {
        // region Shivering issue solution.
        boolean needUpdateShape = true;
        int trackedDirection = 0;

        float pow51 = (trackedShape[index][0] - inputShape[51 * 2 + 0]) * (trackedShape[index][0] - inputShape[51 * 2 + 0]) +
                (trackedShape[index][1] - inputShape[51 * 2 + 1]) * (trackedShape[index][1] - inputShape[51 * 2 + 1]);
        float pow57 = (trackedShape[index][2] - inputShape[57 * 2 + 0]) * (trackedShape[index][2] - inputShape[57 * 2 + 0]) +
                (trackedShape[index][3] - inputShape[57 * 2 + 1]) * (trackedShape[index][3] - inputShape[57 * 2 + 1]);
        if (pow51 > scale / 30 && pow57 > scale / 30 &&
                pow51 < scale / 2 && pow57 < scale / 2) {
            // Shivering, just use old value.
        } else {
            // Mouth Part: 48*2 ... 66*2
            for (int i = 96; i < 132; i++) outputShape[index][i] = inputShape[i];
        }

        float pow37 = (trackedShape[index][4] - inputShape[37 * 2 + 0]) * (trackedShape[index][4] - inputShape[37 * 2 + 0]) +
                (trackedShape[index][5] - inputShape[37 * 2 + 1]) * (trackedShape[index][5] - inputShape[37 * 2 + 1]);
        float pow40 = (trackedShape[index][6] - inputShape[40 * 2 + 0]) * (trackedShape[index][6] - inputShape[40 * 2 + 0]) +
                (trackedShape[index][7] - inputShape[40 * 2 + 1]) * (trackedShape[index][7] - inputShape[40 * 2 + 1]);
        if (pow37 > scale / 30 && pow40 > scale / 30 &&
                pow37 < scale / 1.5 && pow40 < scale / 1.5) {

            // 4-bit direction
            if (trackedShape[index][4] - inputShape[37 * 2 + 0] > 0) trackedDirection += 1;
            if (trackedShape[index][5] - inputShape[37 * 2 + 1] > 0) trackedDirection += 2;
            if (trackedShape[index][6] - inputShape[40 * 2 + 0] > 0) trackedDirection += 4;
            if (trackedShape[index][7] - inputShape[40 * 2 + 1] > 0) trackedDirection += 8;

            // If direction keep changing in 3 frames, guess it's shivering, don't update.
            if (!(lastDirection[index][0] == trackedDirection && lastDirection[index][1] == trackedDirection &&
                    lastDirection[index][2] == trackedDirection)) needUpdateShape = false;

            lastDirection[index][2] = lastDirection[index][1];
            lastDirection[index][1] = lastDirection[index][0];
            lastDirection[index][0] = trackedDirection;
        }

        if (needUpdateShape) {
            // Eye part: 36*2 ... 48*2
            for (int i = 72; i < 96; i++) outputShape[index][i] = inputShape[i];
        }

        trackedShape[index][0] = inputShape[51 * 2 + 0];
        trackedShape[index][1] = inputShape[51 * 2 + 1];
        trackedShape[index][2] = inputShape[57 * 2 + 0];
        trackedShape[index][3] = inputShape[57 * 2 + 1];
        trackedShape[index][4] = inputShape[37 * 2 + 0];
        trackedShape[index][5] = inputShape[37 * 2 + 1];
        trackedShape[index][6] = inputShape[40 * 2 + 0];
        trackedShape[index][7] = inputShape[40 * 2 + 1];
        // endregion

        for (int i = 0; i < 72; i++) {
            outputShape[index][i] = inputShape[i];
        }

        float vectorX = getX(inputShape, 27) - getX(inputShape, 30);
        float vectorY = getY(inputShape, 27) - getY(inputShape, 30);

        set(outputShape[index], 86, (float) (getX(inputShape, 17) + 0.45 * vectorX), (float) (getY(inputShape, 17) + 0.45 * vectorY));
        set(outputShape[index], 87, (float) (getX(inputShape, 18) + 0.35 * vectorX), (float) (getY(inputShape, 18) + 0.35 * vectorY));
        set(outputShape[index], 88, (float) (getX(inputShape, 19) + 0.45 * vectorX), (float) (getY(inputShape, 19) + 0.45 * vectorY));
        set(outputShape[index], 89, (float) (getX(inputShape, 20) + 0.5 * vectorX), (float) (getY(inputShape, 20) + 0.5 * vectorY));
        set(outputShape[index], 90, (float) (getX(inputShape, 21) + 0.6 * vectorX), (float) (getY(inputShape, 21) + 0.6 * vectorY));

        float tempX = (getX(inputShape, 21) + getX(inputShape, 22)) / 2;
        float tempY = (getY(inputShape, 21) + getY(inputShape, 22)) / 2;
        set(outputShape[index], 91, (float) (tempX + 0.6 * vectorX), (float) (tempY + 0.6 * vectorY));

        set(outputShape[index], 92, (float) (getX(inputShape, 22) + 0.6 * vectorX), (float) (getY(inputShape, 22) + 0.6 * vectorY));
        set(outputShape[index], 93, (float) (getX(inputShape, 23) + 0.5 * vectorX), (float) (getY(inputShape, 23) + 0.5 * vectorY));
        set(outputShape[index], 94, (float) (getX(inputShape, 24) + 0.45 * vectorX), (float) (getY(inputShape, 24) + 0.45 * vectorY));
        set(outputShape[index], 95, (float) (getX(inputShape, 25) + 0.35 * vectorX), (float) (getY(inputShape, 25) + 0.35 * vectorY));
        set(outputShape[index], 96, (float) (getX(inputShape, 26) + 0.45 * vectorX), (float) (getY(inputShape, 26) + 0.45 * vectorY));

//        set(outputShape, 97, (float) (getX(inputShape,0)+0.45*vectorX), (float) (getY(inputShape,0)+0.45*vectorY));
//        set(outputShape, 98, (float) (getX(inputShape,16)+0.45*vectorX), (float) (getY(inputShape,16)+0.45*vectorY));
        tempX = (float) ((getX(outputShape[index], 0) + getX(outputShape[index], 86)) / 2 + 0.2 * vectorX);
        tempY = (float) ((getY(outputShape[index], 0) + getY(outputShape[index], 86)) / 2 + 0.2 * vectorY);
        set(outputShape[index], 97, tempX, tempY);

        tempX = (float) ((getX(outputShape[index], 16) + getX(outputShape[index], 96)) / 2 + 0.2 * vectorX);
        tempY = (float) ((getY(outputShape[index], 16) + getY(outputShape[index], 96)) / 2 + 0.2 * vectorY);
        set(outputShape[index], 98, tempX, tempY);

        return outputShape[index];
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
                    float leftEye_1 = PointUtils.angleComputation(getX(floats, 36), getY(floats, 36), getX(floats, 37), getY(floats, 37), getX(floats, 41), getY(floats, 41));
                    float leftEye_2 = PointUtils.angleComputation(getX(floats, 39), getY(floats, 39), getX(floats, 38), getY(floats, 38), getX(floats, 40), getY(floats, 40));


                    float rightEye_1 = PointUtils.angleComputation(getX(floats, 42), getY(floats, 42), getX(floats, 43), getY(floats, 43), getX(floats, 47), getY(floats, 47));
                    float rightEye_2 = PointUtils.angleComputation(getX(floats, 45), getY(floats, 45), getX(floats, 44), getY(floats, 44), getX(floats, 46), getY(floats, 46));

                    if (leftEye_1 > CLOSE_THRESHOLD && leftEye_2 > CLOSE_THRESHOLD && rightEye_1 > CLOSE_THRESHOLD && rightEye_2 > CLOSE_THRESHOLD) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    public static void switchPoints(float[] inShape) {
        if (inShape.length < 66 * 2) {
            return;
        }
        for (int i = 0; i < 8; i++) {
            switchPoints(inShape, i, 16 - i);
        }
        for (int i = 17; i < 22; i++) {
            switchPoints(inShape, i, 43 - i);
        }
        switchPoints(inShape, 31, 35);
        switchPoints(inShape, 32, 34);
        for (int i = 36; i < 40; i++) {
            switchPoints(inShape, i, 81 - i);
        }
        switchPoints(inShape, 40, 47);
        switchPoints(inShape, 41, 46);
        for (int i = 48; i < 51; i++) {
            switchPoints(inShape, i, 102 - i);
        }
        switchPoints(inShape, 55, 59);
        switchPoints(inShape, 56, 58);
        switchPoints(inShape, 60, 62);
        switchPoints(inShape, 63, 65);
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
