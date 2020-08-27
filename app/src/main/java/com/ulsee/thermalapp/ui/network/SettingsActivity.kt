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

private val TAG = "SettingsActivity"

const val MAX_TRY_CONNECT_TIMES = 30

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
        if (!intent.hasExtra("old_ip")) {
            Toast.makeText(this, "Error: no old ip specified", Toast.LENGTH_LONG).show()
            finish()
            return
        }
        mDeviceID = intent.getStringExtra("device")


//        var tryTimes = 0
//        var sendTimes = 0
//        val MAX_TRY_CONNECT_TIMES = 30
//        var onceACKSent = false
//        if (connect(wifiInfo)) {
//            val deviceManager = Service.shared.getManagerOfDeviceID(mDeviceID)
//            Thread(Runnable {
//                deviceManager!!.tcpClient.close()
//                while(tryTimes <= MAX_TRY_CONNECT_TIMES && !onceACKSent && !SettingsActivity@this.isFinishing) {
//                    Log.i(javaClass.name, "try connect , isConnected = "+deviceManager!!.tcpClient.isConnected()+", times="+tryTimes)
//                    if (deviceManager.tcpClient.isConnected()) {
//                        SettingsServiceTCP(deviceManager!!).ackWIFI().subscribe( {
//                            onceACKSent = true
//                            Log.i(javaClass.name, "ACK sent!!")
//                            tryTimes += 1
//                            sendTimes += 1
//                            endWithSuccess()
//                            //if (sendTimes >= 5) endWithSuccess()
//                        }, {
//                            tryTimes += 1
//                            Log.i(javaClass.name, "failed to ack "+tryTimes)
//                            if (tryTimes >= MAX_TRY_CONNECT_TIMES) {
//                                if (onceACKSent) {
//                                    endWithSuccess()
//                                } else {
//                                    endWithError(it)
//                                }
//                            }
//                            deviceManager.tcpClient.reconnect()
//                        })
//                    } else {
//                        tryTimes += 1
//                        if (tryTimes >= MAX_TRY_CONNECT_TIMES) {
//                            if (onceACKSent) {
//                                endWithSuccess()
//                            } else {
//                                endWithError(null)
//                            }
//                            break
//                        }
//                    }
//                    Thread.sleep(1000)
//                }
//            }).start()
//        }
        switchAndSendACK()
    }

    /**
     * The UDP Service will try to find the device and reconnect the device again (i.e. close the original socket and reset IP)
     * when the network of the cell phone is successfully switched,
     */
    private fun switchAndSendACK() {
        val wifiInfo = intent.getSerializableExtra("wifi") as WIFIInfo
        var tryTimes = 0
        val result = NetworkUtils.connect(this, wifiInfo)
        if (result) {
            val deviceManager = Service.shared.getManagerOfDeviceID(mDeviceID)
            Thread(Runnable {
                while (tryTimes <= MAX_TRY_CONNECT_TIMES && !SettingsActivity@ this.isFinishing) {
                    Log.d(TAG, "try connect , isConnected = " + deviceManager!!.tcpClient.isConnected() + ", times=" + tryTimes)

//                    if (deviceManager.tcpClient.isConnected()) {
                    if (isStateCorrect()) {

                        sendACK(tryTimes)

                    } else {
                        if (tryTimes >= MAX_TRY_CONNECT_TIMES) {
                            endWithError(null)
                        }
                    }
                    tryTimes += 1
                    Thread.sleep(1000)
                }
            }).start()
        } else {
            Log.d(TAG, "wifiManager.getConfiguredNetworks() is empty.")
            Toast.makeText(this, "Error to switch to Wi-Fi", Toast.LENGTH_LONG).show()
            finish()
        }
    }

    private fun isStateCorrect(): Boolean {
        val deviceManager = Service.shared.getManagerOfDeviceID(mDeviceID)
        val specifiedIP = deviceManager!!.tcpClient.ip
        val oldIP = intent.getStringExtra("old_ip")
        Log.d(TAG, "old ip: $oldIP specified ip: $specifiedIP")

        return (specifiedIP != oldIP) && deviceManager.tcpClient.isConnected()
    }

    private fun sendACK(tryTimes: Int){
        Log.d(TAG, "[Enter] sendACK")
        val deviceManager = Service.shared.getManagerOfDeviceID(mDeviceID)
        SettingsServiceTCP(deviceManager!!).ackWIFI().subscribe( {
            Log.d(TAG, "[Enter] endWithSuccess")
            endWithSuccess()

        }, {
            Log.d(TAG, "[onError] ackWIFI it.message: "+it.message)

            if (tryTimes >= MAX_TRY_CONNECT_TIMES) {
                endWithError(it)
            }
//            deviceManager.tcpClient.reconnect()
        })
    }

    fun endWithSuccess () {
        this.runOnUiThread {
            setResult(RESULT_OK)
            Toast.makeText(this, "Succeed switch Wi-Fi!!", Toast.LENGTH_LONG).show()
            Log.d(TAG, "Succeed switch Wi-Fi!!")
            finish()
        }
    }

    fun endWithError (exception: Throwable?) {
        setResult(Activity.RESULT_FIRST_USER)
        exception?.printStackTrace()
        runOnUiThread{
            if (exception != null) {
                Toast.makeText(this, "Error to switch to Wi-Fi (ACK): "+exception!!.message, Toast.LENGTH_LONG).show()
                Log.d(TAG, "Error to switch to Wi-Fi (ACK): "+exception!!.message)
            } else {
                Toast.makeText(this, "Error to switch to Wi-Fi (ACK) ", Toast.LENGTH_LONG).show()
                Log.d(TAG, "Error to switch to Wi-Fi (ACK) ")
            }
            finish()
        }
    }

}