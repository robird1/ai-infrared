package com.ulsee.thermalapp.ui.device

import android.app.Activity
import android.content.*
import android.net.wifi.WifiConfiguration
import android.net.wifi.WifiManager
import android.os.Bundle
import android.text.format.Formatter
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.widget.EditText
import android.widget.ProgressBar
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import com.google.android.gms.vision.CameraSource
import com.google.android.gms.vision.Detector
import com.google.android.gms.vision.barcode.Barcode
import com.google.android.gms.vision.barcode.BarcodeDetector
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.google.zxing.integration.android.IntentIntegrator
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.model.WIFIInfo
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.ui.network.NetworkUtils
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.InetAddress
import java.net.InetSocketAddress
import java.net.Socket
import java.net.SocketException

class ScanActivity : AppCompatActivity() {

    val TAG = "ScanActivity"

    lateinit var cameraSource: CameraSource
    lateinit var barcodeDetector : BarcodeDetector
    var mScannedDeviceList = ArrayList<Device>() // IP, ID, Timestamp
    lateinit var mSearchingDeviceProgressDialog : AlertDialog
    var mSearchingDeviceID = ""
    var mAPTCPSocket : Socket? = null
    var mIsConnectedToAPTCP = false
    var mAPSettings : Settings? = null
    var mAPDevice : Device? = null
    private lateinit var mAddDeviceController: AddDeviceController
    private var mWifiList: ArrayList<WIFIInfo>?= null
    private lateinit var mProgressView: ConstraintLayout
    var wifiScanReceiver: BroadcastReceiver?= null

