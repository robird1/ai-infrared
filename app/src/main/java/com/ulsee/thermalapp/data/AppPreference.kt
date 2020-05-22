package com.ulsee.thermalapp.data

import android.content.SharedPreferences

class AppPreference (prefs: SharedPreferences) {

    val PERMISSION_ONCE_CREATE_FIRST_DEVICE = "isOnceCreateFirstDevice"

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

}