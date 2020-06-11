package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.People

class ChangePeople(people: People, oldName: String?, changeType: ChangeType) {

    enum class ChangeType(val flag: Int){
        create(0),
        delete(1),
        update(2)
    }

    val Name = if(oldName != null && changeType==ChangeType.update && oldName.equals(people.Name)==false) arrayOf(oldName, people.Name) else arrayOf(people.Name)
    val Data = people.Image
    val Flag = changeType.flag
}