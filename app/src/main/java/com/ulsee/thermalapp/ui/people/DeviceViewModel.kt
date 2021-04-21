package com.ulsee.thermalapp.ui.people

import android.util.Log
import androidx.lifecycle.*
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import kotlinx.coroutines.*

private val TAG = DeviceViewModel::class.java.simpleName
const val POLLING_INTERVAL = 3000L

open class DeviceViewModel(private val repo: DeviceSyncRepository): ViewModel() {
    private var _deviceList = MutableLiveData<List<Device>>()
    val deviceList : LiveData<List<Device>>
        get() = _deviceList
    private var _deviceStatus = MutableLiveData<DeviceStatus>()
    val deviceStatus: LiveData<DeviceStatus>
        get() = _deviceStatus
    private val jobList = ArrayList<Job>()


    init {
        loadDevices()
    }

    private fun loadDevices() {
        Log.d(TAG, "[Enter] loadDevices()")
        cancelAllConnectionJobs()
        viewModelScope.launch {
            _deviceList.value = repo.loadDevices()
        }
    }

    fun getConnectionStatus(device: Device) {
        val job = viewModelScope.launch  {
            while(true) {
                try {
                    val deviceManager = Service.shared.getManagerOfDevice(device)
                    val isOnline = deviceManager!!.tcpClient.isConnected()
                    if (isOnline) {
                        _deviceStatus.value = DeviceStatus(device.getID(), true)
//                        Log.d(TAG, "deviceId: ${device.getID()} isConnected: true")
                    } else {
                        _deviceStatus.value = DeviceStatus(device.getID(), false)
//                        Log.d(TAG, "deviceId: ${device.getID()} isConnected: false")
                    }

                } catch (e: Exception) {
                    _deviceStatus.value = DeviceStatus(device.getID(), false)
                }

                delay(POLLING_INTERVAL)
            }
        }
        jobList.add(job)
    }

    fun cancelAllConnectionJobs() {
        for (j in jobList) {
            j.cancel()
        }
        jobList.clear()
    }

    data class DeviceStatus(val deviceID: String, val isConnected: Boolean)

}

class DeviceFactory(private val repository: DeviceSyncRepository) : ViewModelProvider.Factory {
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(DeviceViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return DeviceViewModel(repository) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}
