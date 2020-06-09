package com.ulsee.thermalapp.data.services

import com.ulsee.thermalapp.data.model.Settings
import io.reactivex.Completable
import io.reactivex.Observable

interface ISettingsService {

    fun get() : Observable<Settings>
    fun update(settings: Settings) : Completable
}