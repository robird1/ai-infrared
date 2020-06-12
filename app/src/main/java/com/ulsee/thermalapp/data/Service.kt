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

//    var settings = SettingsServiceHTTP()
//    var people : IPeopleService = PeopleServiceHTTP()
    var deviceManagerList : ArrayList<DeviceManager> = ArrayList<DeviceManager>()

    // tutorial
    var tutorialDeviceID : String? = "-123"

    init {
        getDeviceList()
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

    fun getFirstConnectedDeviceManager(): DeviceManager? {
        var result : DeviceManager? = null
        for (deviceManager in Service.shared.deviceManagerList) {
            if (deviceManager.tcpClient.isConnected() && deviceManager.settings != null) {
                result = deviceManager
                break
            }
        }
        return result
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