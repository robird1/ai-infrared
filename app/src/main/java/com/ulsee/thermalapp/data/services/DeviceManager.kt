package com.ulsee.thermalapp.data.services

import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.People
import com.ulsee.thermalapp.data.model.Settings
import java.lang.StringBuilder

class DeviceManager(device: Device) {
    enum class Status {
        connecting, connected
    }

    interface OnStatusChangedListener {
        fun onChanged(status: Status)
    }
    var status = Status.connecting
    val device = device
    var settings : Settings? = null
    var mOnStatusChangedListener: OnStatusChangedListener? = null
    val tcpClient = TCPClient(device.getIP(), 13888)

    init {
        // 1. 建立連線
        Thread(Runnable {
            tcpClient.connect()
        }).start()

        // 2. 取得設定
        val gson = Gson()
        val stringBuilder = StringBuilder();
        tcpClient.setOnReceivedDataListener(object: TCPClient.OnReceivedDataListener{
            override fun onData(data: CharArray, size: Int) {
                stringBuilder.append(data, 0, size)

                val responseString = stringBuilder.toString()
                val itemType = object : TypeToken<Settings>() {}.type
                settings = gson.fromJson<Settings>(responseString, itemType)
                status = Status.connected
                mOnStatusChangedListener?.onChanged(status)
                tcpClient.setOnReceivedDataListener(null)
            }
        })

        // 3. 斷線自動重連
        // todo 斷線自動重連
    }

    fun setOnStatusChangedListener(listener: OnStatusChangedListener?) {
        mOnStatusChangedListener = listener
    }
}