package com.ulsee.thermalapp.ui.network

import android.content.Context
import android.net.wifi.WifiConfiguration
import android.net.wifi.WifiManager
import android.util.Log
import com.ulsee.thermalapp.data.model.WIFIInfo

private val TAG = VersionBelowQ::class.java.simpleName

class VersionBelowQ: INetwork() {
    override fun requestWifi(context: Context, wifiInfo: WIFIInfo): Boolean {
        Log.d(TAG, "[Enter] connect id: " + wifiInfo.ssid + " password: " + wifiInfo.password)

        val networkSSID = wifiInfo.ssid
        val networkPass = wifiInfo.password

        val conf = WifiConfiguration()
        conf.SSID = "\"" + networkSSID + "\""

        val wifiType = wifiInfo.wifiType

        when (wifiType) {
            WIFIInfo.WIFIType.wep -> {
                conf.wepKeys[0] = "\"" + networkPass + "\"";
                conf.wepTxKeyIndex = 0;
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                conf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            }
            WIFIInfo.WIFIType.wpa -> {
                conf.preSharedKey = "\"" + networkPass + "\"";
            }
            WIFIInfo.WIFIType.open -> {
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            }
        }

        val wifiManager = (context.getSystemService(Context.WIFI_SERVICE) as WifiManager)
        wifiManager.addNetwork(conf)

        val list: List<WifiConfiguration> = wifiManager.getConfiguredNetworks()
        for (i in list) {
            if (i.SSID != null && i.SSID == "\"" + networkSSID + "\"") {
//                    wifiManager.disconnect()
                val result = wifiManager.enableNetwork(i.networkId, true)
//                    val result = wifiManager.reconnect()
                if (result) {
                    return true
                } else {
                    Log.d(TAG, "[Failed] wifiManager.reconnect() ")
//                        setResult(Activity.RESULT_FIRST_USER)
                }
            } else {
                Log.d(TAG, "[Enter] else block ")
            }
        }
        Log.d(TAG, "wifiManager.getConfiguredNetworks() is empty.")
        return false


//        if(isConnectedTo(context, wifiInfo.ssid)) {
////            toast("Connected to"+wifiInfo.ssid)
//            Log.d(TAG, "Connected to"+wifiInfo.ssid)
//            return true
//        }
//
//        val wm = context.getSystemService(Context.WIFI_SERVICE) as WifiManager
//        var wifiConfig= getWiFiConfig(context, wifiInfo.ssid)
//        if(wifiConfig == null) { //if the given ssid is not present in the WiFiConfig, create a config for it
//            // TODO password might be null
//            createTypeProfile(context, wifiInfo)
//            wifiConfig = getWiFiConfig(context, wifiInfo.ssid)
//        }
//        wm.disconnect()
//        Log.d(TAG,"wifiConfig!!.networkId: "+wifiConfig!!.networkId);
//        wm.enableNetwork(wifiConfig!!.networkId,true)
//        wm.reconnect()
//        Log.d(TAG,"intiated connection to SSID: "+wifiInfo.ssid);
//
//        // TODO
//        return true
    }

    private fun isConnectedTo(context: Context, ssid: String): Boolean {
        val wm:WifiManager= context.getSystemService(Context.WIFI_SERVICE) as WifiManager
        if(wm.connectionInfo.ssid == ssid){
            return true
        }
        return false
    }

    private fun getWiFiConfig(context: Context, ssid: String): WifiConfiguration? {
        Log.d(TAG,"[Enter] getWiFiConfig()")
        val wm:WifiManager= context.getSystemService(Context.WIFI_SERVICE) as WifiManager
        val wifiList= wm.configuredNetworks
        for (item in wifiList){
            Log.d(TAG,"item.SSID: "+item.SSID);
            if(item.SSID == "\""+ssid+"\"") {
                Log.d(TAG,"[Enter] return item")
                return item
            }
        }
        return null
    }

    private fun createTypeProfile(context: Context, wifiInfo: WIFIInfo) {
        Log.d(TAG, "[Enter] createWPAProfile() ssid: "+wifiInfo.ssid)
        val conf = WifiConfiguration()
        conf.SSID = "\"" + wifiInfo.ssid + "\""
        configByWiFiType(wifiInfo, conf)
        val wm = context.getSystemService(Context.WIFI_SERVICE) as WifiManager
        wm.addNetwork(conf)
    }

    private fun configByWiFiType(wifiInfo: WIFIInfo, conf: WifiConfiguration) {
        when (wifiInfo.wifiType) {
            WIFIInfo.WIFIType.wep -> {
                conf.wepKeys[0] = "\"" + wifiInfo.password + "\"";
                conf.wepTxKeyIndex = 0;
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                conf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            }
            WIFIInfo.WIFIType.wpa -> {
                conf.preSharedKey = "\"" + wifiInfo.password + "\"";
            }
            WIFIInfo.WIFIType.open -> {
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            }
        }
    }

}