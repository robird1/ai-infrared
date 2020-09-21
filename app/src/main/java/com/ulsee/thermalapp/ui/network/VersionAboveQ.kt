package com.ulsee.thermalapp.ui.network

import android.content.Context
import android.net.ConnectivityManager
import android.net.Network
import android.net.NetworkCapabilities
import android.net.NetworkRequest
import android.net.wifi.WifiNetworkSpecifier
import android.util.Log
import com.ulsee.thermalapp.data.model.WIFIInfo

private val TAG = VersionAboveQ::class.java.simpleName

class VersionAboveQ: INetwork() {
    override fun requestWifi(context: Context, wifiInfo: WIFIInfo): Boolean {
        val specifier = WifiNetworkSpecifier.Builder()
            .setSsid(wifiInfo.ssid)
            .setWpa2Passphrase(wifiInfo.password!!)
            .build()
        val networkRequest = NetworkRequest.Builder()
            .addTransportType(NetworkCapabilities.TRANSPORT_WIFI)
            .setNetworkSpecifier(specifier)
            .build()

        val connectivityManager =
            context.getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager

        // not sure, if this is needed
//            if (networkCallback != null) {
//                connectivityManager.unregisterNetworkCallback(networkCallback)
//            }

        val networkCallback = object : ConnectivityManager.NetworkCallback() {
            override fun onAvailable(network: Network) {
                super.onAvailable(network)
                Log.d(TAG, "[Enter] onAvailable() AndroidQ+ connected to wifi...")
                // bind so all api calls are performed over this new network
                connectivityManager.bindProcessToNetwork(network)
            }

            override fun onUnavailable() {
                super.onUnavailable()
                Log.d(TAG, "[Enter] onAvailable() AndroidQ+ could not connect to wifi...")
            }
        }

        connectivityManager.requestNetwork(networkRequest, networkCallback)

        return true
    }

}