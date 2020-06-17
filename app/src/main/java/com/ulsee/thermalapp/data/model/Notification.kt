package com.ulsee.thermalapp.data.model

import android.util.Log
import java.io.Serializable
import java.text.SimpleDateFormat
import java.util.*

open class Notification : Serializable {
    val Name : String = "" // alarm people name
    var Image = ""

    val peopleName : String
    get() {
        var arr = Name.split("_")
        if (arr.size == 6) {
            return arr[5]
        }
        return ""
    }

    val createdAt : Date
    get() {
        var arr = Name.split("_")
        if (arr.size == 6) {
            val pattern = "yyyyMMddHHmmss"
            val simpleDateFormat = SimpleDateFormat(pattern)
            return simpleDateFormat.parse(arr[0]+arr[1])
        }
        return Date()
    }

    val tempratureUnit : String
        get() {
        var arr = Name.split("_")
        if (arr.size == 6) {
            return arr[2]
        }
        return ""
    }
    val temprature : String
        get() {
        var arr = Name.split("_")
        if (arr.size == 6) {
            return arr[3].substring(0,2)+"."+arr[3].substring(2)
        }
        return ""
    }

    val hasMask  : Boolean
        get() {
            var arr = Name.split("_")
            if (arr.size == 6) {
                return arr[4] == "mask"
            }
            return false
        }

//    var peopleName = ""
//    var createdAt = Date()
//    var tempratureUnit = ""
//    var temprature = ""
//    var hasMask = false
//
//    init {
//        var arr = Name.split("_")
//        if (arr.size != 6) {
//            Log.w(javaClass.name, "notification name format error")
//        } else {
//            val pattern = "yyyyMMddHHmmss"
//            val simpleDateFormat = SimpleDateFormat(pattern)
//            createdAt = simpleDateFormat.parse(arr[0]+arr[1])
//
//            tempratureUnit = arr[2]
//            temprature = arr[3].substring(0,2)+"."+arr[3].substring(2)
//            hasMask = arr[4] == "mask"
//            peopleName = arr[5]
//        }
//    }
}