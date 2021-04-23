package com.ulsee.thermalapp.ui.people

import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.RealmDevice
import io.realm.Realm
import io.realm.kotlin.where
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext

class DeviceSyncRepository {
    suspend fun loadDevices(): List<Device> = withContext(Dispatchers.IO) {
        val realm = Realm.getDefaultInstance()
        val results = realm.where<RealmDevice>().findAll()
        val deviceList = ArrayList<Device>()
        for (realmDevice in results) {
            val device = Device.clone(realmDevice)
            deviceList.add(device)
        }
        realm.close()
        return@withContext deviceList
//        return@withContext Service.shared.getDeviceList()
    }
}