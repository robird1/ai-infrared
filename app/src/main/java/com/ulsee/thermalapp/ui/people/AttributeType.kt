package com.ulsee.thermalapp.ui.people

import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Face

enum class AttributeType(val fieldName: String, val position: Int, val viewType: Int, var inputValue: String, var isInputValid: Boolean) {

    FACE("Face", 0, R.layout.item_list_people_face, "", true),
    NAME("Name", 1, R.layout.item_list_people_text, "", false),
    AGE("Age", 2, R.layout.item_list_people_text, "", true),
    GENDER("Gender", 3, R.layout.item_list_people_gender, "", true),
    EMPLOYEE_ID("Employee ID", 4, R.layout.item_list_people_text, "", true),
    BIRTHDATE("Birthdate", 5, R.layout.item_list_people_text, "", true),
    REMARK("Remark", 6, R.layout.item_list_people_text, "", true),
    TestFloatArr("TestFloatArr", 7, R.layout.item_list_people_text,"",true);

    companion object {
        fun fromPosition(position: Int):  AttributeType {
            for (instance in values()) {
                if (instance.position == position)
                    return instance
            }
            return NAME
        }

        fun clearAttributeData() {
            for (i in values()) {
//                if (i.fieldName != FACE.fieldName) {
//                    i.inputValue = ""
//                }

                if (i.fieldName == NAME.fieldName) {
                    i.isInputValid = false
                } else {
                    i.isInputValid = true
                }
            }
        }

        fun setAttributeData(face: Face) {
            FACE.inputValue = face.Image            // base64 format
            NAME.inputValue = face.Name
            AGE.inputValue = face.Age.toString()
            GENDER.inputValue = face.Gender
            EMPLOYEE_ID.inputValue = face.EmployeeID
            BIRTHDATE.inputValue = face.Birthdate       // yyyy/MM/dd
            REMARK.inputValue = face.Remark
        }

        fun getAttributeData(): Face {
            val face = Face()
            face.Image = FACE.inputValue
            face.Name = NAME.inputValue

            // Input value of age has been checked when user presses save button, so it is valid here and
            // can be directly transformed to integer.
            face.Age = AGE.inputValue.toInt()
            face.Gender = GENDER.inputValue
            face.EmployeeID = EMPLOYEE_ID.inputValue
            face.Birthdate = BIRTHDATE.inputValue
            face.Remark = REMARK.inputValue
            return face
        }
    }
}
