package com.ulsee.thermalapp.data.model

import io.realm.RealmObject

open class Device {

     private var mID: String = ""
     open fun setID(value: String) {
          mID = value
     }
     open fun getID(): String {
          return mID
     }

     private var mIP: String = ""
     open fun setIP(value: String) {
          mIP = value
     }
     open fun getIP(): String {
          return mIP
     }

     private var mName: String = ""
     open fun setName(value: String) {
          mName = value
     }
     open fun getName(): String {
          return mName
     }

     private var mCreatedAt: Long = 0
     open fun setCreatedAt(value: Long) {
          mCreatedAt = value
     }
     open fun getCreatedAt(): Long {
          return mCreatedAt
     }

     public companion object {
          fun clone (realmDevice: RealmDevice) : Device {
               val device = Device()
               device.setID(realmDevice.getID())
               device.setIP(realmDevice.getIP())
               device.setName(realmDevice.getName())
               device.setCreatedAt(realmDevice.getCreatedAt())
               return device
          }
     }

}
