package com.ulsee.thermalapp.ui.network

import android.content.Context
import com.ulsee.thermalapp.data.model.WIFIInfo

class NetworkController(private val context: Context) {
    private var mController: INetwork

    init {
        mController = getInstance()
    }

    fun requestWifi(wifiInfo: WIFIInfo): Boolean {
        return mController.requestWifi(context, wifiInfo)
    }

    private fun getInstance(): INetwork {
        return if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.Q) {
            VersionAboveQ()
        } else {
            VersionBelowQ()
        }
    }
}

abstract class INetwork {
    abstract fun requestWifi(context: Context, wifiInfo: WIFIInfo): Boolean
}
