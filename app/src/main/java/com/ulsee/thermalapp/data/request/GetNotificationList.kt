package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.services.DeviceManager

class GetNotificationList {
    val Action = DeviceManager.Action.requestNotificationList.ordinal
}