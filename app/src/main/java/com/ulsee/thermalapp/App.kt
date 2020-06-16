package com.ulsee.thermalapp

import android.app.Application
import com.ulsee.thermalapp.data.Service
import io.realm.Realm
import io.realm.RealmConfiguration

class App : Application() {

    override fun onCreate() {
        super.onCreate()
        Realm.init(this)
        val config = RealmConfiguration.Builder()
            .name("appv1.realm")
            .schemaVersion(1) // Must be bumped when the schema changes
//            .migration { realm, oldVersion, newVersion ->
//                if (oldVersion == 0L) {
//                    val schema = realm.schema
//                    schema.create("RealmDevice")
//                        ?.addField("mID", String.javaClass)
//                        ?.addField("mIP", String.javaClass)
//                        ?.addField("mName", String.javaClass)
//                        ?.addField("mCreatedAt", Long.javaClass)
//                }
//            } // Migration to run
            .build()
        Realm.setDefaultConfiguration(config)
        Service.shared.udpBroadcastService.initialize(applicationContext)
    }
}