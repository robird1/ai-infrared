package com.ulsee.thermalapp.data.model

import java.io.Serializable
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
//            if (mDate!=null) return mDate!!
            val pattern = "yyyy-MM-dd HH:mm:ss"
            val simpleDateFormat = SimpleDateFormat(pattern)
            return try {
                simpleDateFormat.parse(Time)
            } catch (e: Exception) {
                simpleDateFormat.parse("1970-01-01 00:00:00")
            }
        }

    var Data  : String = ""
}