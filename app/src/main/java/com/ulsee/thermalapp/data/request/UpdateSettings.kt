package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.Settings

class UpdateSettings(settings: Settings) : Settings() {
    val Action = 0
    init {
        TempUnit = settings.TempUnit
        TempAlarmValue = settings.TempAlarmValue
        Deviation = settings.Deviation
    }
}