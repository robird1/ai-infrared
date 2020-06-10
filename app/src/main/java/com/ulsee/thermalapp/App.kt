package com.ulsee.thermalapp

import android.app.Application
import com.ulsee.thermalapp.data.Service
import io.realm.Realm

class App : Application() {

    override fun onCreate() {
        super.onCreate()
        Realm.init(this)
    }
}