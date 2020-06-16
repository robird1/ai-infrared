package com.ulsee.thermalapp.ui.network

import android.app.Activity
import android.content.Context
import android.net.wifi.WifiConfiguration
import android.net.wifi.WifiManager
import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.WIFIInfo
import com.ulsee.thermalapp.data.services.SettingsServiceTCP

class SettingsActivity : AppCompatActivity() {

    var mDeviceID = ""

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_network_settings)

        if (!intent.hasExtra("wifi")) {
            Toast.makeText(this, "Error: no wifi specified", Toast.LENGTH_LONG).show()
            finish()
            return
        }
        if (!intent.hasExtra("device")) {
            Toast.makeText(this, "Error: no device specified", Toast.LENGTH_LONG).show()
            finish()
            return
        }
        mDeviceID = intent.getStringExtra("device")

        val wifiInfo = intent.getSerializableExtra("wifi") as WIFIInfo

        var failedTimes = 0
        if (connect(wifiInfo)) {
            Thread(Runnable {
                for(deviceManager in Service.shared.deviceManagerList) {
                    val deviceManager = Service.shared.getManagerOfDeviceID(mDeviceID)
                    if (deviceManager != null) {
                        SettingsServiceTCP(deviceManager!!).ackWIFI().subscribe( {
                            setResult(RESULT_OK)
                            Toast.makeText(this, "Succeed switch WIFI!!", Toast.LENGTH_LONG).show()
                            finish()
                        }, {
                            failedTimes += 1
                            if (failedTimes >= 30) {
                                setResult(Activity.RESULT_FIRST_USER)
                                it.printStackTrace()
                                Toast.makeText(this, "Error to switch to wifi (ACK): "+it.message, Toast.LENGTH_LONG).show()
                                finish()
                            }
                        })
                    }
                }
                Thread.sleep(1000)
            }).start()
        }
    }

    fun connect (wifiInfo: WIFIInfo) : Boolean {
        val networkSSID = wifiInfo.ssid
        val networkPass = wifiInfo.password

        val conf = WifiConfiguration()
        conf.SSID = "\"" + networkSSID + "\""

        val wifiType = wifiInfo.wifiType

        when(wifiType) {
            WIFIInfo.WIFIType.wep -> {
                conf.wepKeys[0] = "\"" + networkPass + "\"";
                conf.wepTxKeyIndex = 0;
                conf.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
                conf.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
            }
            WIFIInfo.WIFIType.wpa -> {
                conf.preSharedKey = "\""+ networkPass +"\"";
            }
            WIFIInfo.WIFIType.open -> {
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
                val result = wifiManager.reconnect()
                if (result) {
                    return true
                } else {
                    setResult(Activity.RESULT_FIRST_USER)
                }
            }
        }
        return false
    }
}