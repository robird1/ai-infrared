package com.ulsee.thermalapp.ui.device

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.content.Intent.FLAG_ACTIVITY_CLEAR_TASK
import android.content.Intent.FLAG_ACTIVITY_NEW_TASK
import android.net.wifi.WifiManager
import android.os.Handler
import android.os.HandlerThread
import android.text.format.Formatter
import android.util.Log
import android.widget.EditText
import android.widget.ProgressBar
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
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


class AddDeviceController(activity: Activity) {
    private val mActivity = activity
    private var mConnectHotspotDialog : AlertDialog?= null
    private var mHandler: Handler? = null
    private var mThread: HandlerThread? = null
    private lateinit var mTask: Runnable

    fun execute() {
        AlertDialog.Builder(mActivity)
            .setTitle(R.string.dialog_add_device)
            .setItems(R.array.add_device) { _, which ->
                when (which) {
                    0 -> openScanner()
                    1 -> searchHotspot ()
                    2 -> searchLAN()
                }
            }
            .create()
            .show()
    }

    private fun openScanner() {
        mActivity.startActivity(Intent(mActivity, ScanActivity::class.java))
    }

    private fun searchLAN() {
        mActivity.startActivity(Intent(mActivity, IPCListActivity::class.java))
    }

    private fun searchHotspot() {
        mActivity.startActivity(Intent(mActivity, HotspotActivity::class.java))
    }

    private fun showConnectHotspotDialog() {
        mConnectHotspotDialog = AlertDialog
            .Builder(mActivity)
            .setView(ProgressBar(mActivity))
            .setMessage(R.string.dialog_connect_hotspot)
            .setNegativeButton(mActivity.getString(R.string.cancel)) { dialog, _ ->
                cancelTask()
                dialog.dismiss()
            }
            .setCancelable(false)
            .create()

        mConnectHotspotDialog!!.show()
    }

    private fun initHandler() {
        mThread = HandlerThread("add_device_thread")
        mThread?.start()
        mHandler = Handler(mThread?.looper!!)
    }

    private fun cancelTask() {
        mHandler?.removeCallbacks(mTask)
        mHandler = null
        mThread?.quit()
        mThread = null
    }

    private fun getLocalIP () : String {
        val wm = mActivity.getSystemService(Context.WIFI_SERVICE) as WifiManager
        return Formatter.formatIpAddress(wm.connectionInfo.ipAddress)
    }

    fun getAPTCPIP () : String {
        val localIP = getLocalIP()
        val arr = localIP.split(".")
        return String.format("%s.%s.%s.1",arr[0],arr[1],arr[2])
    }

    fun connectToAPTCP () {
        Log.d("AddDeviceController", "[Enter] connectToAPTCP")
        initHandler()

        val gson = Gson()
        var mAPTCPSocket: Socket? = null
        var retryCounter = 0
        mTask = Runnable {
            try {
                // 1. get ip
                val ip = getAPTCPIP()
                Log.d("AddDeviceController", "getAPTCPIP() ip: $ip")

                // 2. try connect
                mAPTCPSocket = Socket(ip, DeviceManager.TCP_PORT)

                // 3. try to get settings ID
                val bufferedReader = BufferedReader(InputStreamReader(mAPTCPSocket!!.getInputStream()))
                val buffer = CharArray(4096)
                val readLen = bufferedReader.read(buffer, 0, buffer.size)
                val responseString = String(buffer, 0, readLen)
                Log.d("AddDeviceController", "responseString: $responseString")
                // 4. try to get settings
                val itemType = object : TypeToken<Settings>() {}.type
                val mAPSettings = gson.fromJson<Settings>(responseString, itemType)

                cancelTask()
                mActivity.runOnUiThread { mConnectHotspotDialog?.dismiss(); askDeviceName(obtainDevice(mAPSettings, ip))}

            } catch(e: SocketException) {
                Log.d("AddDeviceController", "try to connect to IP.1 (AP TCP), but failed, isn't AP mode")
//                    e.printStackTrace()
                mAPTCPSocket?.close()
                mAPTCPSocket = null
                retryTask(retryCounter++)
            } catch(e: java.net.ConnectException) {
                mAPTCPSocket?.close()
                mAPTCPSocket = null
                retryTask(retryCounter++)
            } catch(e: Exception) {
                e.printStackTrace()
                mAPTCPSocket?.close()
                mAPTCPSocket = null
                retryTask(retryCounter++)
            }
        }

        mHandler?.post(mTask)
    }

