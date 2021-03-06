package com.ulsee.thermalapp.ui.people

import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Face

enum class AttributeType(val fieldName: Int, val position: Int, val viewType: Int, var inputValue: String, var isInputValid: Boolean) {

//    FACE("Face", 0, R.layout.item_list_people_face, "", true),
    NAME(R.string.activity_editor_name, 0, R.layout.item_list_people_text, "", false),
    AGE(R.string.activity_editor_age, 1, R.layout.item_list_people_text, "", true),
    GENDER(R.string.activity_editor_gender, 2, R.layout.item_list_people_gender, "", true),
    EMPLOYEE_ID(R.string.activity_editor_id, 3, R.layout.item_list_people_text, "", true),
    BIRTHDAY(R.string.activity_editor_birthday, 4, R.layout.item_list_people_text, "", true),
    REMARK(R.string.activity_editor_remark, 5, R.layout.item_list_people_text, "", true);

    companion object {
        var id: String = ""
        var faceCode: String = ""

        fun fromPosition(position: Int):  AttributeType {
            for (instance in values()) {
                if (instance.position == position)
                    return instance
            }
            return NAME
        }

        fun clearAttributeData() {
            id = ""
            faceCode = ""
            for (i in values()) {
                if (i.fieldName == NAME.fieldName) {
                    i.isInputValid = false
                } else {
                    i.isInputValid = true
                }
            }
        }

        fun setAttributeData(face: Face) {
//            FACE.inputValue = face.Image            // base64 format
            id = face.ID
            faceCode = face.Data
            NAME.inputValue = face.Name
            AGE.inputValue = face.Age.toString()
            GENDER.inputValue = face.Gender
            EMPLOYEE_ID.inputValue = face.EmployeeID
            BIRTHDAY.inputValue = face.Birthday       // yyyy/MM/dd
            REMARK.inputValue = face.Remark
        }

        fun getAttributeData(): Face {
            val face = Face()
//            face.Image = FACE.inputValue
            face.ID = id
            face.Data = faceCode
            face.Name = NAME.inputValue

            if (AGE.inputValue == "0" || AGE.inputValue == "") {
                face.Age = 0
            } else {
                face.Age = AGE.inputValue.toInt()
            }
            face.Gender = GENDER.inputValue
            face.EmployeeID = EMPLOYEE_ID.inputValue
            face.Birthday = BIRTHDAY.inputValue
            face.Remark = REMARK.inputValue

            return face
        }
    }
}
