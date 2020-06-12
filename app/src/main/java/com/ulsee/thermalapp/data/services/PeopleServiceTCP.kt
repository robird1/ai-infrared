package com.ulsee.thermalapp.data.services

import android.util.Log
import com.google.gson.Gson
import com.ulsee.thermalapp.data.request.ChangePeople
import com.ulsee.thermalapp.data.request.GetFace
import com.ulsee.thermalapp.data.request.GetFaceList
import com.ulsee.thermalapp.data.response.Face
import io.reactivex.Completable
import io.reactivex.CompletableOnSubscribe
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.lang.Exception

class PeopleServiceTCP(deviceManager: DeviceManager) : IPeopleService {

    val deviceManager = deviceManager
    var apiClient : TCPClient? = deviceManager.tcpClient
    val gson = Gson()

    override fun getAll(): Observable<List<com.ulsee.thermalapp.data.model.Face>> {
        val handler: ObservableOnSubscribe<List<com.ulsee.thermalapp.data.model.Face>> = ObservableOnSubscribe<List<com.ulsee.thermalapp.data.model.Face>> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()

            deviceManager.setOnGotFaceListListener(object: DeviceManager.OnGotFaceListListener{
                override fun onGotFaceList(falceList: List<com.ulsee.thermalapp.data.model.Face>) {
                    deviceManager.setOnGotFaceListListener(null)
                    emitter.onNext(falceList)
                    emitter.onComplete()
                }
            })
            apiClient?.send(gson.toJson(GetFaceList()))
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    fun getSingleFace(name: String): Observable<String> {
        val handler: ObservableOnSubscribe<String> = ObservableOnSubscribe<String> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()

            val listener = object: DeviceManager.OnGotFaceListener{
                override fun onFace(face: Face) : Boolean {
                    Log.i(javaClass.name, "got single face, name = "+face.Name+", is got = "+(face.Name == name))
                    if (face.Name == name) {
                        deviceManager.removeOnGotFaceListener(this)
                        emitter.onNext(face.Data!!)
                        emitter.onComplete()
                        return true
                    }
                    return false
                }
            }

            deviceManager.addOnGotFaceListener(listener)
            apiClient?.send(gson.toJson(GetFace(name)))
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun create(face: com.ulsee.thermalapp.data.model.Face): Observable<com.ulsee.thermalapp.data.model.Face> {
        val handler: ObservableOnSubscribe<com.ulsee.thermalapp.data.model.Face> = ObservableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(ChangePeople(face, null, ChangePeople.ChangeType.create)))
            val empty = com.ulsee.thermalapp.data.model.Face()
            emitter.onNext(empty)
            emitter.onComplete()
        }
        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun update(face: com.ulsee.thermalapp.data.model.Face): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(ChangePeople(face, face.oldName, ChangePeople.ChangeType.update)))
            emitter.onComplete()
        }
        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun delete(face: com.ulsee.thermalapp.data.model.Face): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(ChangePeople(face, null, ChangePeople.ChangeType.delete)))
            emitter.onComplete()
        }
        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}