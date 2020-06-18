package com.ulsee.thermalapp.data.services

import android.util.Log
import com.google.gson.Gson
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.model.WIFIInfo
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
import kotlin.math.max
import kotlin.math.min

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
            apiClient?.send(gson.toJson(UpdateSettings.from(settings)))
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

    fun calculateFPS(sentCount: Int, recvCount: Int) : Long {
        var fps = 20L

        if (sentCount - recvCount > 3) {
            fps = (20 - min(19, (sentCount-recvCount)*3)).toLong()
        }

        Log.i(javaClass.name, String.format("%d sent, %d recv, new fps = %d", sentCount, recvCount, fps))

        return fps
    }

    override fun openRGBStream(): Observable<String> {
        val handler: ObservableOnSubscribe<String> = ObservableOnSubscribe<String> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()

            var fps = 20L
            var interval = 50L
            var sentCount = 0
            var recvCount = 0
            var i = 0L

            deviceManager.setOnGotVideoFrameListener(object: DeviceManager.OnGotVideoFrameListener{
                override fun onVideoFrame(frame: String) {
                    val newFPS = calculateFPS(sentCount, ++recvCount)
                    if (newFPS != fps) {
                        fps = newFPS
                    }
                    emitter.onNext(frame)
                }
            })
            try {
                while(!emitter.isDisposed) {
                    i ++
                    i %= 20
                    if (i % (20L/fps) == 0L) {
                        apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.openRGB)))
                        val newFPS = calculateFPS(++sentCount, recvCount)
                        if (newFPS != fps) {
                            fps = newFPS
                        }
                    }
                    Thread.sleep(interval)
                }
                Log.i(javaClass.name, "end send loop")
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

            var fps = 20L
            var sentCount = 0
            var recvCount = 0

            deviceManager.setOnGotVideoFrameListener(object: DeviceManager.OnGotVideoFrameListener{
                override fun onVideoFrame(frame: String) {
                    fps = calculateFPS(sentCount, ++recvCount)
                    emitter.onNext(frame)
                }
            })
            try {
                while(!emitter.isDisposed) {
                    apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.openThermal)))
                    fps = calculateFPS(++sentCount, recvCount)
                    Thread.sleep(1000/fps)
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

    fun switchWIFI(wifiInfo: WIFIInfo?, advanceParameter: SetWIFI.AdvanceParameter?): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(SetWIFI(wifiInfo, advanceParameter)))
            emitter.onComplete()
        }
        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    fun ackWIFI(): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(SetWIFIAck()))
            emitter.onComplete()
        }
        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}