package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.Face

//import com.ulsee.thermalapp.data.response.Face

class GetFace(face: Face) {
    val Action: Int = 10
    val ID : String = face.ID
    val Name: String = face.Name
}