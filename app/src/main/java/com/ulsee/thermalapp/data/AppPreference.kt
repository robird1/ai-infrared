package com.ulsee.thermalapp.data

import android.content.SharedPreferences

class AppPreference (prefs: SharedPreferences) {

    val PERMISSION_ONCE_CREATE_FIRST_DEVICE = "isOnceCreateFirstDevice"
    val SETTINGS_FEVER_NOTIFICATION_ENABLED = "isFeverNotificationEnabled"

    private var sharedPreferences : SharedPreferences

    init {
        sharedPreferences = prefs
    }

    fun isOnceCreateFirstDevice(): Boolean {
        return sharedPreferences.getBoolean(PERMISSION_ONCE_CREATE_FIRST_DEVICE, false)
    }

    fun setOnceCreateFirstDevice() {
        sharedPreferences.edit().putBoolean(PERMISSION_ONCE_CREATE_FIRST_DEVICE, true).apply()
    }

    fun isFeverNotificationEnabled(): Boolean {
        return sharedPreferences.getBoolean(SETTINGS_FEVER_NOTIFICATION_ENABLED, false)
    }

    fun setIsFeverNotificationEnabled(enabled: Boolean) {
        sharedPreferences.edit().putBoolean(SETTINGS_FEVER_NOTIFICATION_ENABLED, enabled).apply()
    }

}