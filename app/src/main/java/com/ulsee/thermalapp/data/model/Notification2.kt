package com.ulsee.thermalapp.data.model

import java.io.Serializable
import java.lang.Exception
import java.text.SimpleDateFormat
import java.util.*

open class Notification2 : Serializable {
    val ID : String = "-1"
    val Name : String = "" // alarm people name
    val displayName : String
        get() {
            return if (!IsUnkown) "$Name " else ""
        }
    val IsUnkown : Boolean = false

    var TempUnit  : Int = 0
    val tempratureUnitString : String
        get() {
            return if (TempUnit == 0) "°C" else "°F"
        }

    var TempValue  : Float = 0F

    var IsMask  : Boolean = true
    val isMaskString : String
        get() {
            return if (IsMask) "mask" else "no_mask"
        }

    var Time  : String = ""
    private var mDate : Date? = null
    val timeDate : Date
        get() {
            if (mDate!=null) return mDate!!
//            val pattern = "yyyy-MM-dd HH:mm:ss"
            val pattern = "EEE MMM dd HH:mm:ss YYYY"
            val simpleDateFormat = SimpleDateFormat(pattern)
            var dateString: Date
            dateString = try {
                simpleDateFormat.parse(Time)
            } catch (e: Exception) {
//                simpleDateFormat.parse("Mon Jan 01 00:00:00 1970")
                Date(1970,1,1)
            }
            return dateString
        }

    var Data  : String = ""
}