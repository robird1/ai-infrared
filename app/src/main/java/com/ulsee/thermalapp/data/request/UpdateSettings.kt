package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.Settings

class UpdateSettings(settings: Settings) : Settings() {
    val Action = 0
    init {
        TemperatureUnit = settings.TemperatureUnit
        AlarmThreshold = settings.AlarmThreshold
        Deviation = settings.Deviation
    }
}