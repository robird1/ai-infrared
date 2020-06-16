package com.ulsee.thermalapp.data.services

import com.google.gson.Gson
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.request.*
import com.ulsee.thermalapp.data.response.TwoPicture
import io.reactivex.Completable
import io.reactivex.CompletableOnSubscribe
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.lang.Exception
import java.lang.StringBuilder

class SettingsServiceTCP(deviceManager: DeviceManager) : ISettingsService {

    val deviceManager = deviceManager
    var apiClient : TCPClient? = deviceManager.tcpClient
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

            deviceManager.setOnGotTwoPictureListListener(object: DeviceManager.OnGotTwoPictureListListener{
                override fun onGotTwoPictureList(rgb: String, thermal: String) {
                    deviceManager.setOnGotTwoPictureListListener(null)
                    emitter.onNext(TwoPicture(0,rgb,thermal))
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

    override fun openRGBStream(): Observable<String> {
        val handler: ObservableOnSubscribe<String> = ObservableOnSubscribe<String> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()

            deviceManager.setOnGotVideoFrameListener(object: DeviceManager.OnGotVideoFrameListener{
                override fun onVideoFrame(frame: String) {
                    emitter.onNext(frame)
                }
            })
            try {
                while(!emitter.isDisposed) {
                    apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.openRGB)))
                    Thread.sleep(50) // 20 fps
                }
            } catch(e: Exception) {
                if (!emitter.isDisposed) emitter.onError(e)
            }
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun closeRGBStream(): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe {
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()
            deviceManager.setOnGotVideoFrameListener(null)
            apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.closeRGB)))
            it.onComplete()
        }

        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun openThermaltream(): Observable<String> {
        val handler: ObservableOnSubscribe<String> = ObservableOnSubscribe<String> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()

            deviceManager.setOnGotVideoFrameListener(object: DeviceManager.OnGotVideoFrameListener{
                override fun onVideoFrame(frame: String) {
                    emitter.onNext(frame)
                }
            })
            try {
                while(!emitter.isDisposed) {
                    apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.openThermal)))
                    Thread.sleep(50) // 20 fps
                }
            } catch(e: Exception) {
                if (!emitter.isDisposed) emitter.onError(e)
            }
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun closeThermaltream(): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe {
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()
            deviceManager.setOnGotVideoFrameListener(null)
            apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.closeThermal)))
            it.onComplete()
        }

        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}