package com.ulsee.thermalapp.data.model

data class Settings(
    val TemperatureUnit : String,
    val AlarmThreshold : Double,
    val TemperatureOffset : Double,
    val IsFlipHorizontal : Boolean
)