package com.ulsee.thermalapp.data.model

import android.util.Log
import java.io.Serializable
import java.text.SimpleDateFormat
import java.util.*

open class Notification : Serializable {
    val Name : String = "" // alarm people name
    var Image = ""

    var mPeopleName : String? = null
    val peopleName  : String
        get() {
            if (mPeopleName == null) parseFileName()
            return mPeopleName!!
        }


    var mCreatedAt : Date? = null
    val createdAt : Date
    get() {
        if (mCreatedAt == null) parseFileName()
        return mCreatedAt!!
    }

    var mTempratureUnitString : String? = null
    val tempratureUnitString  : String
        get() {
            if (mTempratureUnitString == null) parseFileName()
            return mTempratureUnitString!!
        }

    val tempratureUnit : String
        get() {
            return if (tempratureUnitString == "Celsius") "°C" else "°F"
        }

    var mTemprature : String? = null
    val temprature  : String
        get() {
            if (mTemprature == null) parseFileName()
            return mTemprature!!
        }

    var mHasMask : Boolean? = null
    val hasMask  : Boolean
        get() {
            if (mHasMask == null) parseFileName()
            return mHasMask!!
        }

    fun parseFileName() {
        val arr= Name.split("_")
        if (arr.size != 6) {
            Log.w(javaClass.name, "notification name format error")
            mCreatedAt = Date()
            mTempratureUnitString = ""
            mTemprature = ""
            mHasMask = false
            mPeopleName = ""
        } else {
            val pattern = "yyyyMMddHHmmss"
            val simpleDateFormat = SimpleDateFormat(pattern)
            mCreatedAt = simpleDateFormat.parse(arr[0]+arr[1])

            mTempratureUnitString = arr[2]
            mTemprature = arr[3].substring(0,2)+"."+arr[3].substring(2)
            mHasMask = arr[4] == "mask"

            mPeopleName = arr[5]
            val arr2 = mPeopleName!!.split(".")
            if (arr2.size > 1) {
                mPeopleName = mPeopleName!!.substring(0, mPeopleName!!.length-1-arr2[arr2.size-1].length)
            }
        }
    }
}