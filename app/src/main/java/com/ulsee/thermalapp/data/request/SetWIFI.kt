package com.ulsee.thermalapp.data.request

import com.ulsee.thermalapp.data.model.WIFIInfo
import com.ulsee.thermalapp.data.services.DeviceManager

class SetWIFI(wifiInfo: WIFIInfo, advanceParameter: AdvanceParameter?) {
    class AdvanceParameter {
        val IP = ""
        val Mask = ""
        val Gateway = ""
        val DNS = ""
    }
    val Action = DeviceManager.Action.modifyWifi.ordinal
    val SSID = wifiInfo.ssid
    val Password = wifiInfo.password

    val Flag : Int = if(advanceParameter == null) 1 else 2
    val IP = if(advanceParameter == null) "" else advanceParameter!!.IP
    val Mask = if(advanceParameter == null) "" else advanceParameter!!.Mask
    val Gateway = if(advanceParameter == null) "" else advanceParameter!!.Gateway
    val DNS = if(advanceParameter == null) "" else advanceParameter!!.DNS

//    val Flag : Int = 2
//    val IP = "192.168.11.197"
//    val Mask = "255.255.255.0"
//    val Gateway = "192.168.11.254"
//    val DNS = "8.8.8.8 8.8.4.4"
}