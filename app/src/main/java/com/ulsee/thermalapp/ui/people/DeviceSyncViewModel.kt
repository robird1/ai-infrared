package com.ulsee.thermalapp.ui.people

import android.util.Log
import androidx.lifecycle.*
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import kotlinx.coroutines.*

private val TAG = DeviceSyncViewModel::class.java.simpleName

class DeviceSyncViewModel(private val repo: DeviceSyncRepository) : ViewModel() {
    private var _onlineList = MutableLiveData<ArrayList<Device>>()
    val onlineList: LiveData<ArrayList<Device>>
        get() = _onlineList
    private var _syncResult = MutableLiveData<Boolean>()
    val syncResult : LiveData<Boolean>
        get() = _syncResult
    var syncFailedList = ArrayList<String>()

    init {
        getOnlineDevices()
    }

    private fun getOnlineDevices() {
        viewModelScope.launch {
            val deferredList = repo.loadDevices().map { device ->
                async(Dispatchers.IO) {
                    try {
                        Log.d(TAG, "[Before] repo.requestDeviceInfo() ip: ${device.getIP()}")
                        val deviceManager = Service.shared.getManagerOfDeviceID(device.getID())
                        if (deviceManager?.tcpClient?.isConnected() == true) return@async device
                        else return@async null

                    } catch (e: Exception) {
                        Log.d(TAG, "e.message: ${e.message}")
                        return@async null
                    }
                }
            }
            _onlineList.value = obtainOnlineList(deferredList)
        }
    }

    private suspend fun obtainOnlineList(deferredList: List<Deferred<Device?>>): ArrayList<Device> {
        val list = ArrayList<Device>()
        for (i in deferredList) {
            val device = i.await()
            device?.let { list.add(it) }
        }
        return list
    }

    fun synFace(selectedDevices: ArrayList<Device>) {
        viewModelScope.launch {
            val deferredList = selectedDevices.map { device ->
                async(Dispatchers.IO) {
                    val resultCode = requestAddPerson(device)
                    Log.d(TAG, "[Enter] device: ${device.getName()} resultCode: $resultCode")
                    Pair(device.getName(), resultCode)
                }
            }
            val resultList = deferredList.awaitAll()

            obtainSyncFailedList(resultList)
            _syncResult.value = true
        }
    }

    private fun obtainSyncFailedList(resultList: List<Pair<String, Int>>) {
        val list = ArrayList<String>()
        for (pair in resultList) {
            if (pair.second == -1)
                list.add(pair.first)
        }
        syncFailedList = list
    }

    private fun requestAddPerson(device: Device): Int {
        Log.d(TAG, "[Before] repository.requestAddPerson")
        val people = AttributeType.getAttributeData()
        val deviceManager = Service.shared.getManagerOfDevice(device)
        return if (deviceManager != null) {
            try {
                deviceManager.createProfile(people)
                0
            } catch (e: Exception) {
                Log.d(TAG, "[Exception] ${e.message}")
                -1
            }
        } else {
            Log.d(TAG, "[Exception] deviceManager == null")
            -1
        }
    }
}

class DeviceSyncFactory(private val repository: DeviceSyncRepository) : ViewModelProvider.Factory {
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(DeviceSyncViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return DeviceSyncViewModel(repository) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}