    private fun retryTask(retryCounter: Int) {
        if (retryCounter < 3) {
            mHandler?.postDelayed(mTask, 3000)
        } else {
            cancelTask()
            showUnableConnectDialog()
        }
    }

    private fun obtainDevice(
        mAPSettings: Settings,
        ip: String
    ): Device {
        val mAPDevice = Device()
        mAPDevice.setID(mAPSettings.ID)
        mAPDevice.setIP(ip)
        mAPDevice.setCreatedAt(System.currentTimeMillis())
        mAPDevice.setIsFRVisible(mAPSettings.IsFRVisible)
        return mAPDevice
    }

    private fun showUnableConnectDialog() {
        mActivity.runOnUiThread {
            mConnectHotspotDialog?.dismiss()

            AlertDialog
                .Builder(mActivity)
                .setMessage(mActivity.getString(R.string.unable_to_connect_to_device))
                .setNegativeButton(mActivity.getString(R.string.cancel)) { dialog, _ ->
                    dialog.dismiss()
                }
                .setCancelable(true)
//                .setOnDismissListener {
//                    it.dismiss()
//                }
                .create()
                .show()
        }
    }

    fun askDeviceName (device: Device) {
        val ctx = mActivity
        var message : String? = null
        val input = EditText(ctx)

        val (duplicatedDeviceIdx, isDuplicated) = isDeviceDuplicate(device)
        if(isDuplicated) {
            val duplicatedDevice = Service.shared.getDeviceList()[duplicatedDeviceIdx]
            message = ctx.getString(R.string.hint_device_overwrite)
            input.setText(duplicatedDevice.getName())
        }

        AlertDialog.Builder(ctx)
            .setTitle(ctx.getString(R.string.hint_input_device_name))
            .setMessage(message)
            .setView(input)
            .setPositiveButton(R.string.save
            ) { _, _ ->
                val deviceName = input.text.toString()
                if (deviceName.isEmpty()) {
                    Toast.makeText(ctx, ctx.getString(R.string.hint_input_device_name), Toast.LENGTH_SHORT).show()
                } else {
                    device.setName(deviceName)
                    save(device, isDuplicated)
                }
            }
            .setNegativeButton(R.string.cancel
            ) { dialog, _ ->
                dialog.dismiss()
            }
            .setCancelable(false)
            .create()
            .show()
    }

    fun save(device: Device, isDuplicated: Boolean) {
        saveToDB(device, isDuplicated)
        updateServiceList(isDuplicated, device)
        AppPreference(mActivity.getSharedPreferences("app", Context.MODE_PRIVATE)).setOnceCreateFirstDevice()
        goMain()
    }

    private fun updateServiceList(isDuplicated: Boolean, device: Device) {
        if (!isDuplicated) {
            Service.shared.justJoinedDeviceIDList.add(device.getID())
        } else {
            Service.shared.getManagerOfDevice(device)?.updateDevice(device)
        }
    }

    private fun isDeviceDuplicate(device: Device): Pair<Int, Boolean> {
        val duplicatedDeviceIdx =
            Service.shared.getDeviceList().indexOfFirst { it.getID().equals(device.getID()) }
        val isDuplicated = duplicatedDeviceIdx >= 0
        return Pair(duplicatedDeviceIdx, isDuplicated)
    }

    private fun saveToDB (obj: Device, isDuplicated: Boolean) {
        val realm = Realm.getDefaultInstance()
        realm.beginTransaction()
        val device: RealmDevice = if(isDuplicated) realm.where(RealmDevice::class.java).equalTo("mID", obj.getID()).findFirst()!! else realm.createObject(
            RealmDevice::class.java)
        device.setID(obj.getID())
        device.setName(obj.getName())
        device.setIP(obj.getIP())
        device.setCreatedAt(obj.getCreatedAt())
        device.setIsFRVisible(obj.getIsFRVisible())
        realm.commitTransaction()
    }

    private fun goMain () {
        val intent = Intent(mActivity, MainActivity::class.java)
        intent.flags = FLAG_ACTIVITY_NEW_TASK or FLAG_ACTIVITY_CLEAR_TASK
        mActivity.startActivity(intent)
    }

}