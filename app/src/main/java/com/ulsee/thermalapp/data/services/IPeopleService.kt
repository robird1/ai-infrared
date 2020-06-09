package com.ulsee.thermalapp.data.services

import com.ulsee.thermalapp.data.model.People
import io.reactivex.Completable
import io.reactivex.Observable

interface IPeopleService {
    fun getAll(): Observable<List<People>>
    fun create(people: People): Observable<People>
    fun update(people: People): Completable
    fun delete(id: Int): Completable
}