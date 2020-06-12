package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.Face

class ChangePeople(face: Face, oldName: String?, changeType: ChangeType) {

    enum class ChangeType(val flag: Int){
        create(0),
        delete(1),
        update(2)
    }

    val Action = 2
    val Name = if(oldName != null && changeType==ChangeType.update && oldName.equals(face.Name)==false) arrayOf(oldName, face.Name) else arrayOf(face.Name)
    val Data : String? = if (changeType.flag == ChangeType.delete.flag) null else face.Image
    val Flag = changeType.flag
}