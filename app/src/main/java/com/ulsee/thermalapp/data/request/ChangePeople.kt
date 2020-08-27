package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.Face

class ChangePeople(face: Face, oldName: String?, changeType: ChangeType) {

    enum class ChangeType(val flag: Int){
        create(0),
        delete(1),
        update(2)
    }

    val Action = 2
//    val Action = 20
    val ID = face.ID
//    val str = if(oldName != null && changeType==ChangeType.update && oldName.equals(face.Name)==false) arrayOf(oldName, face.Name) else arrayOf(face.Name)
    var Name = face.Name
    // this is facecode
    val Data : String? = if (changeType.flag == ChangeType.delete.flag) null else face.Image
    val Flag = changeType.flag

    // TODO
    val Gender = "Female"
    val Age = 28
    val EmployeeID = "333"
    val Remark = "Engineer"



//    init {
//        var fileExtension = "jpg"
//        if (oldName != null) {
//            val arr = oldName!!.split(".")
//            fileExtension = arr[arr.size-1]
//        }
//
//        val idx = str.size - 1
//        val dotIdx = str[idx].indexOfFirst { it == '.' }
//        if (dotIdx < 0) {
//            Name[idx] = str[idx]+"."+fileExtension
//        } else if (dotIdx == str.size-1) {
//            Name[idx] += fileExtension
//        }
//    }
}