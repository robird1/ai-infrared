package com.ulsee.thermalapp.ui.network

import android.app.Activity
import android.content.Context
import android.content.IntentSender
import android.location.LocationManager
import android.net.wifi.WifiConfiguration
import android.net.wifi.WifiManager
import android.os.Build
import android.provider.Settings
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.google.android.gms.common.api.ApiException
import com.google.android.gms.common.api.ResolvableApiException
import com.google.android.gms.location.LocationRequest
import com.google.android.gms.location.LocationServices
import com.google.android.gms.location.LocationSettingsRequest
import com.google.android.gms.location.LocationSettingsStatusCodes
import com.ulsee.thermalapp.data.model.WIFIInfo

private val TAG = "NetworkUtils"

class NetworkUtils {
    companion object {
        const val REQUEST_LOCATION_SETTINGS = 5678

        fun isLocationEnabled(context: Context): Boolean {
            return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                // This is new method provided in API 28
                val lm = context.getSystemService(AppCompatActivity.LOCATION_SERVICE) as LocationManager
                lm.isLocationEnabled
            } else {
                // This is Deprecated in API 28
                val mode = Settings.Secure.getInt(context.contentResolver, Settings.Secure.LOCATION_MODE, Settings.Secure.LOCATION_MODE_OFF);
                (mode != Settings.Secure.LOCATION_MODE_OFF);
            }
        }

        fun checkLocationSetting(activity: Activity) {
            val mLocationRequest = LocationRequest.create()
                .setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY)
                .setInterval(10 * 1000)
                .setFastestInterval(1 * 1000)

            val settingsBuilder = LocationSettingsRequest.Builder().addLocationRequest(mLocationRequest)
            settingsBuilder.setAlwaysShow(true)

            val result = LocationServices.getSettingsClient(activity).checkLocationSettings(settingsBuilder.build())

            result.addOnCompleteListener {
                try {
                    val response = it.getResult(ApiException::class.java)
                    Log.d(TAG, "response: "+ response.toString())

                } catch (ex: ApiException) {
                    when (ex.statusCode) {
                        LocationSettingsStatusCodes.RESOLUTION_REQUIRED -> {
                            Log.d(TAG,"Location settings are not satisfied. Show the user a dialog to upgrade location settings.")
                            try {
                                val resolvableApiException = ex as ResolvableApiException
                                resolvableApiException.startResolutionForResult(activity, REQUEST_LOCATION_SETTINGS)

                            } catch (e: IntentSender.SendIntentException) {
                                Log.d(TAG, "PendingIntent unable to execute request.")
//                                Toast.makeText(this, "Failed to switch Wi-Fi...", Toast.LENGTH_LONG).show()
//                                finish()
                            }
                        }
                        LocationSettingsStatusCodes.SETTINGS_CHANGE_UNAVAILABLE -> {
                            Log.d(TAG, "Location settings are inadequate, and cannot be fixed here. Dialog not created.")
//                            Toast.makeText(this, "Failed to switch Wi-Fi...", Toast.LENGTH_LONG).show()
//                            finish()
                        }
                    }
                }
            }

        }

        fun connect(context: Context, wifiInfo: WIFIInfo): Boolean {
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
                    wifiManager.disconnect()
                    wifiManager.enableNetwork(i.networkId, true)
                    val result = wifiManager.reconnect()
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
        }
    }

}