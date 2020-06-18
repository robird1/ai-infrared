package com.ulsee.thermalapp.ui.device

import android.app.ProgressDialog
import android.content.Context
import android.content.DialogInterface
import android.content.Intent
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
import com.google.android.gms.vision.CameraSource
import com.google.android.gms.vision.Detector
import com.google.android.gms.vision.barcode.Barcode
import com.google.android.gms.vision.barcode.BarcodeDetector
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.google.zxing.integration.android.IntentIntegrator
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.RealmDevice
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.services.DeviceManager
import io.realm.Realm
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.Socket
import java.net.SocketException

class ScanActivity : AppCompatActivity() {

    val TAG = "ScanActivity"

    lateinit var cameraSource: CameraSource
    lateinit var barcodeDetector : BarcodeDetector
    var mScannedDeviceList = ArrayList<Device>() // IP, ID, Timestamp
    lateinit var mSearchingDeviceProgressDialog : AlertDialog
    var mSearchingDeviceID = ""

    enum class Status {
        scanningQRCode, searchingDevice, askingName
    }
    var mStatus = Status.scanningQRCode

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_scan)

        mSearchingDeviceProgressDialog = AlertDialog
            .Builder(this)
            .setView(ProgressBar(this))
            .setMessage(getString(R.string.unable_to_connect_to_device))
            .setNegativeButton(getString(R.string.cancel), DialogInterface.OnClickListener { dialog, which ->
                dialog.dismiss()
            })
            .setCancelable(false)
            .setOnDismissListener {
                mStatus = ScanActivity.Status.scanningQRCode
                initZxingScanner()
            }
            .create()

        initZxingScanner()
        // initQRCodeScanner()

        Service.shared.mDeviceSearchedListener = mOnDeviceSearchedListener
        keepTryConenctToAPTCP()
    }

    override fun onDestroy() {
        Service.shared.mDeviceSearchedListener = null
        super.onDestroy()
    }

    val mOnDeviceSearchedListener = object : Service.DeviceSearchedListener {
        override fun onNewDevice(device: Device) {
            device.setCreatedAt(System.currentTimeMillis())
            mScannedDeviceList.add(device)
            if (mStatus == ScanActivity.Status.searchingDevice) {
                if (device.getID() == mSearchingDeviceID) {
                    // 找到了
                    mStatus = ScanActivity.Status.askingName
                    this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.dismiss(); askDeviceName(device) }
                }
            }
        }
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
//                    Log.i(TAG, "qrcode scanned: "+qrCode)
//
//                    val isValidQRCode = qrCode?.startsWith("ULSEE")
//                    if(!isValidQRCode!!) {
//                        this@ScanActivity.runOnUiThread { Toast.makeText(this@ScanActivity, "此QRCode無效!!", Toast.LENGTH_SHORT).show() }
//                        return
//                    }
//
//                    val deviceID = qrCode.substring(5)
//                    val idx = mScannedDeviceList.indexOfFirst { it.getID().equals(deviceID) }
//                    val isDeviceAlreadyScanned = idx >= 0
//                    if (isDeviceAlreadyScanned) {
//                        mStatus = Status.askingName
//                        this@ScanActivity.runOnUiThread { askDeviceName(mScannedDeviceList[idx]) }
//                    } else {
//                        this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.show() }
//                        mSearchingDeviceID = deviceID
//                        mBroadcaseSendInterval = 1
//                        mStatus = Status.searchingDevice
//                    }
                }
            }
        })
    }

    private fun processQRCode(qrCode: String) {
        val isValidQRCode = qrCode?.startsWith("ULSEE-")
        if(!isValidQRCode!!) {
            this@ScanActivity.runOnUiThread { Toast.makeText(this@ScanActivity, "此QRCode無效!!", Toast.LENGTH_SHORT).show() }
            return
        }

        val deviceID = qrCode.substring(6)
        val idx = mScannedDeviceList.indexOfFirst { it.getID().equals(deviceID) }
        val isDeviceAlreadyScanned = idx >= 0
        if (isDeviceAlreadyScanned) {
            mStatus = Status.askingName
            this@ScanActivity.runOnUiThread { askDeviceName(mScannedDeviceList[idx]) }
        } else {
            this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.show() }
            mSearchingDeviceID = deviceID
            mStatus = Status.searchingDevice
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
                    saveDevice(device, isDuplicated)
                    AppPreference(getSharedPreferences("app", Context.MODE_PRIVATE)).setOnceCreateFirstDevice()
                    goMain()
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

    private fun saveDevice (obj: Device, isDuplicated: Boolean) {
        val realm = Realm.getDefaultInstance()
        realm.beginTransaction()
        val device: RealmDevice = if(isDuplicated) realm.where(RealmDevice::class.java).equalTo("mID", obj.getID()).findFirst()!! else realm.createObject(RealmDevice::class.java)
        device.setID(obj.getID())
        device.setName(obj.getName())
        device.setIP(obj.getIP())
        device.setCreatedAt(obj.getCreatedAt())
        realm.commitTransaction()

        if (!isDuplicated) {
            Log.i(javaClass.name, "found new joined device: "+device.getID())
            Service.shared.justJoinedDeviceIDList.add(device.getID())
        }
    }

    private fun goMain () {
        startActivity(Intent(this, MainActivity::class.java))
        finish()
    }

    private fun getLocalIP () : String {
        val wm = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
        val ip: String = Formatter.formatIpAddress(wm.connectionInfo.ipAddress)
        return ip
    }

    private fun getAPTCPIP () : String {
        var localIP = getLocalIP()
        var arr = localIP.split(".")
        return String.format("%s.%s.%s.1",arr[0],arr[1],arr[2])
    }

    private fun keepTryConenctToAPTCP () {
        var TCPClientSocket : Socket? = null
        val gson = Gson()
        Thread(Runnable {
            while(!isFinishing) {
                try {
                    // 1. get ip
                    val ip = getAPTCPIP()
                    // 2. try connect
                    TCPClientSocket = Socket(ip, DeviceManager.TCP_PORT)
                    // 3. try to get settings ID
                    val bufferedReader = BufferedReader(InputStreamReader(TCPClientSocket!!.getInputStream()))
                    var buffer = CharArray(4096)
                    val readLen = bufferedReader.read(buffer, 0, buffer.size)
                    val responseString = String(buffer, 0, readLen)
                    Log.i(javaClass.name, "try to connect to IP.1, received:")
                    Log.i(javaClass.name, responseString)
                    // 4. try to get settings
                    val itemType = object : TypeToken<Settings>() {}.type
                    val settings = gson.fromJson<Settings>(responseString, itemType)
                    // 5. check device
                    if (mStatus == ScanActivity.Status.searchingDevice) {
                        if (settings.ID == mSearchingDeviceID) {
                            val device = Device()
                            device.setID(settings.ID)
                            device.setIP(ip)
                            device.setCreatedAt(System.currentTimeMillis())
                            // 找到了
                            mStatus = ScanActivity.Status.askingName
                            this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.dismiss(); askDeviceName(device) }
                        }
                    }
                    Thread.sleep(1000)
                } catch(e: SocketException) {
                    Log.i(javaClass.name, "try to connect to IP.1 (AP TCP), but failed, isn't AP mode")
//                    e.printStackTrace()
                } catch(e: java.net.ConnectException) {
                    // ignore connection error
                } catch(e: Exception) {
                    e.printStackTrace()
                } finally {
                    TCPClientSocket?.close()
                    TCPClientSocket = null
                }
            }
        }).start()
    }
}