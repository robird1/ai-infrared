package com.ulsee.thermalapp.data.model

open class Settings {
    var TemperatureUnit = 0
    var AlarmThreshold : Double = 0.0
    var Deviation : Double = 0.0
    var IsFirstSetting: Boolean = false
    var ID = ""
    var FaceRecognition: Boolean = false
    var FlipImage: Boolean = false
    var OnlyROI: Boolean = false
}