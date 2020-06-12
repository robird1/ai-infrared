package com.ulsee.thermalapp.data

import android.util.Log
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.services.*
import io.realm.Realm
import io.realm.kotlin.where

class Service {
    companion object {
        val shared = Service()
    }
    
    interface DeviceSearchedListener {
        fun onNewDevice(device: Device)
    }

    var deviceManagerList : ArrayList<DeviceManager> = ArrayList<DeviceManager>()
    val udpBroadcastService = UDPBroadcastService()
    var mDeviceSearchedListener : DeviceSearchedListener? = null
    val isScanning: Boolean
        get() = mDeviceSearchedListener != null

    init {
        udpBroadcastService.subscribeSearchedDevice().subscribe{
            // if device ip changed
            for (deviceManager in deviceManagerList) {
                if (!deviceManager.tcpClient.isConnected() && !deviceManager.device.getIP().equals(it.getIP())) {
                    Log.i(javaClass.name, "found device ip changed, old ip: "+deviceManager.device.getIP()+", new ip: "+it.getIP())
                    deviceManager.device.setIP(it.getIP())
                    deviceManager.resetIP(it.getIP())
                }
            }
            // if scanning
            mDeviceSearchedListener?.onNewDevice(it)
        }
        getDeviceList()
        keepSearchingDevice()
    }

    // device
    fun getDeviceList():List<Device> {
        val realm = Realm.getDefaultInstance()
        val results = realm.where<Device>().findAll()

        for (device in results) {
            var isDeviceManagerExists = false
            for (deviceManager in deviceManagerList) {
                if (deviceManager.device.getID().equals(device.getID())) {
                    Log.e(javaClass.name, "Error: got duplicated device: "+device.getID()+", "+device.getName()+", "+device.getIP())
                    isDeviceManagerExists = true
                    continue
                }
            }
            if (!isDeviceManagerExists) {
                deviceManagerList.add(DeviceManager(device))
            }
        }
        return results
    }

    fun keepSearchingDevice () {
        var isAnyDeviceNotConnected : Boolean
        Thread(Runnable {
            while (true) {
                try {
                    isAnyDeviceNotConnected = deviceManagerList.indexOfFirst { !it.tcpClient.isConnected() } >= 0
                    udpBroadcastService.shouldBroadcasting = isAnyDeviceNotConnected || isScanning
                    Thread.sleep(1000)
                } catch(e: Exception) {
                    Log.e(javaClass.name, "Error: Service keepSearchingDevice:")
                    e.printStackTrace()
                }
            }
        }).start()
    }

    fun getManagerOfDevice(device: Device) : DeviceManager? {
        return getManagerOfDeviceID(device.getID())
    }

    fun getManagerOfDeviceID(deviceID: String) : DeviceManager? {
        var result: DeviceManager? = null
        for (deviceManager in deviceManagerList) {
            if (deviceManager.device.getID().equals(deviceID)) {
                result = deviceManager
                break
            }
        }
        return result
    }
}