package com.uls.multifacetrackerlib.bean;

import android.graphics.RectF;

import java.util.Arrays;

/**
 * 人脸信息
 * Created by zhangchao on 17-11-13.
 */
public class FaceInfo {
    /**
     * 人脸id
     */
    public int id;

    /**
     * 所属tracker的索引
     */
    public int index;
    /**
     * 关键点[X0,Y0,X1,Y1,...,Xn, Yn]
     */
    public float[] points;
    /**
     * 3d点位
     */
    public float[] point3Ds;
    /**
     * 瞳孔点位
     */
    public float[] pupil;
    /**
     * 人脸位置
     */
    public float[] translationInImages;
    /**
     * 人脸框 rect框基于点位信息做了扩大处理，可能会超出图片坐标的范围（比如有负值）
     */
    public RectF rect;
    /**
     * 3d姿态-欧拉角（弧度）
     */
    public Euler euler;
    /**
     * 人脸大小，相对于输入图片
     */
    public float scale;
    /**
     * 人脸属性
     */
    public FaceAttribute faceAttribute;
    /**
     * 人脸表情
     */
    public FaceEmotion faceEmotion;
    /**
     * 特征值
     */
    public float[] features;

    /**
     * 自定义数据
     */
    public Object tag;

    private boolean isPerfect = true;

    /**
     * 2d点位置信度,0~1
     */
    private float[] confidance;

    /**
     * 人脸3d旋转信息（弧度）
     */
    private float[] rotationAngles;

    public FaceInfo() {

    }

    public boolean isPerfect() {
        return isPerfect;
    }

    public void setPerfect(boolean perfect) {
        isPerfect = perfect;
    }

    public float[] getConfidance() {
        return confidance;
    }

    public void setConfidance(float[] confidance) {
        this.confidance = confidance;
    }

    public float[] getRotationAngles() {
        return rotationAngles;
    }

    public void setRotationAngles(float[] rotationAngles) {
        this.rotationAngles = rotationAngles;
    }

    public float[] getPoints() {
        return points;
    }

    public void setPoints(float[] points) {
        this.points = points;
    }

    public RectF getRect() {
        return rect;
    }

    public void setRect(RectF rect) {
        this.rect = rect;
    }

    public Euler getEuler() {
        return euler;
    }

    public void setEuler(Euler euler) {
        this.euler = euler;
    }

    public float getScale() {
        return scale;
    }

    public void setScale(float scale) {
        this.scale = scale;
    }

    public FaceAttribute getFaceAttribute() {
        return faceAttribute;
    }

    public void setFaceAttribute(FaceAttribute faceAttribute) {
        this.faceAttribute = faceAttribute;
    }

    public FaceEmotion getFaceEmotion() {
        return faceEmotion;
    }

    public void setFaceEmotion(FaceEmotion faceEmotion) {
        this.faceEmotion = faceEmotion;
    }

    public float[] getFeatures() {
        return features;
    }

    public void setFeatures(float[] features) {
        this.features = features;
    }

    public Object getTag() {
        return tag;
    }

    public float[] getPoint3Ds() {
        return point3Ds;
    }

    public void setPoint3Ds(float[] point3Ds) {
        this.point3Ds = point3Ds;
    }

    public float[] getPupil() {
        return pupil;
    }

    public void setPupil(float[] pupil) {
        this.pupil = pupil;
    }

    public float[] getTranslationInImages() {
        return translationInImages;
    }

    public void setTranslationInImages(float[] translationInImages) {
        this.translationInImages = translationInImages;
    }
    public void setTag(Object tag) {
        this.tag = tag;
    }

    public void setIsPerfect(boolean isPerfect) {
        this.isPerfect = isPerfect;
    }

    public FaceInfo clone() {
        FaceInfo faceInfo = new FaceInfo();

        faceInfo.id = id;

        if (points != null) {
            faceInfo.points = Arrays.copyOf(points, points.length);
        }
        if (point3Ds != null) {
            faceInfo.point3Ds = Arrays.copyOf(point3Ds, point3Ds.length);
        }
        if (translationInImages != null) {
            faceInfo.translationInImages = Arrays.copyOf(translationInImages, translationInImages.length);
        }
        if (pupil != null) {
            faceInfo.pupil = Arrays.copyOf(pupil, pupil.length);
        }
        if (rect != null) {
            faceInfo.rect = new RectF(rect.left, rect.top, rect.right, rect.bottom);
        }

        if (euler != null) {
            faceInfo.euler = euler.clone();
        }

        faceInfo.isPerfect = isPerfect;

        faceInfo.scale = scale;
        if (faceAttribute != null) {
            faceInfo.faceAttribute = faceAttribute.clone();
        }

        if (features != null) {
            faceInfo.features = Arrays.copyOf(features, features.length);
        }

        faceInfo.tag = tag;

        if (confidance != null) {
            faceInfo.confidance = Arrays.copyOf(confidance, confidance.length);
        }

        if (rotationAngles != null) {
            faceInfo.rotationAngles = Arrays.copyOf(rotationAngles, rotationAngles.length);
        }

        return faceInfo;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();

        sb.append("id:" + id + "\n");

        if (points != null) {
            sb.append("points:" + (points.length / 2) + "\n");
            for (int i = 0; i < points.length / 2; i++) {
                sb.append("" + points[i * 2] + "," + points[i * 2 + 1] + "\n");
            }
        } else {
            sb.append("points:null\n");
        }
        if (point3Ds != null) {
            sb.append("point3Ds:" + (point3Ds.length / 2) + "\n");
            for (int i = 0; i < point3Ds.length / 2; i++) {
                sb.append("" + point3Ds[i * 2] + "," + point3Ds[i * 2 + 1] + "\n");
            }
        } else {
            sb.append("point3Ds:null\n");
        }

        if (translationInImages != null) {
            sb.append("translationInImages:" + (translationInImages.length / 2) + "\n");
            for (int i = 0; i < translationInImages.length / 2; i++) {
                sb.append("" + translationInImages[i * 2] + "," + translationInImages[i * 2 + 1] + "\n");
            }
        } else {
            sb.append("translationInImages:null\n");
        }
        if (pupil != null) {
            sb.append("pupil:" + (pupil.length / 2) + "\n");
            for (int i = 0; i < pupil.length / 2; i++) {
                sb.append("" + pupil[i * 2] + "," + pupil[i * 2 + 1] + "\n");
            }
        } else {
            sb.append("pupil:null\n");
        }
        if (rect != null) {
            sb.append("rect:(" + rect.left + "," + rect.top + "," + rect.right + "," + rect.bottom + ")\n");
        } else {
            sb.append("rect:null\n");
        }

        if (euler != null) {
            sb.append("euler:\n");
            sb.append("roll=" + euler.roll + "\n");
            sb.append("pitch=" + euler.pitch + "\n");
            sb.append("yaw=" + euler.yaw + "\n");
        } else {
            sb.append("euler:null\n");
        }
        sb.append("scale:" + scale + "\n");
        if (faceAttribute != null) {
            sb.append("faceAttribute:\n");
            sb.append("age=" + faceAttribute.age + "\n");
            sb.append("gender=" + faceAttribute.gender + "\n");
            sb.append("emotion=" + faceAttribute.emotion + "\n");
        } else {
            sb.append("faceAttribute:null\n");
        }
        if (features != null) {
            sb.append("features:" + (features.length) + "\n");
            for (int i = 0; i < features.length; i++) {
                sb.append("" + features[i] + "\n");
            }
        } else {
            sb.append("features:null\n");
        }
        sb.append("isPerfect=" + isPerfect + "\n");

        return sb.toString();
    }
}
