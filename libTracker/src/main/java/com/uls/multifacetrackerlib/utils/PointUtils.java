package com.uls.multifacetrackerlib.utils;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.RectF;
import android.view.Surface;

import com.uls.multifacetrackerlib.bean.FaceInfo;

import java.util.ArrayList;
import java.util.List;


public class PointUtils {

    public static float[] getFlipXPoints(float[] inShape, int width) {
        int length = inShape.length;
        float outShape[] = new float[length];
        System.arraycopy(inShape, 0, outShape, 0, length);
        for (int i = 0; i < length / 2; i++) {
            outShape[i * 2 + 0] = width - inShape[i * 2 + 0];
            outShape[i * 2 + 1] = inShape[i * 2 + 1];
        }

        ForeheadAdder.switchPoints(outShape);

        return outShape;
    }

    /**
     * 获取rect
     *
     * @param points
     * @param scale
     * @return
     */
    public static RectF points2rect(float[] points, float scale) {
        int maxValue = 60000;
        RectF rect = new RectF(maxValue, maxValue, -maxValue, -maxValue);
        for (int i = 0; i < points.length / 2; i++) {
            float x = points[i * 2];
            float y = points[i * 2 + 1];
//            if(Float.compare(x, 0) < 0) {
//                L.e("zhangc", "xy=" + x + "," + y);
//            }
            boolean pointInvalid = (Float.compare(x, -128.0f) == 0 && Float.compare(y, -128.0f) == 0);
            if (!pointInvalid) {
                rect.left = Math.min(rect.left, x);
                rect.top = Math.min(rect.top, y);
                rect.right = Math.max(rect.right, x);
                rect.bottom = Math.max(rect.bottom, y);
            }
        }

//        L.e("zhangc", "scale=" + scale);

        rect.left -= 5 * scale;
        rect.right += 5 * scale;
        rect.top -= 3 * scale;
        rect.bottom += 3 * scale;
        return rect;
    }


    /**
     * 逆时针旋转点
     * @param inShape
     * @param pointRoate
     * @return
     */
    public static float[] getRoatePoints(float[] inShape, int pointRoate, int width, int height) {
        int length = inShape.length;
        float outShape[] = new float[length];

        switch (pointRoate) {
            case 180:
                for (int i = 0; i < length / 2; i++) {
                    outShape[i * 2 + 0] = width - inShape[i * 2 + 0];
                    outShape[i * 2 + 1] = height - inShape[i * 2 + 1];
                }
                break;
            case 0:
                for (int i = 0; i < length / 2; i++) {
                    outShape[i * 2 + 0] = inShape[i * 2 + 0];
                    outShape[i * 2 + 1] = inShape[i * 2 + 1];
                }
                break;
            case 90:
                for (int i = 0; i < length / 2; i++) {
                    outShape[i * 2 + 0] = inShape[i * 2 + 1];
                    outShape[i * 2 + 1] = width - inShape[i * 2 + 0];
                }
                break;
            case 270:
                for (int i = 0; i < length / 2; i++) {
                    outShape[i * 2 + 0] = height - inShape[i * 2 + 1];
                    outShape[i * 2 + 1] = inShape[i * 2 + 0];
                }
                break;
        }
        return outShape;
    }

    public static float angleComputation(float x1, float y1, float x2, float y2, float x3, float y3) {
        float v1_x = x2 - x1;
        float v1_y = y2 - y1;
        float v2_x = x3 - x1;
        float v2_y = y3 - y1;

        return (float) ((v1_x * v2_x + v1_y * v2_y) / (Math.sqrt(v1_x * v1_x + v1_y * v1_y) * Math.sqrt(v1_x * v1_x + v1_y * v1_y)));
    }


    public static float getPointDistanse(PointF pt1, PointF pt2) {
        return (float) Math.sqrt(Math.pow(pt1.x - pt2.x, 2) + Math.pow(pt1.y - pt2.y, 2));
    }