    enum class Status {
        scanningQRCode, searchingDevice, askingName
    }
    var mStatus = Status.scanningQRCode

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_scan)

        val isLocationOn = NetworkUtils.isLocationEnabled(this)
        Log.d(TAG, "isLocationOn: "+ isLocationOn)
        if (isLocationOn) {
            doInit()
        } else {
            NetworkUtils.checkLocationSetting(this)
        }
    }

    private fun doInit() {
        mProgressView = findViewById(R.id.progress_view)
        mSearchingDeviceProgressDialog = AlertDialog
            .Builder(this)
            .setView(ProgressBar(this))
            .setMessage(getString(R.string.unable_to_connect_to_device))
            .setNegativeButton(getString(R.string.cancel), DialogInterface.OnClickListener { dialog, which ->
                dialog.dismiss()
            })
            .setCancelable(false)
            .setOnDismissListener {
                if (mStatus != ScanActivity.Status.askingName) {
                    mStatus = ScanActivity.Status.scanningQRCode
                    initZxingScanner()
                }
            }
            .create()

        registerWIFIBroadcast()

        initZxingScanner()
        // initQRCodeScanner()

        Service.shared.mDeviceSearchedListener = mOnDeviceSearchedListener
        mAddDeviceController = AddDeviceController(this)
        keepTryConenctToAPTCP()
    }

    override fun onDestroy() {
        unregisterWIFIBroadcast()
        Service.shared.mDeviceSearchedListener = null
        super.onDestroy()
    }

    val mOnDeviceSearchedListener = object : Service.DeviceSearchedListener {
        override fun onNewDevice(device: Device) {
            if (!isDeviceDuplicated(device)) {
                device.setCreatedAt(System.currentTimeMillis())
                mScannedDeviceList.add(device)
            }

            if (mStatus == ScanActivity.Status.searchingDevice) {
                if (device.getID() == mSearchingDeviceID) {
                    Log.d(TAG, "[Enter] onNewDevice device.getID() == mSearchingDeviceID")
                    // 找到了
                    mStatus = ScanActivity.Status.askingName
                    this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.dismiss(); askDeviceName(device) }
                }
            }
        }
    }

    private fun isDeviceDuplicated(device: Device): Boolean {
        for (d in mScannedDeviceList) {
            if (d.getID().equals(device.getID()) && d.getIP().equals(device.getIP())) {
                return true
            }
        }
        return false
    }

    private fun initZxingScanner () {
        val integrator = IntentIntegrator(this)
        integrator.setDesiredBarcodeFormats(IntentIntegrator.QR_CODE)
        integrator.setPrompt("Scan device QRCode");
        integrator.initiateScan()
        //IntentIntegrator(this).initiateScan()
    }

    // Get the results:
    override fun onActivityResult(
        requestCode: Int,
        resultCode: Int,
        data: Intent?
    ) {
        if (requestCode == IntentIntegrator.REQUEST_CODE) {
            val result = IntentIntegrator.parseActivityResult(requestCode, resultCode, data)
            if (result != null) {
                if (result.contents == null) {
                    Toast.makeText(this, "Cancelled", Toast.LENGTH_LONG).show()
                    finish()
                    //initZxingScanner()
                } else {
                    processQRCode(result.contents)
                }
            } else {
                super.onActivityResult(requestCode, resultCode, data)
            }
        } else if (requestCode == NetworkUtils.REQUEST_LOCATION_SETTINGS) {
            if (resultCode == RESULT_OK) {
                doInit()
            } else {
                finish()
            }
        }
    }

    private fun initQRCodeScanner () {
        val surfaceView = findViewById<View>(R.id.surfaceView) as SurfaceView

        barcodeDetector = BarcodeDetector.Builder(this)
            .setBarcodeFormats(Barcode.QR_CODE).build()
        cameraSource = CameraSource.Builder(this, barcodeDetector)
            .setAutoFocusEnabled(true)
            .setRequestedPreviewSize(800, 800).build()

        surfaceView.holder.addCallback(object: SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, format: Int,
                                        width: Int, height: Int) {
                cameraSource.start(holder);
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
                //cameraSource.start(holder);
            }

            override fun surfaceDestroyed(holder: SurfaceHolder) {
                cameraSource.stop();
            }
        })

        barcodeDetector.setProcessor(object : Detector.Processor<Barcode?> {
            override fun release() {
                TODO("Not yet implemented")
            }

            override fun receiveDetections(detections: Detector.Detections<Barcode?>?) {
                if (mStatus == Status.askingName) return

                val qrCodeList = detections?.detectedItems
                if (qrCodeList != null && qrCodeList.size() != 0) {
                    val qrCode = qrCodeList.valueAt(0)?.displayValue
                    processQRCode(qrCode!!)
                }
            }
        })
    }

    private fun processQRCode(qrCode: String) {
        val isValidQRCode = qrCode?.startsWith("ULS-")
        if(!isValidQRCode) {
            this@ScanActivity.runOnUiThread {
                Toast.makeText(this@ScanActivity, getString(R.string.qrcode_invalid), Toast.LENGTH_SHORT).show()
                initZxingScanner()
            }
            return
        }

        val deviceID = qrCode
        val idx = mScannedDeviceList.indexOfFirst { it.getID().equals(deviceID) }
        val isDeviceAlreadyScanned = idx >= 0
        if (mIsConnectedToAPTCP && mAPDevice?.getID() == deviceID) {
            mStatus = Status.askingName
            this@ScanActivity.runOnUiThread {
                mSearchingDeviceProgressDialog.dismiss(); askDeviceName(mAPDevice!!)
            }
        } else if (isDeviceAlreadyScanned) {
            mStatus = Status.askingName
            this@ScanActivity.runOnUiThread { askDeviceName(mScannedDeviceList[idx]) }
        } else {
            mSearchingDeviceID = deviceID
            mStatus = Status.searchingDevice

            mProgressView.visibility = View.VISIBLE
            loadWIFIList()
        }
    }

    private fun askDeviceName (device: Device) {
        val ctx = this
        var message : String? = null
        val input = EditText(ctx)

        val duplicatedDeviceIdx = Service.shared.getDeviceList().indexOfFirst { it.getID().equals(device.getID()) }
        val isDuplicated = duplicatedDeviceIdx >= 0

        if(isDuplicated) {
            val duplicatedDevice = Service.shared.getDeviceList()[duplicatedDeviceIdx]
            message = ctx.getString(R.string.hint_device_overwrite)
            input.setText(duplicatedDevice.getName())
        }

        AlertDialog.Builder(ctx)
            .setTitle(ctx.getString(R.string.hint_input_device_name))
            .setMessage(message)
            .setView(input)
            .setPositiveButton("Save"
            ) { dialog, whichButton ->
                val deviceName = input.text.toString()
                if (deviceName.isEmpty()) {
                    Toast.makeText(ctx, ctx.getString(R.string.hint_input_device_name), Toast.LENGTH_SHORT).show()
                } else {
                    device.setName(deviceName)
                    mAddDeviceController.save(device, isDuplicated)
                }
            }
            .setNegativeButton("Cancel"
            ) { dialog, whichButton ->
                mStatus = Status.scanningQRCode
//                dialog.dismiss()
//                initZxingScanner()
            }
            .setCancelable(false)
            .setOnDismissListener {
                if (mStatus != ScanActivity.Status.askingName) {
                    mStatus = Status.scanningQRCode
                    it.dismiss()
                    initZxingScanner()
                }
            }
            .create()
            .show()
    }

    private fun keepTryConenctToAPTCP () {
        val gson = Gson()
        Thread(Runnable {
            while(!isFinishing) {
                try {
                    Thread.sleep(1000)

                    Log.d(TAG, "[Before] Thread.sleep(1000)")
                    if (mIsConnectedToAPTCP) {
                        Thread.sleep(1000)
                        continue
                    }
                    Log.d(TAG, "[After] Thread.sleep(1000)")

                    // 1. get ip
                    val ip = mAddDeviceController.getAPTCPIP()
                    // 2. try connect
                    Log.d(TAG, "ip: "+ip)
                    mAPTCPSocket = Socket(ip, DeviceManager.TCP_PORT)
//                    Log.d(TAG, "local ip: "+InetAddress.getByName(getLocalIP()))
//                    Log.d(TAG, "remote ip: "+InetAddress.getByName(ip))
//                    mAPTCPSocket = Socket(InetAddress.getByName(ip), DeviceManager.TCP_PORT,
//                        InetAddress.getByName(getLocalIP()), 8888)

                    // 3. try to get settings ID
                    val bufferedReader = BufferedReader(InputStreamReader(mAPTCPSocket!!.getInputStream()))
                    var buffer = CharArray(4096)
                    val readLen = bufferedReader.read(buffer, 0, buffer.size)
                    val responseString = String(buffer, 0, readLen)
                    Log.i(javaClass.name, "try to connect to IP.1, received:")
                    Log.i(javaClass.name, responseString)
                    Log.d(TAG, "responseString: "+ responseString)
                    // 4. try to get settings
                    val itemType = object : TypeToken<Settings>() {}.type
                    mAPSettings = gson.fromJson<Settings>(responseString, itemType)

                    mAPDevice = Device()
                    mAPDevice?.setID(mAPSettings!!.ID)
                    mAPDevice?.setIP(ip)
                    mAPDevice?.setCreatedAt(System.currentTimeMillis())
                    mAPDevice?.setIsFRVisible(mAPSettings!!.IsFRVisible)


                    mIsConnectedToAPTCP = true
                    Log.d(TAG, "[Enter] mIsConnectedToAPTCP = true")

                    // 5. check device
                    if (mStatus == ScanActivity.Status.searchingDevice) {
                        Log.d(TAG, "[Enter] mStatus == ScanActivity.Status.searchingDevice")

                        if (mAPSettings?.ID == mSearchingDeviceID) {
                            Log.d(TAG, "[Enter] mAPSettings?.ID == mSearchingDeviceID")
                            // 找到了
                            mStatus = ScanActivity.Status.askingName
                            this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.dismiss(); askDeviceName(mAPDevice!!) }
                        }
                    }
                } catch(e: SocketException) {
//                    Log.i(javaClass.name, "try to connect to IP.1 (AP TCP), but failed, isn't AP mode")
                    Log.d(TAG, "[Enter] SocketException: "+e.message)
                    mAPTCPSocket?.close()
                    mAPTCPSocket = null
                    mIsConnectedToAPTCP = false
                } catch(e: java.net.ConnectException) {
                    Log.d(TAG, "[Enter] ConnectException")

                    // ignore connection error
                    mAPTCPSocket?.close()
                    mAPTCPSocket = null
                    mIsConnectedToAPTCP = false
                } catch(e: Exception) {
                    Log.d(TAG, "[Enter] Exception")

                    e.printStackTrace()
                    mAPTCPSocket?.close()
                    mAPTCPSocket = null
                    mIsConnectedToAPTCP = false
                }
            }
        }).start()
    }

    private fun loadWIFIList() {
        val wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
        val success = wifiManager.startScan()
        if (!success) {
            mProgressView.visibility = View.INVISIBLE
            this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.show() }
            Log.d(TAG, "[Failed] wifiManager.startScan()")
        } else {
            Log.d(TAG, "[Success] wifiManager.startScan()")
        }
    }

    private fun isHotspotExist(): Boolean {
        for (i in mWifiList!!) {
            if (i.ssid.equals(mSearchingDeviceID)) {
                return true
            }
        }
        return false
    }

    private fun switchWifi(): Boolean {
        for (i in mWifiList!!) {
            if (i.ssid.equals(mSearchingDeviceID)) {
                i.password = "ulsee168"
                mIsConnectedToAPTCP = false
                return NetworkUtils.connect(this, i)
            }
        }
        return false
    }

    private fun registerWIFIBroadcast () {
        wifiScanReceiver = initWifiReceiver()
        val intentFilter = IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)
        registerReceiver(wifiScanReceiver, intentFilter)
    }

    private fun unregisterWIFIBroadcast () {
        if (wifiScanReceiver != null)
            unregisterReceiver(wifiScanReceiver)
    }

    private fun initWifiReceiver(): BroadcastReceiver {
        return object: BroadcastReceiver() {
            override fun onReceive(context: Context, intent: Intent) {
                var isSwitchSuccess = false
                val success = intent.getBooleanExtra(WifiManager.EXTRA_RESULTS_UPDATED, false)
                if (success) {
                    val wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
                    val results = wifiManager.scanResults
                    mWifiList = ArrayList<WIFIInfo>()
                    for (result in results) {
                        if(result.SSID.isNullOrEmpty()) continue
                        var wifiInfo = WIFIInfo()
                        wifiInfo.ssid = result.SSID
                        wifiInfo.bssid = result.BSSID
                        wifiInfo.capabilities = result.capabilities
                        mWifiList!!.add(wifiInfo)
//                    Log.d(TAG, String.format("got Wi-Fi, ssid=%s, bssid=%s, capabilities=%s", result.SSID, result.BSSID, result.capabilities))
                    }

                    if (results.size == 0) {
                        Log.d(TAG, "[wifiScanReceiver.onReceive] There is no Wi-Fi scanned")
                    } else {
                        if (isHotspotExist()) {
                            if (switchWifi()) {
                                Log.d(TAG, "[Success] switchWifi()")
                                isSwitchSuccess = true
                            } else {
                                Log.d(TAG, "[Failed] switchWifi()")
                            }
                        } else {
                            Log.d(TAG, "Hotspot is not existing.")
                        }
                    }
                } else {
                    Log.d(TAG, "[wifiScanReceiver.onReceive] Failed to scan Wi-Fi")
                    this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.show() }
                }

                mProgressView.visibility = View.INVISIBLE
                if (!isSwitchSuccess) {
                    this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.show() }
                }
            }
        }
    }

}