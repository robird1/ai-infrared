package com.ulsee.thermalapp.data.services

import com.google.gson.Gson
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.request.UpdateCalibration
import com.ulsee.thermalapp.data.request.UpdateSettings
import io.reactivex.Completable
import io.reactivex.CompletableOnSubscribe
import io.reactivex.Observable
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.lang.Exception

class SettingsServiceTCP(apiClient: TCPClient) : ISettingsService {

    var apiClient : TCPClient? = apiClient
    val gson = Gson()

    override fun get(): Observable<Settings> {
        TODO("Not yet implemented")
    }

    override fun update(settings: Settings): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(UpdateSettings(settings)))
            emitter.onComplete()
        }
        return Completable.create(handler).subscribeOn(Schedulers.newThread())
                .observeOn(AndroidSchedulers.mainThread())
    }

    override fun calibration(calibration: UpdateCalibration): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(calibration))
            emitter.onComplete()
        }
        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}