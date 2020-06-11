package com.ulsee.thermalapp.data.services

import com.google.gson.Gson
import com.ulsee.thermalapp.data.model.People
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

    override fun getAll(): Observable<List<People>> {
        val handler: ObservableOnSubscribe<List<People>> = ObservableOnSubscribe<List<People>> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()

            deviceManager.setOnGotFaceListListener(object: DeviceManager.OnGotFaceListListener{
                override fun onGotFaceList(falceList: List<People>) {
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
                override fun onFace(face: Face) {
                    if (face.Name == name) {
                        deviceManager.removeOnGotFaceListener(this)
                        emitter.onNext(face.Data!!)
                        emitter.onComplete()
                    }
                }
            }

            deviceManager.addOnGotFaceListener(listener)
            apiClient?.send(gson.toJson(GetFace(name)))
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun create(people: People): Observable<People> {
        val handler: ObservableOnSubscribe<People> = ObservableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(ChangePeople(people, null, ChangePeople.ChangeType.create)))
            val empty = People()
            emitter.onNext(empty)
            emitter.onComplete()
        }
        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun update(people: People): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(ChangePeople(people, people.oldName, ChangePeople.ChangeType.update)))
            emitter.onComplete()
        }
        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun delete(people: People): Completable {
        val handler: CompletableOnSubscribe = CompletableOnSubscribe { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true) throw Exception("error: target not connected")
            apiClient?.send(gson.toJson(ChangePeople(people, null, ChangePeople.ChangeType.delete)))
            emitter.onComplete()
        }
        return Completable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}