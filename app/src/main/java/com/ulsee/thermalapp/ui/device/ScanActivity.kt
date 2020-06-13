package com.ulsee.thermalapp.ui.device

import android.app.ProgressDialog
import android.content.Context
import android.content.Intent
import android.net.wifi.WifiManager
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.google.android.gms.vision.CameraSource
import com.google.android.gms.vision.Detector
import com.google.android.gms.vision.barcode.Barcode
import com.google.android.gms.vision.barcode.BarcodeDetector
import com.google.zxing.integration.android.IntentIntegrator
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import io.realm.Realm
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

class ScanActivity : AppCompatActivity() {

    val TAG = "ScanActivity"

    lateinit var cameraSource: CameraSource
    lateinit var barcodeDetector : BarcodeDetector
    var mScannedDeviceList = ArrayList<Device>() // IP, ID, Timestamp
    lateinit var mSearchingDeviceProgressDialog : ProgressDialog
    var mSearchingDeviceID = ""

    enum class Status {
        scanningQRCode, searchingDevice, askingName
    }
    var mStatus = Status.scanningQRCode

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_scan)

        mSearchingDeviceProgressDialog = ProgressDialog(this)
        mSearchingDeviceProgressDialog.setMessage("無法與此裝置連線...請確認裝置網路狀況...")

        initZxingScanner()
        // initQRCodeScanner()

        Service.shared.mDeviceSearchedListener = mOnDeviceSearchedListener
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
                    this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.dismiss(); askDeviceName(device) }
                }
            }
        }
    }

    private fun initZxingScanner () {
        IntentIntegrator(this).initiateScan()
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
            .setRequestedPreviewSize(1000, 1000).build()

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
            message = "此裝置已掃描過，將複寫手機中的設定"
            input.setText(duplicatedDevice.getName())
        }

        AlertDialog.Builder(ctx)
            .setTitle("請輸入裝置名稱")
            .setMessage(message)
            .setView(input)
            .setPositiveButton("Save"
            ) { dialog, whichButton ->
                val deviceName = input.text.toString()
                if (deviceName.isEmpty()) {
                    Toast.makeText(ctx, "請輸入裝置名稱!", Toast.LENGTH_SHORT).show()
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
                dialog.dismiss()
            }
            .create()
            .show()
    }

    private fun saveDevice (obj: Device, isDuplicated: Boolean) {
        val realm = Realm.getDefaultInstance()
        realm.beginTransaction()
        val device: Device = if(isDuplicated) realm.where(Device::class.java).equalTo("mID", obj.getID()).findFirst()!! else realm.createObject(Device::class.java)
        device.setID(obj.getID())
        device.setName(obj.getName())
        device.setIP(obj.getIP())
        device.setCreatedAt(obj.getCreatedAt())
        realm.commitTransaction()
    }

    private fun goMain () {
        startActivity(Intent(this, MainActivity::class.java))
        finish()
    }
}