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
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.model.Device
import io.realm.Realm
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

class ScanActivity : AppCompatActivity() {

    val TAG = "ScanActivity"

    lateinit var cameraSource: CameraSource
    lateinit var barcodeDetector : BarcodeDetector
    var mUDPSocket = DatagramSocket()
    var mBroadcaseSendCounter = 1 // 數到0就送出
    var mBroadcaseSendInterval = 3 // 數幾下才送出，平常是3，已經掃到有效的QRCode時是1
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

        val surfaceView = findViewById<View>(R.id.surfaceView) as SurfaceView

        mSearchingDeviceProgressDialog = ProgressDialog(this)
        mSearchingDeviceProgressDialog.setMessage("無法與此裝置連線...請確認裝置網路狀況...")

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
                    Log.i(TAG, "qrcode scanned: "+qrCode)

                    val isValidQRCode = qrCode?.startsWith("ULSEE")
                    if(!isValidQRCode!!) {
                        this@ScanActivity.runOnUiThread { Toast.makeText(this@ScanActivity, "此QRCode無效!!", Toast.LENGTH_SHORT).show() }
                        return
                    }

                    val deviceID = qrCode.substring(5)
                    val idx = mScannedDeviceList.indexOfFirst { it.getID().equals(deviceID) }
                    val isDeviceAlreadyScanned = idx >= 0
                    if (isDeviceAlreadyScanned) {
                        mStatus = Status.askingName
                        this@ScanActivity.runOnUiThread { askDeviceName(mScannedDeviceList[idx]) }
                    } else {
                        this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.show() }
                        mSearchingDeviceID = deviceID
                        mBroadcaseSendInterval = 1
                        mStatus = Status.searchingDevice
                    }
                }
            }
        })

        keepSendUDPBroadcast()
    }

    override fun onDestroy() {
        if (!mUDPSocket.isClosed)mUDPSocket.close()
        super.onDestroy()
    }

    private fun askDeviceName (device: Device) {
        val ctx = this

        val input = EditText(ctx)

        AlertDialog.Builder(ctx)
            .setMessage("請輸入裝置名稱")
            .setView(input)
            .setPositiveButton("Save"
            ) { dialog, whichButton ->
                val deviceName = input.text.toString()
                if (deviceName.isEmpty()) {
                    Toast.makeText(ctx, "請輸入裝置名稱!", Toast.LENGTH_SHORT).show()
                } else {
                    device.setName(deviceName)
                    saveDevice(device)
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

    private fun saveDevice (obj: Device) {
        val realm = Realm.getDefaultInstance()
        realm.beginTransaction()
        val device: Device = realm.createObject(Device::class.java)
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

    fun getBroadcastAddress(): InetAddress? {
        val wifi = getApplicationContext().getSystemService(Context.WIFI_SERVICE) as WifiManager
        val dhcp = wifi.dhcpInfo
        // handle null somehow
        val broadcast = dhcp.ipAddress and dhcp.netmask or dhcp.netmask.inv()
        val quads = ByteArray(4)
        for (k in 0..3) quads[k] = (broadcast shr k * 8 and 0xFF).toByte()
        return InetAddress.getByAddress(quads)
    }

    // 每3秒傳送廣播，如果掃到qrcode,無法匹配，跳出progress表示無法連線，並改為每1秒傳送
    private fun keepSendUDPBroadcast() {
        // TODO: 討論傳送的訊息
        val message = "ULSEE-Thermal-Searching"
        val BROADCAST_PORT = 8829

        mUDPSocket.broadcast = true
        val sendData: ByteArray = message.toByteArray()
        val sendPacket = DatagramPacket(
            sendData,
            sendData.size,
            getBroadcastAddress(),
            BROADCAST_PORT
        )

        // 1. keep send
        Thread(Runnable {
            while (!mUDPSocket.isClosed and !isFinishing) {
                try {
                    if(--mBroadcaseSendCounter==0) {
                        mUDPSocket.send(sendPacket)
                        mBroadcaseSendCounter = mBroadcaseSendInterval
                    }
                    Thread.sleep(1000)
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }
        }).start()

        // 2. keep recv
        Thread(Runnable {
            val lMsg = ByteArray(4096)
            val dp = DatagramPacket(lMsg, lMsg.size)
            try {
                while (!mUDPSocket.isClosed and !isFinishing) {
                    mUDPSocket.receive(dp);
                    val receivedMessage = String(lMsg, 0, dp.length)
                    Log.i(javaClass.name, "got: "+receivedMessage)

                    val isValidDeviceResponse = receivedMessage.startsWith("ULSEE") // TODO: validate message
                    if (isValidDeviceResponse) {
                        val deviceID = receivedMessage.substring(5)
                        val idx = mScannedDeviceList.indexOfFirst { it.getID() == deviceID }
                        if (idx >= 0) {
                            mScannedDeviceList[idx].setCreatedAt(System.currentTimeMillis())
                        } else {
                            val device = Device()
                            device.setID(deviceID)
                            device.setIP(dp.address.hostAddress)
                            device.setCreatedAt(System.currentTimeMillis())
                            mScannedDeviceList.add(device)

                            if (mStatus == Status.searchingDevice) {
                                if (deviceID == mSearchingDeviceID) {
                                    // 找到了
                                    this@ScanActivity.runOnUiThread { mSearchingDeviceProgressDialog.dismiss(); askDeviceName(device) }
                                }
                            }
                        }
                    }
                }
                Log.i(javaClass.name, "Broadcast packet sent to: " + getBroadcastAddress()?.hostAddress)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }).start()
    }

}