package com.ulsee.thermalapp.data.response

import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.services.DeviceManager

class NotificationList {
    val Action = DeviceManager.Action.notificationListResponse.ordinal
    val FeverList : List<Notification> = ArrayList<Notification>()
}