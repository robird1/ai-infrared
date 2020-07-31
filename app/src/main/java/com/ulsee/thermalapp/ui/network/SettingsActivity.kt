package com.ulsee.thermalapp.ui.network

import android.app.Activity
import android.content.Context
import android.net.wifi.WifiConfiguration
import android.net.wifi.WifiManager
import android.os.Bundle
import android.util.Log
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
            Toast.makeText(this, "Error: no Wi-Fi specified", Toast.LENGTH_LONG).show()
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

        var tryTimes = 0
        var sendTimes = 0
        val MAX_TRY_CONNECT_TIMES = 30
        var onceACKSent = false
        if (connect(wifiInfo)) {
            val deviceManager = Service.shared.getManagerOfDeviceID(mDeviceID)
            Thread(Runnable {
                deviceManager!!.tcpClient.close()
                while(tryTimes <= MAX_TRY_CONNECT_TIMES && sendTimes< 5 || SettingsActivity@this.isFinishing) {
                    Log.i(javaClass.name, "try connect , isConnected = "+deviceManager!!.tcpClient.isConnected()+", times="+tryTimes)
                    if (deviceManager.tcpClient.isConnected()) {
                        SettingsServiceTCP(deviceManager!!).ackWIFI().subscribe( {
                            onceACKSent = true
                            Log.i(javaClass.name, "ACK sent!!")
                            tryTimes += 1
                            sendTimes += 1
                            endWithSuccess()
                            //if (sendTimes >= 5) endWithSuccess()
                        }, {
                            tryTimes += 1
                            Log.i(javaClass.name, "failed to ack "+tryTimes)
                            if (tryTimes >= MAX_TRY_CONNECT_TIMES) {
                                if (onceACKSent) {
                                    endWithSuccess()
                                } else {
                                    endWithError(it)
                                }
                            }
                            deviceManager.tcpClient.reconnect()
                        })
                    } else {
                        tryTimes += 1
                        if (tryTimes >= MAX_TRY_CONNECT_TIMES) {
                            if (onceACKSent) {
                                endWithSuccess()
                            } else {
                                endWithError(null)
                            }
                            break
                        }
                    }
                    Thread.sleep(1000)
                }
            }).start()
        }
    }

    fun endWithSuccess () {
        this.runOnUiThread {
            setResult(RESULT_OK)
            Toast.makeText(this, "Succeed switch Wi-Fi!!", Toast.LENGTH_LONG).show()
            finish()
        }
    }

    fun endWithError (exception: Throwable?) {
        setResult(Activity.RESULT_FIRST_USER)
        exception?.printStackTrace()
        runOnUiThread{
            if (exception != null) {
                Toast.makeText(this, "Error to switch to Wi-Fi (ACK): "+exception!!.message, Toast.LENGTH_LONG).show()
            } else {
                Toast.makeText(this, "Error to switch to Wi-Fi (ACK) ", Toast.LENGTH_LONG).show()
            }
            finish()
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