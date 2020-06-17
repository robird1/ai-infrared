package com.ulsee.thermalapp.data.model

open class Settings {
    var TempUnit = 0
    var TempAlarmValue : Double = 0.0
    var Deviation : Double = 0.0
    var IsFirstActivate: Boolean = false
    var ID = ""
    var IsFR: Boolean = false
    var IsFlip: Boolean = false
    var IsOnlyROI: Boolean = false

    var AutoSaveDir = ""
    var IsAutoSave = false
    var IsDeveloper = false
    var IsFeverOnly = false
    var LastModifiedDate = "2020/06/12" //
    var TempMin = 0.0
    var ThermalTime = 0
    var Version = ""
    var XRatio = 0.0
    var XShift = 0.0
    var YRatio = 0.0
    var YShift = 0.0
}