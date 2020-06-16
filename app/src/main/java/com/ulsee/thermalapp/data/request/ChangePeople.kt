package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.Face

class ChangePeople(face: Face, oldName: String?, changeType: ChangeType) {

    enum class ChangeType(val flag: Int){
        create(0),
        delete(1),
        update(2)
    }

    val Action = 2
    val str = if(oldName != null && changeType==ChangeType.update && oldName.equals(face.Name)==false) arrayOf(oldName, face.Name) else arrayOf(face.Name)
    var Name = str
    val Data : String? = if (changeType.flag == ChangeType.delete.flag) null else face.Image
    val Flag = changeType.flag

    init {
        var fileExtension = "jpg"
        if (oldName != null) {
            val arr = oldName!!.split(".")
            fileExtension = arr[arr.size-1]
        }

        val dotIdx = str[0].indexOfFirst { it == '.' }
        if (dotIdx < 0) {
            Name[0] = str[0]+"."+fileExtension
        } else if (dotIdx == str.size-1) {
            Name[0] += fileExtension
        }
    }
}