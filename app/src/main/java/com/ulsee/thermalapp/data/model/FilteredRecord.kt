package com.ulsee.thermalapp.data.model

import android.os.Parcelable
import com.ulsee.thermalapp.ui.notification.RecordFilterActivity.FaceMask
import com.ulsee.thermalapp.ui.notification.RecordFilterActivity.TemperatureUnit
import kotlinx.android.parcel.Parcelize

@Parcelize
data class FilteredRecord (
    val Action: Int = 21,
    var Name : String = "",
    var DateTimeStart : String = "",   // yyyy-MM-dd HH:mm:ss
    var DateTimeEnd : String = "",
    var TempUnit : Int = TemperatureUnit.CELSIUS.apiValue,       // 0-> celsius, 1-> Fahrenheit
    var TempMin: Float = -1F,
    var TempMax: Float  = -1F,
    var Mask: Int  = FaceMask.ALL.apiValue        // 0-> No, 1-> Yes, others -> all
) : Parcelable