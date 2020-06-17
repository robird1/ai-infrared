package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.Settings

class UpdateSettings(settings: Settings) : Settings() {
    val Action = 0
    init {
        TempUnit = settings.TempUnit
        TempAlarmValue = settings.TempAlarmValue
        Deviation = settings.Deviation

        IsFR = settings.IsFR
        IsFlip = settings.IsFlip
        IsOnlyROI = settings.IsOnlyROI
        AutoSaveDir = settings.AutoSaveDir
        IsAutoSave = settings.IsAutoSave
        IsDeveloper = settings.IsDeveloper
        IsFeverOnly = settings.IsFeverOnly
        LastModifiedDate = settings.LastModifiedDate
        TempMin = settings.TempMin
        ThermalTime = settings.ThermalTime
        Version = settings.Version
        XRatio = settings.XRatio
        XShift = settings.XShift
        YRatio = settings.YRatio
        YShift = settings.YShift
    }
}