    /**
     * 66点转5点
     * 测试结果: 很正的正脸Tracker66点转5点效果比较好, 现实场景人脸不一定很正, Detector直接取5点效果比较好
     * @param points [x0,y0,x1,y1,...,xn,yn]
     * @return [x0,y0,x1,y1,x2,y2,x3,y3,x4,y4]
     */
    public static float[] cal5Point(float[] points) {
        if (points == null)
            return null;
        float[] face5point = new float[10];

        boolean is106pts = false;

        if(is106pts) {
            face5point[0] = points[74 * 2];
            face5point[1] = points[74 * 2 + 1];
            face5point[2] = points[77 * 2];
            face5point[3] = points[77 * 2 + 1];
            face5point[4] = points[46 * 2];
            face5point[5] = points[46 * 2 + 1];
            face5point[6] = points[84 * 2];
            face5point[7] = points[84 * 2 + 1];
            face5point[8] = points[90 * 2];
            face5point[9] = points[90 * 2 + 1];
        } else {
            face5point[0] = (points[36 * 2] + points[39 * 2]) / 2;
            face5point[1] = (points[36 * 2 + 1] + points[39 * 2 + 1]) / 2;
            face5point[2] = (points[42 * 2] + points[45 * 2]) / 2;
            face5point[3] = (points[42 * 2 + 1] + points[45 * 2 + 1]) / 2;
            face5point[4] = points[30 * 2];
            face5point[5] = points[30 * 2 + 1];
            face5point[6] = points[48 * 2];
            face5point[7] = points[48 * 2 + 1];
            face5point[8] = points[54 * 2];
            face5point[9] = points[54 * 2 + 1];
        }

        return face5point;
    }

    private static float calFlip(float point, float centerPoint) {
        float distance = centerPoint - point;
        return centerPoint + distance;
    }

    // faceInfo面部信息逆时针旋转degrees度，然后判断flipx左右flip
    public static List<FaceInfo> convertFaceInfo(int degrees, List<FaceInfo> faceInfos, int width, int height, boolean flipx) {
        List<FaceInfo> faceInfoList = new ArrayList<>();
        if (faceInfos == null) {
            return faceInfoList;
        } else {
            for (int i = 0; i < faceInfos.size(); ++i) {
                faceInfoList.add(convertFaceInfo(degrees, faceInfos.get(i), width, height, flipx));
            }
            return faceInfoList;
        }
    }

    // faceInfo面部信息逆时针旋转degrees度，然后判断flipx左右flip
    public static FaceInfo convertFaceInfo(int degrees, FaceInfo faceInfo, int width, int height, boolean flipx) {
        if (faceInfo == null) {
            return faceInfo;
        } else {
            boolean isTranspose = (degrees == 90 || degrees == 270);
                FaceInfo faceInfoResult = faceInfo.clone();
                float[] originalShape = faceInfo.getPoints();
                if (originalShape != null) {
                    float[] dstPoints = getRoatePoints(originalShape, degrees, width, height);
                    if(flipx) {
                        dstPoints = getFlipXPoints(dstPoints, isTranspose ? height : width);
                    }
                    faceInfoResult.setPoints(dstPoints);
                }

                RectF rectF;
                if (faceInfo.getRect() == null) {
                    rectF = points2rect(faceInfoResult.getPoints(), faceInfo.getScale());
                    faceInfoResult.setRect(rectF);
                } else {
                    rectF = faceInfo.getRect();
                    float[] rect = new float[]{rectF.left, rectF.top, rectF.right, rectF.bottom};
                    rect = getRoatePoints(rect, degrees, width, height);
                    if(flipx) {
                        rect = getFlipXPoints(rect, isTranspose ? height : width);
                    }
                    faceInfoResult.setRect(new RectF(Math.min(rect[0], rect[2]), Math.min(rect[1], rect[3]), Math.max(rect[0], rect[2]), Math.max(rect[1], rect[3])));
                }

                float[] rotationAngles = faceInfoResult.getRotationAngles();
                if(rotationAngles != null) {
                    if(isTranspose) {
                        float tmp = rotationAngles[0];
                        rotationAngles[0] = rotationAngles[1];
                        rotationAngles[1] = tmp;
                    }

                    if(flipx) {
                        rotationAngles[2] = rotationAngles[2] - (float) Math.toRadians(degrees);
                    } else {
                        rotationAngles[2] = - rotationAngles[2] + (float) Math.toRadians(degrees);
                    }
                }

            return faceInfoResult;
        }
    }

