package com.ulsee.thermalapp.data.services

import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.api.SettingsAPI
import com.ulsee.thermalapp.data.request.UpdateCalibration
import com.ulsee.thermalapp.data.response.TwoPicture
import com.ulsee.thermalapp.data.response.VideoFrame
import io.reactivex.Completable
import io.reactivex.Observable
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import retrofit2.Retrofit
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory
import retrofit2.converter.gson.GsonConverterFactory

class SettingsServiceHTTP : ISettingsService {

    private val client = Retrofit.Builder()
        .baseUrl("http://192.168.11.155:8080")
        .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
        .addConverterFactory(GsonConverterFactory.create())
        .build().create(SettingsAPI::class.java)

    override fun get() : Observable<Settings> {
        return client.get()
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    override fun update(settings: Settings) : Completable {
        return client.update(settings)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    override fun getTwoPicture(): Observable<TwoPicture> {
        TODO("Not yet implemented")
    }

    override fun calibration(calibration: UpdateCalibration): Completable {
        TODO("Not yet implemented")
    }

    override fun openRGBStream(): Observable<VideoFrame> {
        TODO("Not yet implemented")
    }

    override fun closeRGBStream(): Completable {
        TODO("Not yet implemented")
    }

    override fun openThermaltream(): Observable<VideoFrame> {
        TODO("Not yet implemented")
    }

    override fun closeThermaltream(): Completable {
        TODO("Not yet implemented")
    }
}