package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.services.DeviceManager

class GetNotificationImage(name: String) {
    val Action = DeviceManager.Action.requestNotificationImage.ordinal
    val Name: String = name
}