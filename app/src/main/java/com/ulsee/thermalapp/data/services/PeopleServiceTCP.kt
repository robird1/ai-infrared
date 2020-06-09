package com.ulsee.thermalapp.data.services

import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.ulsee.thermalapp.data.model.People
import com.ulsee.thermalapp.data.request.GetFaceList
import com.ulsee.thermalapp.data.response.FaceList
import io.reactivex.Completable
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.lang.Exception
import java.lang.StringBuilder

class PeopleServiceTCP(apiClient: TCPClient) : IPeopleService {

    var apiClient : TCPClient? = apiClient
    val gson = Gson()

    override fun getAll(): Observable<List<People>> {
        val handler: ObservableOnSubscribe<List<People>> = ObservableOnSubscribe<List<People>> { emitter ->
            if (apiClient == null) throw Exception("error: target not specified")
            if (apiClient?.isConnected() != true)throw Exception("error: target not connected")
//            if (apiClient?.isConnected() != true) apiClient?.reconnect()
            val stringBuilder = StringBuilder()

            apiClient?.setOnReceivedDataListener(object: TCPClient.OnReceivedDataListener{
                override fun onData(data: CharArray, size: Int) {
                    stringBuilder.append(data, 0, size)

                    if (!stringBuilder.endsWith("}")) return;

                    val responseString = stringBuilder.toString()
                    val itemType = object : TypeToken<FaceList>() {}.type
                    val faceList = gson.fromJson<FaceList>(responseString, itemType)
                    apiClient?.setOnReceivedDataListener(null)
                    emitter.onNext(faceList.facelist)
                    emitter.onComplete()
                }
            })
            apiClient?.send(gson.toJson(GetFaceList()))
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    override fun create(people: People): Observable<People> {
        TODO("Not yet implemented")
    }

    override fun update(people: People): Completable {
        TODO("Not yet implemented")
    }

    override fun delete(id: Int): Completable {
        TODO("Not yet implemented")
    }
}