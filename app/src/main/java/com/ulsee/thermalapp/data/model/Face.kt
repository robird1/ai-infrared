package com.ulsee.thermalapp.data.model

import java.io.Serializable

open class Face: Serializable {

    var ID : Int = 0
    var Name : String = ""
    var Image : String? = null
    var oldName: String? = null
}