package com.ulsee.thermalapp.data.services

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.request.*
import com.ulsee.thermalapp.data.response.TwoPicture
import com.ulsee.thermalapp.data.response.VideoFrame
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

    override fun openRGBStream(): Observable<VideoFrame> {
        val handler: ObservableOnSubscribe<VideoFrame> = ObservableOnSubscribe<VideoFrame> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()
            val stringBuilder = StringBuilder()

            apiClient?.setOnReceivedDataListener(object: TCPClient.OnReceivedDataListener{
                override fun onData(data: CharArray, size: Int) {
                    stringBuilder.append(data, 0, size)

                    if (!stringBuilder.startsWith("{")) {
                        Log.e(javaClass.name, "response not start with {, error, drop...")
                        stringBuilder.clear()
                        return
                    }

                    if (!stringBuilder.endsWith("}")) return

                    var responseString = stringBuilder.toString()
                    val len = parseStickyPacketFirstPacketLength(responseString)
                    if(len == -1) return
                    responseString = responseString.substring(0, len)
                    stringBuilder.removeRange(0, len)
//                    stringBuilder.clear()
                    val itemType = object : TypeToken<VideoFrame>() {}.type
                    try {
                        val videoFrame = gson.fromJson<VideoFrame>(responseString, itemType)
                        emitter.onNext(videoFrame)
                        Log.i(javaClass.name, "got video frame")
                    } catch(e:Exception) {
                        Log.e(javaClass.name, responseString)
                        e.printStackTrace()
                    }
//                    apiClient?.setOnReceivedDataListener(null)
//                    emitter.onComplete()
                }
            })
            apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.openRGB)))
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun closeRGBStream(): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            apiClient?.setOnReceivedDataListener(null)
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()
            apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.closeRGB)))
        }

        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun openThermaltream(): Observable<VideoFrame> {
        val handler: ObservableOnSubscribe<VideoFrame> = ObservableOnSubscribe<VideoFrame> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()
            val stringBuilder = StringBuilder()

            apiClient?.setOnReceivedDataListener(object: TCPClient.OnReceivedDataListener{
                override fun onData(data: CharArray, size: Int) {
                    stringBuilder.append(data, 0, size)

                    if (!stringBuilder.startsWith("{")) {
                        Log.e(javaClass.name, "response not start with {, error, drop...")
                        stringBuilder.clear()
                        return
                    }

                    if (!stringBuilder.endsWith("}")) return

                    var responseString = stringBuilder.toString()
                    val len = parseStickyPacketFirstPacketLength(responseString)
                    if(len == -1) return
                    responseString = responseString.substring(0, len)
                    stringBuilder.removeRange(0, len)
//                    stringBuilder.clear()

                    val itemType = object : TypeToken<VideoFrame>() {}.type
                    try {
                        val videoFrame = gson.fromJson<VideoFrame>(responseString, itemType)
                        emitter.onNext(videoFrame)
                        Log.i(javaClass.name, "got video frame")
                    } catch(e:Exception) {
                        Log.e(javaClass.name, responseString)
                        e.printStackTrace()
                    }
//                    apiClient?.setOnReceivedDataListener(null)
//                    emitter.onComplete()
                }
            })
            apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.openThermal)))
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun closeThermaltream(): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            apiClient?.setOnReceivedDataListener(null)
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()
            apiClient?.send(gson.toJson(SetVideo(SetVideo.VideoStatus.closeThermal)))
        }

        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    private fun parseStickyPacketFirstPacketLength(content: String) : Int {
        var leftBigParanthesesCount = 0
        var rightBigParanthesesCount = 0
        for(i in 0 until content.length-1) {
            if(content[i] == '{')leftBigParanthesesCount +=1
            if(content[i] == '}')rightBigParanthesesCount +=1
            if (leftBigParanthesesCount>0 && leftBigParanthesesCount==rightBigParanthesesCount) {
                return i+1
            }
        }
        return -1
    }
}