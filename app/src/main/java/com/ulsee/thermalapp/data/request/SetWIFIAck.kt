package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.services.DeviceManager

class SetWIFIAck {
    val Action = DeviceManager.Action.modifyWifiACK.ordinal
}