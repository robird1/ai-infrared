package com.ulsee.thermalapp.data.model

import java.io.Serializable

open class Face: Serializable {

    var ID : Int = 0
    var Name : String = ""
    var Image : String? = null
    var oldName: String? = null
    val NameWihtoutFileType : String
        get() {
            val arr = Name.split(".")
            if (arr.size < 2) return Name
            return Name.substring(0, Name.length - arr[arr.size-1].length - 1)
        }
}