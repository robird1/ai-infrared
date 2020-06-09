package com.ulsee.thermalapp.data.services

import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.request.UpdateCalibration
import com.ulsee.thermalapp.data.response.TwoPicture
import io.reactivex.Completable
import io.reactivex.Observable

interface ISettingsService {

    fun get() : Observable<Settings>
    fun update(settings: Settings) : Completable
    fun getTwoPicture() : Observable<TwoPicture>
    fun calibration(calibration: UpdateCalibration) : Completable
}