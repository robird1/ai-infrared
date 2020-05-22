package com.ulsee.thermalapp.data.services

import com.google.gson.Gson
import com.ulsee.thermalapp.data.api.PeopleAPI
import com.ulsee.thermalapp.data.model.ErrorResponse
import com.ulsee.thermalapp.data.model.People
import io.reactivex.Observable
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import retrofit2.Response
import retrofit2.Retrofit
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory
import retrofit2.converter.gson.GsonConverterFactory
import java.lang.Exception


class PeopleService {

    private val gson = Gson()
    private val client = Retrofit.Builder()
        .baseUrl("http://192.168.11.155:8080")
        .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
        .addConverterFactory(GsonConverterFactory.create())
        .build().create(PeopleAPI::class.java)

    fun getAll(): Observable<List<People>> {
        return client.getAll()
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    fun create(people: People): Observable<People> {
        return client.create(people)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io())
            .flatMap { response: Response<People> ->
                if (response.isSuccessful) {
                    Observable.just(response.body());
                } else {
                    val errorResponse = gson.fromJson(response.errorBody()!!.charStream(), ErrorResponse::class.java)
                    Observable.error(Exception(errorResponse.error))
                }
            };
    }

    fun update(people: People): Observable<Void> {
        return client.update(people.ID, people)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }

    fun delete(id: Int): Observable<Void> {
        return client.delete(id)
            .observeOn(AndroidSchedulers.mainThread())
            .subscribeOn(Schedulers.io());
    }
}