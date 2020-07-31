package com.ulsee.thermalapp.data

import android.util.Log
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.RealmDevice
import com.ulsee.thermalapp.data.services.*
import io.realm.Realm
import io.realm.kotlin.where

val TAG = "Service"

class Service {
    companion object {
        val shared = Service()
    }
    
    interface DeviceSearchedListener {
        fun onNewDevice(device: Device)
    }

    var deviceManagerList : ArrayList<DeviceManager> = ArrayList<DeviceManager>()
    var mScannedDeviceList = ArrayList<Device>() // IP, ID, Timestamp
    val udpBroadcastService = UDPBroadcastService()
    var mDeviceSearchedListener : DeviceSearchedListener? = null
    val isScanning: Boolean
        get() = mDeviceSearchedListener != null
    var isStarterActivity: Boolean = false

    // tutorial
    fun requestTutorial(deviceID: String) {
        tutorialDeviceID = deviceID
    }
    var tutorialDeviceID : String? = null
    var justJoinedDeviceIDList = ArrayList<String>()

    init {
        Log.d("Service", "[Enter] init")
        udpBroadcastService.subscribeSearchedDevice().subscribe{
            // if device ip changed
            for (deviceManager in deviceManagerList) {
                if (deviceManager.device.getID().equals(it.getID()) && !deviceManager.device.getIP().equals(it.getIP())) {
                    Log.i(javaClass.name, "found device ip changed, old ip: "+deviceManager.device.getIP()+", new ip: "+it.getIP())
                    if (!deviceManager.tcpClient.isConnected()){
                        Thread(Runnable {
                            try {
                                deviceManager.resetIP(it.getIP())
                                Log.i(javaClass.name, "reset to new IP!")
                            } catch (e: java.lang.Exception) {
                                Log.i(javaClass.name, "failed to reset to new IP:")
                                e.printStackTrace()
                            }
                        }).start()
                    } else {
                        Log.e(javaClass.name, "found device ip changed, but still connected... old ip: "+deviceManager.device.getIP()+", new ip: "+it.getIP())
                    }
                }
            }
            // if scanning
            mDeviceSearchedListener?.onNewDevice(it)
            
            updateScannedDeviceList(it)
        }
        getDeviceList()
        keepSearchingDevice()
    }

    private fun updateScannedDeviceList(device: Device) {
        if (!isDeviceDuplicated(device)) {
            device.setCreatedAt(System.currentTimeMillis())
            mScannedDeviceList.add(device)
        }
    }

    private fun isDeviceDuplicated(device: Device): Boolean {
        for (d in mScannedDeviceList) {
            if (d.getID().equals(device.getID()) && d.getIP().equals(device.getIP())) {
                return true
            }
        }
        return false
    }

    // device
    fun getDeviceList():List<Device> {
        val realm = Realm.getDefaultInstance()
        val results = realm.where<RealmDevice>().findAll()
        val deviceList = ArrayList<Device>()

        for (realmDevice in results) {
            val device = Device.clone(realmDevice)
            deviceList.add(device)

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

        // remove devicemanager
        var i = deviceManagerList.size-1
        while(i >= 0) {
            val deviceManager = deviceManagerList[i]
            var exists = false
            for(realmDevice in results) {
                if (realmDevice.getID() == deviceManager.device.getID()) {
                    exists = true
                    break
                }
            }
            if (!exists) {
                deviceManagerList.removeAt(i)
            }
            --i
        }
        return deviceList
    }

    fun keepSearchingDevice () {
        var isAnyDeviceNotConnected: Boolean
        Thread(Runnable {
            while (true) {
                try {
                    isAnyDeviceNotConnected =
                        deviceManagerList.indexOfFirst { !it.tcpClient.isConnected() } >= 0
                    udpBroadcastService.shouldBroadcasting = isAnyDeviceNotConnected || isScanning || isStarterActivity

                    Log.d(TAG, "udpBroadcastService.shouldBroadcasting: "+ udpBroadcastService.shouldBroadcasting)

                    Thread.sleep(1000)
                } catch (e: Exception) {
                    Log.e(javaClass.name, "Error: Service keepSearchingDevice:")
                    e.printStackTrace()
                }
            }
        }).start()
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