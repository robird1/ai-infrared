package com.ulsee.thermalapp.data.model

import java.io.Serializable

open class Face: Serializable {

    var ID : String = "-1"
    var Name : String = ""
//    var Image : String? = null
    var Image : String = ""
    var Data : String? = null
    var oldName: String? = null
    var Gender : String = ""
    var Age : Int = 0
    var EmployeeID = ""
    var Birthdate = ""
    var Remark = ""
    val NameWihtoutFileType : String
        get() {
            val arr = Name.split(".")
            if (arr.size < 2) return Name
            return Name.substring(0, Name.length - arr[arr.size-1].length - 1)
        }
    var TestFloatArr : FloatArray? = null
}