package com.uls.multifacetrackerlib.utils;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

/**
 * Created by ian on 2017/1/12.
 */

public class DeviceRotationDetector implements SensorEventListener {
    float x, y, z;
    private SensorManager mSensorManager;
    private Sensor mSensor;

    public DeviceRotationDetector(Context context) {
        mSensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
        boolean registered = mSensorManager.registerListener(this, mSensor, SensorManager.SENSOR_DELAY_NORMAL);
        L.d("GRAVITY", "gravity sensor registered: " + registered);
        if (!registered) {
            mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
            registered = mSensorManager.registerListener(this, mSensor, SensorManager.SENSOR_DELAY_NORMAL);
            L.d("GRAVITY", "accelerometer registered: " + registered);
        }
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
//        L.d("GRAVITY", "X:"+sensorEvent.values[0]+
//                ", Y:"+sensorEvent.values[1]+
//                ", Z:"+sensorEvent.values[2]);
        x = sensorEvent.values[0];
        y = sensorEvent.values[1];
        z = sensorEvent.values[2];
//        L.d("GRAVITY", ""+getRotationDegree());
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }

    public int getRotationDegree() {
        if (Math.abs(y) >= Math.abs(x)) {
            if (y >= 0) {
                return 0;
            } else {
                return 180;
            }
        } else {
            if (x >= 0) {
                return 90;
            } else {
                return 270;
            }
        }
    }
}
