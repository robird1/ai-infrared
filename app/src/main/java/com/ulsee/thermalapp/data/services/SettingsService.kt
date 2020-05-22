package com.ulsee.thermalapp.data.services

import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.api.SettingsAPI
import io.reactivex.Observable
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import retrofit2.Retrofit
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory
import retrofit2.converter.gson.GsonConverterFactory

class SettingsService {

    private val client = Retrofit.Builder()
        .baseUrl("http://192.168.11.155:8080")
        .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
        .addConverterFactory(GsonConverterFactory.create())
        .build().create(SettingsAPI::class.java)

    fun get() : Observable<Settings> {
        return client.get()
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    fun update(settings: Settings) : Observable<Void> {
        return client.update(settings)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    fun updateTemperatureUnit(value: String) : Observable<Void> {
        var settings = Settings(value, 0.0, 0.0, false)
        return client.updateTemperatureUnit(settings)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    fun updateAlarmThreshold(value: Double) : Observable<Void> {
        var settings = Settings("", value, 0.0, false)
        return client.updateAlarmThreshold(settings)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    fun updateTemperatureOffset(value: Double) : Observable<Void> {
        var settings = Settings("", 0.0, value, false)
        return client.updateTemperatureOffset(settings)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    fun updateFlipHorizontal(value: Boolean) : Observable<Void> {
        var settings = Settings("", 0.0, 0.0, value)
        return client.updateFlipHorizontal(settings)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }
}