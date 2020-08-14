package com.ulsee.thermalapp.ui.network

import android.app.Activity
import android.app.ProgressDialog
import android.content.*
import android.location.LocationManager
import android.net.wifi.WifiManager
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import android.util.Log
import android.view.View
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.google.android.gms.common.api.ApiException
import com.google.android.gms.common.api.ResolvableApiException
import com.google.android.gms.location.LocationRequest
import com.google.android.gms.location.LocationServices
import com.google.android.gms.location.LocationSettingsRequest
import com.google.android.gms.location.LocationSettingsStatusCodes
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.WIFIInfo
import com.ulsee.thermalapp.data.request.SetWIFI
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.data.services.SettingsServiceTCP
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport

const val REQUEST_LOCATION_SETTINGS = 5678

class WIFIListActivity : AppCompatActivity() {

    val REQUEST_CODE_SWITCH_WIFI = 1234

    lateinit var recyclerView : RecyclerView
    lateinit var swipeRefreshLayout: SwipeRefreshLayout
    lateinit var mProgressDialog : ProgressDialog
    var deviceID = ""
    var mDeviceManager : DeviceManager? = null
    private lateinit var mProgressView: ConstraintLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_network_wifilist)

        deviceID = intent.getStringExtra("device")
        mDeviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        if (mDeviceManager == null) {
            Toast.makeText(this, "Error: device not found", Toast.LENGTH_LONG).show()
            Log.d("WIFIListActivity", "[onCreate] Error: device not found")
            finish()
            return
        }

        mProgressDialog = ProgressDialog(this)
        mProgressDialog.setMessage("connecting...")

        initRecyclerView()
        registerWIFIBroadcast()
        loadWIFIList()

        findViewById<View>(R.id.button_reset).setOnClickListener { resetToHostspot() }
    }

    override fun onDestroy() {
        unregisterWIFIBroadcast()
        super.onDestroy()
    }

    private fun resetToHostspot () {
        SettingsServiceTCP(mDeviceManager!!).switchWIFI(null, null).subscribe( {
            Toast.makeText(this, getString(R.string.switch_to_hotspot_successfully), Toast.LENGTH_LONG).show()
            Log.d("WIFIListActivity", "[resetToHostspot] "+ getString(R.string.switch_to_hotspot_successfully))

            finish()
        }, {
            it.printStackTrace()
            Toast.makeText(this, "Error to switch to Wi-Fi: "+it.message, Toast.LENGTH_LONG).show()
            Log.d("WIFIListActivity", "[resetToHostspot] "+ "Error to switch to Wi-Fi: "+it.message)

        })
    }

    private fun initRecyclerView () {
        swipeRefreshLayout = findViewById<SwipeRefreshLayout>(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadWIFIList(true) }

        recyclerView = findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = WIFIListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(this)
        mProgressView = findViewById(R.id.progress_view)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, view ->
            val wifiInfo = (recyclerView.adapter as WIFIListAdapter).getList()[position]
            Log.d("WIFIListActivity", "wifiInfo.ssid: "+ wifiInfo.ssid+ " wifiInfo.bssid: "+ wifiInfo.bssid+ " wifiInfo.password: "+ wifiInfo.password+ " wifiInfo.security: "+ wifiInfo.security)
            if(!wifiInfo.passwordRequired) {
                connectToWIFI(wifiInfo, null, null)
            } else {
                askPassword(wifiInfo)
            }
        }
    }

    private fun connectToWIFI(wifiInfo: WIFIInfo, password: String?, advanceParameter: SetWIFI.AdvanceParameter?) {
        wifiInfo.password = password
        mProgressDialog.show()
        SettingsServiceTCP(mDeviceManager!!).switchWIFI(wifiInfo, advanceParameter).subscribe( {
            Log.d("WIFIListActivity", "[Enter] emitter.onComplete() -> switchToWIFI(wifiInfo)")
            switchToWIFI(wifiInfo)
        }, {
            it.printStackTrace()
            Log.d("WIFIListActivity", "[connectToWIFI] emitter.onError() it.message: "+it.message)
            Toast.makeText(this, "Error to switch to Wi-Fi: "+it.message, Toast.LENGTH_LONG).show()
            mProgressDialog.dismiss()
        })
    }

    private fun switchToWIFI(wifiInfo: WIFIInfo) {
        val intent = Intent(this, SettingsActivity::class.java)
        intent.putExtra("wifi", wifiInfo)
        intent.putExtra("device", deviceID)
        intent.putExtra("old_ip", mDeviceManager!!.tcpClient.ip)
        startActivityForResult(intent, REQUEST_CODE_SWITCH_WIFI)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == REQUEST_CODE_SWITCH_WIFI) {
            mProgressDialog.dismiss()
            if (resultCode == RESULT_OK) {
                //startActivity(Intent(this, MainActivity::class.java))
                Log.d("WIFIListActivity", "[onActivityResult] RESULT_OK")
                finish()
            } else if (resultCode != Activity.RESULT_CANCELED) {
                Toast.makeText(this, "Error: failed to switch to specified Wi-Fi", Toast.LENGTH_LONG).show()
                Log.d("WIFIListActivity", "[onActivityResult] Error: failed to switch to specified Wi-Fi")
            } else {
                Toast.makeText(this, "Error: failed to set specified Wi-Fi", Toast.LENGTH_LONG).show()
                Log.d("WIFIListActivity", "[onActivityResult] Error: failed to set specified Wi-Fi")
            }
        } else if (requestCode == REQUEST_LOCATION_SETTINGS) {
            if (resultCode == RESULT_OK) {
                loadWIFIList()
            } else {
                finish()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun askPassword (wifiInfo: WIFIInfo) {
        val ctx = this

        val input = EditText(ctx)

        AlertDialog.Builder(ctx)
            .setTitle(wifiInfo.ssid)
            .setMessage(getString(R.string.hint_input_wifi_password))
            .setView(input)
            .setPositiveButton("Submit"
            ) { dialog, whichButton ->
                val result = input.text.toString()
                if (result.isEmpty()) {
                    Toast.makeText(ctx, getString(R.string.hint_input_wifi_password), Toast.LENGTH_SHORT).show()
                } else {
                    connectToWIFI(wifiInfo, result, null)
                }
            }
            .setNegativeButton("Cancel"
            ) { dialog, whichButton ->
                dialog.dismiss()
            }
            .create()
            .show()
    }

    val wifiScanReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            val success = intent.getBooleanExtra(WifiManager.EXTRA_RESULTS_UPDATED, false)
            swipeRefreshLayout.isRefreshing = false
            if (success) {
//                Log.d("WIFIListActivity", "[wifiScanReceiver.onReceive] Sucess! WifiManager.EXTRA_RESULTS_UPDATED")

                val wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
                val results = wifiManager.scanResults
                var wifiInfoList = ArrayList<WIFIInfo>()
                for (result in results) {
                    if (result.BSSID == wifiManager.connectionInfo.bssid) {
                        val level = WifiManager.calculateSignalLevel(wifiManager.connectionInfo.rssi, result.level)
                        val difference = level * 100 / result.level
                        var signalStrangth = 0
                        if (difference >= 100) signalStrangth =
                            4 else if (difference >= 75) signalStrangth =
                            3 else if (difference >= 50) signalStrangth =
                            2 else if (difference >= 25) signalStrangth = 1

                    }
                    if(result.SSID.isNullOrEmpty()) continue
                    var wifiInfo = WIFIInfo()
                    wifiInfo.ssid = result.SSID
                    wifiInfo.bssid = result.BSSID

                    val level = WifiManager.calculateSignalLevel(result.level, 5)
                    when(level) {
                        0->wifiInfo.bars = "____"
                        1->wifiInfo.bars = "▂___"
                        2->wifiInfo.bars = "▂▄__"
                        3->wifiInfo.bars = "▂▄▆_"
                        4->wifiInfo.bars = "▂▄▆█"
                        else->wifiInfo.bars = "____"
                    }

                    wifiInfo.capabilities = result.capabilities
                    Log.i(javaClass.name, String.format("got Wi-Fi, ssid=%s, bssid=%s, capabilities=%s", result.SSID, result.BSSID, result.capabilities))
                    wifiInfoList.add(wifiInfo)
//                    Log.d("WIFIListActivity", String.format("got Wi-Fi, ssid=%s, bssid=%s, capabilities=%s security=%s passwordRequired=%s", result.SSID, result.BSSID, result.capabilities))
//                    " wifiInfo.security: "+ wifiInfo.security+ " wifiInfo.passwordRequired: "
                }
                (recyclerView.adapter as WIFIListAdapter).setList(wifiInfoList)
                if (results.size == 0) {
                    Toast.makeText(this@WIFIListActivity, "There is no Wi-Fi scanned", Toast.LENGTH_SHORT).show()
                    Log.d("WIFIListActivity", "[wifiScanReceiver.onReceive] There is no Wi-Fi scanned")
//                    finish()
                }
            } else {
                Toast.makeText(this@WIFIListActivity, "Failed to scan Wi-Fi", Toast.LENGTH_LONG).show()
                Log.d("WIFIListActivity", "[wifiScanReceiver.onReceive] Failed to scan Wi-Fi")
            }

            mProgressView.visibility = View.INVISIBLE

        }
    }

    private fun registerWIFIBroadcast () {
        val intentFilter = IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)
        registerReceiver(wifiScanReceiver, intentFilter)
    }

    private fun unregisterWIFIBroadcast () {
        unregisterReceiver(wifiScanReceiver)
    }

    private fun loadWIFIList(isSwipeRefresh: Boolean = false) {
        if (!isSwipeRefresh)
            mProgressView.visibility = View.VISIBLE

        checkLocationSetting()

        val wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
        val success = wifiManager.startScan()
        if (!success) {
            swipeRefreshLayout.isRefreshing = false
            mProgressView.visibility = View.INVISIBLE
            Toast.makeText(this, "Failed to scan Wi-Fi", Toast.LENGTH_LONG).show()
            Log.d("WIFIListActivity", "[loadWIFIList] Failed to scan Wi-Fi")
        }
    }

    fun isLocationEnabled(): Boolean {
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            // This is new method provided in API 28
            val lm = getSystemService(LOCATION_SERVICE) as LocationManager
            lm.isLocationEnabled
        } else {
            // This is Deprecated in API 28
            val mode = Settings.Secure.getInt(this.contentResolver, Settings.Secure.LOCATION_MODE, Settings.Secure.LOCATION_MODE_OFF);
            (mode != Settings.Secure.LOCATION_MODE_OFF);
        }
    }

    fun checkLocationSetting() {
        val mLocationRequest = LocationRequest.create()
            .setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY)
            .setInterval(10 * 1000)
            .setFastestInterval(1 * 1000)

        val settingsBuilder = LocationSettingsRequest.Builder().addLocationRequest(mLocationRequest)
        settingsBuilder.setAlwaysShow(true)

        val result = LocationServices.getSettingsClient(this).checkLocationSettings(settingsBuilder.build())

        result.addOnCompleteListener {
            try {
                val response = it.getResult(ApiException::class.java)
                Log.d("WIFIListActivity", "response: "+ response.toString())

            } catch (ex: ApiException) {
                when (ex.statusCode) {
                    LocationSettingsStatusCodes.RESOLUTION_REQUIRED -> {
                        Log.d("WIFIListActivity","Location settings are not satisfied. Show the user a dialog to upgrade location settings.")
                        try {
                            val resolvableApiException = ex as ResolvableApiException
                            resolvableApiException.startResolutionForResult(this, REQUEST_LOCATION_SETTINGS)

                        } catch (e: IntentSender.SendIntentException) {
                            Log.d("WIFIListActivity", "PendingIntent unable to execute request.")
                            Toast.makeText(this, "Failed to switch Wi-Fi...", Toast.LENGTH_LONG).show()
                            finish()
                        }
                    }
                    LocationSettingsStatusCodes.SETTINGS_CHANGE_UNAVAILABLE -> {
                        Log.d("WIFIListActivity", "Location settings are inadequate, and cannot be fixed here. Dialog not created.")
                        Toast.makeText(this, "Failed to switch Wi-Fi...", Toast.LENGTH_LONG).show()
                        finish()
                    }
                }
            }
        }

    }

}