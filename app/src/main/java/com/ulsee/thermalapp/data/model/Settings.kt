package com.ulsee.thermalapp.data.model

data class Settings(
    var TemperatureUnit : String,
    var AlarmThreshold : Double,
    var TemperatureOffset : Double,
    val IsFlipHorizontal : Boolean
)