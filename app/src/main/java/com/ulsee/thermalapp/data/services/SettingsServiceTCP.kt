package com.ulsee.thermalapp.data.services

import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.ulsee.thermalapp.data.model.People
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.request.GetFaceList
import com.ulsee.thermalapp.data.request.GetTwoPicture
import com.ulsee.thermalapp.data.request.UpdateCalibration
import com.ulsee.thermalapp.data.request.UpdateSettings
import com.ulsee.thermalapp.data.response.FaceList
import com.ulsee.thermalapp.data.response.TwoPicture
import io.reactivex.Completable
import io.reactivex.CompletableOnSubscribe
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.lang.Exception
import java.lang.StringBuilder

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

    override fun getTwoPicture(): Observable<TwoPicture> {
        val handler: ObservableOnSubscribe<TwoPicture> = ObservableOnSubscribe<TwoPicture> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()
            val stringBuilder = StringBuilder()

            apiClient?.setOnReceivedDataListener(object: TCPClient.OnReceivedDataListener{
                override fun onData(data: CharArray, size: Int) {
                    stringBuilder.append(data, 0, size)

                    if (!stringBuilder.endsWith("}")) return;

                    val responseString = stringBuilder.toString()
                    val itemType = object : TypeToken<TwoPicture>() {}.type
                    val twoPicture = gson.fromJson<TwoPicture>(responseString, itemType)
                    apiClient?.setOnReceivedDataListener(null)
                    emitter.onNext(twoPicture)
                    emitter.onComplete()
                }
            })
            apiClient?.send(gson.toJson(GetTwoPicture()))
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
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