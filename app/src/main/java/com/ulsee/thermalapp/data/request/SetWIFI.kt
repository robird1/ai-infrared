package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.WIFIInfo
import com.ulsee.thermalapp.data.services.DeviceManager

class SetWIFI(wifiInfo: WIFIInfo) {
    val Action = DeviceManager.Action.modifyWifi.ordinal
    val Flag : Int = 1
    val SSID = wifiInfo.ssid
    val Password = wifiInfo.password
    val IP = ""
    val Mask = ""
    val Gateway = ""
    val DNS = ""
}