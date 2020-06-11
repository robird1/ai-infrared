package com.ulsee.thermalapp.data.api

import com.ulsee.thermalapp.data.model.Face
import io.reactivex.Completable
import io.reactivex.Observable
import retrofit2.Response
import retrofit2.http.*

interface PeopleAPI {

    @GET("api/people")
    fun getAll() : Observable<List<Face>>

    @Headers("Content-Type: application/json","Accept: application/json")
    @POST("api/people")
    fun create(@Body value: Face) : Observable<Response<Face>>

    @Headers("Content-Type: application/json","Accept: application/json")
    @PUT("api/people/{id}")
    fun update(@Path("id") id: Int, @Body value: Face) : Completable

    @DELETE("api/people/{id}")
    fun delete(@Path("id") id: Int) : Completable
}