package com.ulsee.thermalapp.ui.network

import android.content.Context
import android.net.wifi.WifiConfiguration
import android.net.wifi.WifiManager
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.ulsee.thermalapp.R


class SettingsActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_network_settings)

        connect()
    }

    enum class WIFIType {
        wep, wpa, open
    }

    fun connect () {
        val networkSSID = "ULSEE"
        val networkPass = "ulsee@168"

        val conf = WifiConfiguration()
        conf.SSID = "\"" + networkSSID + "\""

        val wifiType = WIFIType.wep

        when(wifiType) {
            WIFIType.wep -> {
                conf.wepKeys[0] = "\"" + networkPass + "\"";
                conf.wepTxKeyIndex = 0;
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                conf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            }
            WIFIType.wpa -> {
                conf.preSharedKey = "\""+ networkPass +"\"";
            }
            WIFIType.open -> {
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
            }
        }

        val wifiManager = (getApplicationContext().getSystemService(Context.WIFI_SERVICE) as WifiManager)
        wifiManager.addNetwork(conf)

        val list: List<WifiConfiguration> = wifiManager.getConfiguredNetworks()
        for (i in list) {
            if (i.SSID != null && i.SSID == "\"" + networkSSID + "\"") {
                wifiManager.disconnect()
                wifiManager.enableNetwork(i.networkId, true)
                wifiManager.reconnect()
                break
            }
        }
    }
}