package com.ulsee.thermalapp.data.api
import com.ulsee.thermalapp.data.model.Settings
import io.reactivex.Completable
import io.reactivex.Observable
import retrofit2.http.Body
import retrofit2.http.GET
import retrofit2.http.Headers
import retrofit2.http.PUT

interface SettingsAPI {

    @GET("api/settings")
    fun get() : Observable<Settings>

    @Headers("Content-Type: application/json","Accept: application/json")
    @PUT("api/settings")
    fun update(@Body value: Settings) : Completable

    @Headers("Content-Type: application/json","Accept: application/json")
    @PUT("api/settings/temperature-unit")
    fun updateTemperatureUnit(@Body value: Settings) : Completable

    @Headers("Content-Type: application/json","Accept: application/json")
    @PUT("api/settings/alarm-threshold")
    fun updateAlarmThreshold(@Body value: Settings) : Completable

    @Headers("Content-Type: application/json","Accept: application/json")
    @PUT("api/settings/temperature-offset")
    fun updateTemperatureOffset(@Body value: Settings) : Completable

    @Headers("Content-Type: application/json","Accept: application/json")
    @PUT("api/settings/is-flip-horizontal")
    fun updateFlipHorizontal(@Body value: Settings) : Completable
}