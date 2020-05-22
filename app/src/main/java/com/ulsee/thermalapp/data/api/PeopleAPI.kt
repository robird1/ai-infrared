package com.ulsee.thermalapp.data.api

import com.ulsee.thermalapp.data.model.People
import io.reactivex.Observable
import retrofit2.Response
import retrofit2.http.*

interface PeopleAPI {

    @GET("api/people")
    fun getAll() : Observable<List<People>>

    @Headers("Content-Type: application/json","Accept: application/json")
    @POST("api/people")
    fun create(@Body value: People) : Observable<Response<People>>

    @Headers("Content-Type: application/json","Accept: application/json")
    @PUT("api/people/{id}")
    fun update(@Path("id") id: Int, @Body value: People) : Observable<Void>

    @DELETE("api/people/{id}")
    fun delete(@Path("id") id: Int) : Observable<Void>
}