    public static float[] convertPoints(int degrees, float[] originalShape, int width, int height, boolean flipx) {
        float[] dstPoints = null;
        boolean isTranspose = (degrees == 90 || degrees == 270);
        if (originalShape != null) {
            dstPoints = getRoatePoints(originalShape, degrees, width, height);
            if(flipx) {
                dstPoints = getFlipXPoints(dstPoints, isTranspose ? height : width);
            }
        }
        return dstPoints;
    }

    /**
     * 根据图片和66点信息获得人脸框
     * @param width 图片/视频流宽
     * @param height 图片/视频流高
     * @param points 66点
     * @return 获得相对于裁剪
     */
    public static RectF getFaceRect(int width, int height, float[] points){
         return extentRect(boundingSquare(points),width,height,2.2f);
    }

    /**
     * 根据点集确定一个矩形框
     * @param pts 点信息
     * @return 矩形框
     */
    private static RectF boundingSquare(float[]pts) {

        float minx = pts[0], maxx = pts[0];

        float miny = pts[1], maxy = pts[1];

        for (int i = 1; i < pts.length/2; i++) {

            float x = pts[2*i];

            float y = pts[2*i +1];

            if (minx > x) minx = x;

            else if (maxx < x) maxx = x;


            if (miny > y) miny = y;

            else if (maxy < y) maxy = y;

        }
        return new RectF(minx, miny, maxx, maxy);
    }

    /**
     * 根据坤的算法得出新的人脸框
     * @param fr 原始框
     * @param h 原图的高度
     * @param w 原图的宽度
     * @param factor 算法因子, 推荐2.2
     * @return
     */
    private static RectF extentRect(RectF fr, float h, float w, float factor) {
        float eh = fr.height() * factor;
        float ew = fr.width() * factor;
        float ex_1 = fr.left - (ew - fr.width())/ 2;
        float ey_1 = fr.top - (eh - fr.height()) / (1.25f);

        RectF res = new RectF();

        //边界检测
        float ex_2 = Math.min(ex_1 + ew, w - 1), ey_2 =  Math.min(ey_1 + eh, h - 1);
        res.left = Math.max(ex_1, 0);
        res.top = Math.max(ey_1, 0);
        res.right = ex_2;
        res.bottom = ey_2;

        return res;
    }

    /**
     * 点位排序转换, 将格式转换成x0,y0,x1,y1...的格式
     * (x=pt[i] y=pt[i+half])->(x=pt[2*i] y=pt[2*i+1])
     * @param points 顺序:x0,x1,x2...y0,y1,y2...
     * @return points 顺序: x0,y0,x1,y1...
     */
    public static float[] pointsXXYYToPointXYXY(float[] points){
        if (points == null){
            return null;
        }
        float[] newPoints = new float[points.length];
        int half = points.length/2;
        for (int i = 0; i < half; i++) {
            newPoints[2*i] = points[i];
            newPoints[2*i+1] = points[i + half];

        }
      return newPoints;
    }

    /**
     * 点位排序转换
     * (x=pt[i] y=pt[i+half])->(x=pt[2*i] y=pt[2*i+1])
     * @param points 顺序: x0,y0,x1,y1...
     * @return points 顺序:x0,x1,x2...y0,y1,y2...
     */
    public static float[] pointsXYXYToPointXXYY(float[] points){
        if (points == null){
            return null;
        }
        float[] newPoints = new float[points.length];
        int half = points.length/2;
        for (int i = 0; i < half; i++) {
            //push x values
            newPoints[i] = points[2*i];
            // push y values
            newPoints[half+ i] = points[2*i+1];
        }
        return newPoints;
    }

}
