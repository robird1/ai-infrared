package com.ulsee.thermalapp.data.model

open class Settings {
    var TempUnit = 0
    var TempAlarmValue : Double = 0.0
    var Deviation : Double = 0.0
    var IsFirstActivate: Boolean = false           // always false
    var ID = ""
    var IsFR: Boolean = false
    var IsFlip: Boolean = false
    var IsOnlyROI: Boolean = false
    var IsFRVisible: Boolean = true
    var AutoSaveDir = ""
    var IsAutoSave = false
    var IsDeveloper = false
    var IsFeverOnly = false
    var LastModifiedDate = "2020/06/12" //
    var TempMin = 0.0
    var ThermalTime = 0

    // 以下參數要注意...
    var Version = ""
    var XRatio = 0.0
    var XShift = 0
    var YRatio = 0.0
    var YShift = 0
}