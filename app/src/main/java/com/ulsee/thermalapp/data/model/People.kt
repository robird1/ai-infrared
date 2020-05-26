package com.ulsee.thermalapp.data.model

import java.io.Serializable

data class People (
    val ID : Int,
    val Name : String,
    val AvatarURL : String?
) : Serializable