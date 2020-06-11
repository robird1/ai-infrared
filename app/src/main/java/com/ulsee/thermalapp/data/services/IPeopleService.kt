package com.ulsee.thermalapp.data.services

import com.ulsee.thermalapp.data.model.Face
import io.reactivex.Completable
import io.reactivex.Observable

interface IPeopleService {
    fun getAll(): Observable<List<Face>>
    fun create(face: Face): Observable<Face>
    fun update(face: Face): Completable
    fun delete(face: Face): Completable
}