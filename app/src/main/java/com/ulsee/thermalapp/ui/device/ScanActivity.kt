package com.ulsee.thermalapp.ui.device

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.widget.EditText
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.google.android.gms.vision.CameraSource
import com.google.android.gms.vision.Detector
import com.google.android.gms.vision.barcode.Barcode
import com.google.android.gms.vision.barcode.BarcodeDetector
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Device
import io.realm.Realm
import io.realm.kotlin.where

class ScanActivity : AppCompatActivity() {

    val TAG = "ScanActivity"

    lateinit var cameraSource: CameraSource
    lateinit var barcodeDetector : BarcodeDetector
    var isAskingName = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_scan)

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
                if (isAskingName) return

                val qrCodeList = detections?.detectedItems
                if (qrCodeList != null && qrCodeList.size() != 0) {
                    val qrCode = qrCodeList.valueAt(0)?.displayValue
                    Log.i(TAG, "qrcode scanned: "+qrCode)

                    // todo: validate qrcode
                    // todo: validate device connection

                    isAskingName = true
                    this@ScanActivity.runOnUiThread { if (qrCode == null)askDeviceName("") else askDeviceName(qrCode) }
                }
            }
        })
    }

    private fun askDeviceName (qrCode: String) {
        val ctx = this

        val input = EditText(ctx)

        AlertDialog.Builder(ctx)
            .setMessage("請輸入裝置名稱")
            .setView(input)
            .setPositiveButton("Save"
            ) { dialog, whichButton ->
                val deviceName = input.text.toString()
                saveDevice(qrCode, deviceName, qrCode)
                goMain()
            }
            .setNegativeButton("Cancel"
            ) { dialog, whichButton ->
                dialog.dismiss()
            }
            .create()
            .show()
    }

    private fun saveDevice (id: String, name: String, ip: String) {
        val realm = Realm.getDefaultInstance()
        realm.beginTransaction()
        val device: Device = realm.createObject(Device::class.java)
        device.setID(id)
        device.setName(name)
        device.setID(ip)
        realm.commitTransaction()
    }

    private fun goMain () {
        startActivity(Intent(this, MainActivity::class.java))
        finish()
    }
}