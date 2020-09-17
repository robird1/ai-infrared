package com.ulsee.thermalapp.data.services

import android.util.Log
import com.google.gson.Gson
import com.ulsee.thermalapp.data.model.FilteredRecord
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.request.GetNotificationImage
import com.ulsee.thermalapp.data.request.GetNotificationList
import com.ulsee.thermalapp.data.response.NotificationImage
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers

class NotificationServiceTCP(deviceManager: DeviceManager) {
    val deviceManager = deviceManager
    var apiClient : TCPClient? = deviceManager.tcpClient
    val gson = Gson()

     fun getAll(): Observable<List<Notification>> {
         Log.d("NotificationServiceTCP", "[Enter] getAll")

         val handler: ObservableOnSubscribe<List<Notification>> = ObservableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()

            deviceManager.setOnGotNotificationListListener(object: DeviceManager.OnGotNotificationListListener{
                override fun onGotNotificationList(falceList: List<Notification>) {
                    Log.d("NotificationServiceTCP", "[Enter] onGotNotificationList")

                    deviceManager.setOnGotNotificationListListener(null)
                    emitter.onNext(falceList)
                    emitter.onComplete()
                }
            })
            apiClient?.send(gson.toJson(GetNotificationList()))

        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    fun getFilteredRecord(data: FilteredRecord): Observable<List<Notification>> {
        val handler: ObservableOnSubscribe<List<Notification>> = ObservableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")

            deviceManager.setOnGotNotificationListListener(object: DeviceManager.OnGotNotificationListListener{
                override fun onGotNotificationList(falceList: List<Notification>) {
                    deviceManager.setOnGotNotificationListListener(null)
                    emitter.onNext(falceList)
                    emitter.onComplete()
                }
            })
            apiClient?.send(gson.toJson(data))
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    fun getSingleNotification(id: String): Observable<String> {
        val handler: ObservableOnSubscribe<String> = ObservableOnSubscribe<String> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()

            val listener = object: DeviceManager.OnGotNotificationImageListener{
                override fun onNotificationImage(image: NotificationImage): Boolean {
//                    Log.i(javaClass.name, "got single face, name = "+image.Name+", is got = "+(image.Name == name))
                    if (image.ID == id) {
                        deviceManager.removeOnGotNotificationImageListener(this)
                        emitter.onNext(image.Data!!)
                        emitter.onComplete()
                        return true
                    }
                    return false
                }
            }

            deviceManager.addOnGotNotificationImageListener(listener)
            apiClient?.send(gson.toJson(GetNotificationImage(id)))

        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}