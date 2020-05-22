package com.ulsee.thermalapp.data.model

import io.realm.RealmObject

open class Device : RealmObject() {

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
}
