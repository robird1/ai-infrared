package com.ulsee.thermalapp.data.services

import com.google.gson.Gson
import com.ulsee.thermalapp.data.api.PeopleAPI
import com.ulsee.thermalapp.data.model.ErrorResponse
import com.ulsee.thermalapp.data.model.Face
import io.reactivex.Completable
import io.reactivex.Observable
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import retrofit2.Response
import retrofit2.Retrofit
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory
import retrofit2.converter.gson.GsonConverterFactory
import java.lang.Exception

class PeopleServiceHTTP : IPeopleService {

    private val gson = Gson()
    private val client = Retrofit.Builder()
        .baseUrl("http://192.168.11.155:8080")
        .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
        .addConverterFactory(GsonConverterFactory.create())
        .build().create(PeopleAPI::class.java)

    override fun getAll(): Observable<List<Face>> {
        return client.getAll()
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    override fun create(face: Face): Observable<Face> {
        return client.create(face)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io())
            .flatMap { response: Response<Face> ->
                if (response.isSuccessful) {
                    Observable.just(response.body());
                } else {
                    val errorResponse = gson.fromJson(response.errorBody()!!.charStream(), ErrorResponse::class.java)
                    Observable.error(Exception(errorResponse.error))
                }
            };
    }

    override fun update(face: Face): Completable {
        return client.update(face.ID, face)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    override fun delete(face: Face): Completable {
        return client.delete(face.ID)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }
}