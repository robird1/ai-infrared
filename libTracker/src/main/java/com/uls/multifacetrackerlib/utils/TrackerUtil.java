package com.uls.multifacetrackerlib.utils;

import android.graphics.Bitmap;
import android.hardware.Camera;

import com.uls.multifacetrackerlib.bean.FaceInfo;

/**
 * Created by ian on 2017/3/27.
 */

public class TrackerUtil {


    static public float[] getFlipPoints(float[] inShape, int imageOrientation, boolean flip,
                                        Camera.Size camSize, boolean is106pts) {
        int length = inShape.length;
        float outShape[] = new float[length];

        if (flip) {
            switch (imageOrientation) {
                case 0:
                case 180:
                    for (int i = 0; i < length / 2; i++) {
                        outShape[i * 2 + 0] = inShape[i * 2 + 0];
                        outShape[i * 2 + 1] = camSize.height - inShape[i * 2 + 1];
                    }
                    break;
                case 90:
                case 270:
                    for (int i = 0; i < length / 2; i++) {
                        outShape[i * 2 + 0] = camSize.width - inShape[i * 2 + 0];
                        outShape[i * 2 + 1] = inShape[i * 2 + 1];
                    }
                    break;
            }
            if (is106pts) {
                ForeheadAdder106pts.switchPoints(outShape);
            } else {
                ForeheadAdder.switchPoints(outShape);
            }
        } else {
            for (int i = 0; i < length / 2; i++) {
                outShape[i * 2 + 0] = inShape[i * 2 + 0];
                outShape[i * 2 + 1] = inShape[i * 2 + 1];
            }
        }
        return outShape;
    }

    public static byte[] getNV21(int inputWidth, int inputHeight, Bitmap scaled) {

        int[] argb = new int[inputWidth * inputHeight];
        scaled.getPixels(argb, 0, inputWidth, 0, 0, inputWidth, inputHeight);
        byte[] yuv = new byte[inputWidth * inputHeight * 3 / 2];
        encodeYUV420SP(yuv, argb, inputWidth, inputHeight);
        return yuv;
    }

    public static void encodeYUV420SP(byte[] yuv420sp, int[] argb, int width, int height) {
        final int frameSize = width * height;

        int yIndex = 0;
        int uvIndex = frameSize;

        int a, R, G, B, Y, U, V;
        int index = 0;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {

                a = (argb[index] & 0xff000000) >> 24; // a is not used obviously
                R = (argb[index] & 0xff0000) >> 16;
                G = (argb[index] & 0xff00) >> 8;
                B = (argb[index] & 0xff) >> 0;

                // well known RGB to YUV algorithm
                Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
                U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
                V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

                // NV21 has a plane of Y and interleaved planes of VU each sampled by a factor of 2
                //    meaning for every 4 Y pixels there are 1 V and 1 U.  Note the sampling is every other
                //    pixel AND every other scanline.
                yuv420sp[yIndex++] = (byte) ((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
                if (j % 2 == 0 && index % 2 == 0) {
                    yuv420sp[uvIndex++] = (byte) ((V < 0) ? 0 : ((V > 255) ? 255 : V));
                    yuv420sp[uvIndex++] = (byte) ((U < 0) ? 0 : ((U > 255) ? 255 : U));
                }

                index++;
            }
        }
    }

    public static void expandFaceInfo(FaceInfo faceInfo) {
        if (faceInfo == null) {
            return;
        }
        float[] points = faceInfo.getPoints();
        float[] newPoints = new float[points.length + 44 * 2]; // 113~156
        System.arraycopy(points, 0, newPoints, 0, points.length);

        // 0~32 -> 113~145
        for (int i = 0; i <= 32; i++) {
            expandPoint(newPoints, 113 + i, 0 + i);
        }

        // 67~68 -> 146~147
        for (int i = 0; i <= 1; i++) {
            expandPoint(newPoints, 146 + i, 67 + i);
        }

        // 104~112 -> 148~156
        for (int i = 0; i <= 8; i++) {
            expandPoint(newPoints, 148 + i, 104 + i);
        }

        faceInfo.setPoints(newPoints);
    }

    private static void expandPoint(float[] points, int dstIndex, int expandIndex) {
        // 鼻尖
        float centerX = points[46 * 2];
        float centerY = points[46 * 2 + 1];
        float expandX = points[expandIndex * 2];
        float expandY = points[expandIndex * 2 + 1];

        float vectorX = expandX - centerX;
        float vectorY = expandY - centerY;

        final float expandRatio = 10.0f;

        points[dstIndex * 2] = expandX + vectorX * expandRatio;
        points[dstIndex * 2 + 1] = expandY + vectorY * expandRatio;
    }
}
