package com.uls.multifacetrackerlib.bean;

import android.os.Parcel;
import android.os.Parcelable;

/**
 * 3d姿态-欧拉角
 * Created by zhangchao on 17-11-13.
 */


public class Euler implements Parcelable {
    public static final Creator<Euler> CREATOR = new Creator<Euler>() {
        @Override
        public Euler createFromParcel(Parcel in) {
            return new Euler(in);
        }

        @Override
        public Euler[] newArray(int size) {
            return new Euler[size];
        }
    };

    /**
     * 取的是弧度 绕z轴旋转
     */
    public float roll;

    /**
     * 取的是弧度 绕x轴旋转
     */
    public float pitch;

    /**
     * 取的是弧度 绕y轴旋转
     */
    public float yaw;

    public Euler() {

    }

    protected Euler(Parcel in) {
        roll = in.readFloat();
        pitch = in.readFloat();
        yaw = in.readFloat();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int i) {
        parcel.writeFloat(roll);
        parcel.writeFloat(pitch);
        parcel.writeFloat(yaw);
    }

    public Euler clone() {
        Euler euler = new Euler();
        euler.roll = roll;
        euler.pitch = pitch;
        euler.yaw = yaw;
        return euler;
    }
}
