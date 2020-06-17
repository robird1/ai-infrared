package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.WIFIInfo
import com.ulsee.thermalapp.data.services.DeviceManager

class SetWIFI(wifiInfo: WIFIInfo?, advanceParameter: AdvanceParameter?) {
    class AdvanceParameter {
        val IP = ""
        val Mask = ""
        val Gateway = ""
        val DNS = ""
    }
    val Action = DeviceManager.Action.modifyWifi.ordinal

    val Flag : Int = if (wifiInfo == null) 0 else (if(advanceParameter == null) 1 else 2)

    val SSID : String? = wifiInfo?.ssid
    val Password : String?  = wifiInfo?.password

    val IP : String?  = advanceParameter?.IP
    val Mask : String?  = advanceParameter?.Mask
    val Gateway : String?  = advanceParameter?.Gateway
    val DNS  : String? = advanceParameter?.DNS

//    val Flag : Int = 2
//    val IP = "192.168.11.197"
//    val Mask = "255.255.255.0"
//    val Gateway = "192.168.11.254"
//    val DNS = "8.8.8.8 8.8.4.4"
}