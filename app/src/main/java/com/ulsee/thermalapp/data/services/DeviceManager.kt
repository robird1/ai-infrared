package com.ulsee.thermalapp.data.services

import android.content.Context
import android.content.Intent
import android.os.Handler
import android.os.HandlerThread
import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.response.FaceList
import com.ulsee.thermalapp.data.response.NotificationImage
import com.ulsee.thermalapp.data.response.NotificationList
import com.ulsee.thermalapp.data.response.VideoFrame
import com.ulsee.thermalapp.ui.notification.NotificationActivity
import com.ulsee.thermalapp.utils.NotificationCenter
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import org.json.JSONObject

class DeviceManager(context: Context, device: Device) {

    companion object {
        val TCP_PORT = 13888
    }
    enum class Status(status: Int) {
        connecting(0), connected(1)
    }
    enum class Action(val id: Int) {
        putSettings(0),         // Update device settings by user
        calibration(1),
        updateFace(2),
        requestTwoPicture(3),   // 沒用到。For calibration.
        requestStreaming(4),
        requestFaceList(5),
        settingsResponse(6),    // Receive device settings when TCP connection is established
        faceListResponse(7),
        pictureResponse(8),     // 沒用到。For calibration.
        streamResponse(9),
        requestFace(10),        // 沒用到。people face request
        faceResponse(11),       // 沒用到。people face response
        modifyWifi(12),
        modifyWifiACK(13),
        requestNotificationList(14),
        notificationListResponse(15),
        requestNotificationImage(16),
        notificationImageResponse(17),
        notification(18),
        notifyActivated(19),
        peopleSearch(20),
        recordFilter(21),
        requestDeviceSettings(22),     // Request device settings when user enters device setting page
        deviceSettingsResponse(23)     // Obtain device settings after sending Action 22
    }

    private val mLock = Object()

    interface OnGotFaceListListener{
        fun onGotFaceList(falceList: List<com.ulsee.thermalapp.data.model.Face>)
    }
    var mOnGotFaceListListener : OnGotFaceListListener? = null
    fun setOnGotFaceListListener(listener: OnGotFaceListListener?) {
        if(listener != null && mOnGotFaceListListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnGotFaceListListener")
        mOnGotFaceListListener = listener
    }

    interface OnGotVideoFrameListener{
        fun onVideoFrame(frame: String)
    }
    var mOnGotVideoFrameListener : OnGotVideoFrameListener? = null
    fun setOnGotVideoFrameListener(listener: OnGotVideoFrameListener?) {
        if(listener != null && mOnGotVideoFrameListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnGotVideoFrameListener")
        mOnGotVideoFrameListener = listener
    }

    interface OnGotNotificationListListener{
        fun onGotNotificationList(falceList: List<com.ulsee.thermalapp.data.model.Notification>)
    }
    var mOnGotNotificationListListener : OnGotNotificationListListener? = null
    fun setOnGotNotificationListListener(listener: OnGotNotificationListListener?) {
        if(listener != null && mOnGotNotificationListListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnGotNotificationListListener")
        mOnGotNotificationListListener = listener
    }

    interface OnGotNotificationImageListener{
        fun onNotificationImage(image: NotificationImage) : Boolean
    }
    var mOnGotNotificationImageListenerList = ArrayList<OnGotNotificationImageListener>()
    fun addOnGotNotificationImageListener(listener: OnGotNotificationImageListener) {
        synchronized(mLock) {
            mOnGotNotificationImageListenerList.add(listener)
        }
    }
    fun removeOnGotNotificationImageListener(listener: OnGotNotificationImageListener) {
        synchronized(mLock) {
            mOnGotNotificationImageListenerList.remove(listener)
        }
    }

    interface OnGotDeviceSettingsListener{
        fun onSettings(settings: Settings)
    }
    var mOnGotSettingsListener : OnGotDeviceSettingsListener? = null
    fun setOnGotSettingsListener(listener: OnGotDeviceSettingsListener?) {
        if(listener != null && mOnGotSettingsListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnGotSettingsListener")
        mOnGotSettingsListener = listener
    }


    var device = device
    var settings : Settings? = null
    val tcpClient = TCPClient(device, DeviceManager.TCP_PORT)
    var mIsIDNotMatched = false
    private var mHandler: Handler? = null
    private var mThread: HandlerThread? = null
    private lateinit var mTask: Runnable
    val mContext = context

    init {
        listenData()
        keepConnection()
    }

    fun resetIP(ip: String) {
        device.setIP(ip)
        tcpClient.resetIP(ip)
    }
    val isDebug = false
    private fun log (str: String) {
        if (isDebug) Log.i(javaClass.name, str)
    }

    private fun initHandler() {
        mThread = HandlerThread("add_device_thread")
        mThread?.start()
        mHandler = Handler(mThread?.looper!!)
    }

    fun unregisterHandler() {
        Log.d("DeviceManager", "[Enter] unregisterHandler")
        Log.d("DeviceManager", "[Before] tcpClient.close()")
        tcpClient.close()
        Log.d("DeviceManager", "[After] tcpClient.close()")
        mHandler?.removeCallbacks(mTask)
        mHandler = null
        mThread?.quit()
        mThread = null
    }

    fun keepConnection () {
        initHandler()

        mTask = Runnable {
            try {
                Log.d("DeviceManager", "[Before] tcpClient.connect()")
                tcpClient.connect()
                Log.d("DeviceManager", "[After] tcpClient.connect() tcpClient.isConnected(): "+ tcpClient.isConnected())

                if (!mIsIDNotMatched) {
                    // Device is not connected after finishing tcpClient.connect()
                    mHandler?.post(mTask)
                } else {
                    mHandler?.postDelayed(mTask, 60000)
                }
            }
            catch (e: Exception) {
                Log.d("DeviceManager", "[Enter] Exception e.message: "+e.message)
                mHandler?.postDelayed(mTask, 1000)
            }
        }

        mHandler?.post(mTask)
    }

    fun processBuffer (stringBuilder: StringBuilder) : Boolean {
        var hasAtLeastOnePacket = false

        val gson = Gson()
        while(stringBuilder.startsWith("\n"))stringBuilder.delete(0, 1)

        if (stringBuilder.length == 0) return hasAtLeastOnePacket

        // 1. parse packet
        if (!stringBuilder.startsWith("{")) {
            Log.e(javaClass.name, "response not start with {, error, drop..."+stringBuilder.length)
            Log.e(javaClass.name, stringBuilder.toString())
            stringBuilder.clear()
            return hasAtLeastOnePacket
        }

        if (!(stringBuilder.endsWith("}") || stringBuilder.endsWith("}\n"))) {
            log("onData, packet not end, continue...")
            return hasAtLeastOnePacket
        }

        var responseString = stringBuilder.toString()
        val len = parseStickyPacketFirstPacketLength(responseString)
        if(len == -1) return hasAtLeastOnePacket
        hasAtLeastOnePacket = true
        log("parseStickyPacketFirstPacketLength "+len)
        responseString = responseString.substring(0, len)
        stringBuilder.delete(0, len)


        // 2. parse action
        val obj = JSONObject(responseString)
        if (!obj.has("Action")) {
            Log.e(javaClass.name, "response did not contain key 'Action'")
            return hasAtLeastOnePacket
        }
        val action = obj.getInt("Action")

        log("onData, got action"+action)
        Log.d("DeviceManager", "responseString: "+responseString)

        // 3. parse content
        when(action) {
            Action.putSettings.id -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.calibration.id -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.updateFace.id -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.requestTwoPicture.id -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.requestStreaming.id -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.requestFaceList.id -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.requestFace.id -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.modifyWifi.id -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.settingsResponse.id -> {
                Log.d("DeviceManager", "[Enter] when() -> Action.settingsResponse")

                val itemType = object : TypeToken<Settings>() {}.type
                try {
                    settings = gson.fromJson<Settings>(responseString, itemType)
                    if (settings!!.Deviation > 10) settings?.Deviation = 0.0
                    if (settings!!.Deviation < -10) settings?.Deviation = 0.0
                    if (settings!!.ID != device.getID()) {
                        Log.d(TAG, "device.getID(): "+ device.getID()+" settings.ID: "+ settings!!.ID)
                        Log.w(javaClass.name, "device connected but id not match...")
                        mIsIDNotMatched = true;
                        tcpClient.close();
                        return true;
                    } else {
                        mIsIDNotMatched = false
                    }

                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            Action.deviceSettingsResponse.id -> {
                Log.d(TAG, "[Enter] when() -> Action.deviceSettingsResponse ")
                val itemType = object : TypeToken<Settings>() {}.type
                try {
                    settings = gson.fromJson<Settings>(responseString, itemType)
                    if (settings!!.Deviation > 10) settings?.Deviation = 0.0
                    if (settings!!.Deviation < -10) settings?.Deviation = 0.0
                    if (settings!!.ID != device.getID()) {
                        Log.d(TAG, "device.getID(): "+ device.getID()+" settings.ID: "+ settings!!.ID)
                        Log.w(javaClass.name, "device connected but id not match...")
                        mIsIDNotMatched = true;
                        tcpClient.close();
                        return true;
                    } else {
                        mIsIDNotMatched = false
                    }

                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }

                mOnGotSettingsListener?.onSettings(settings!!)

            }
            Action.faceListResponse.id -> {
                val itemType = object : TypeToken<FaceList>() {}.type
                try {
                    val faceList = gson.fromJson<FaceList>(responseString, itemType)
                    log("got face list, size: "+faceList.FaceList.size)
                    if (mOnGotFaceListListener == null) {
                        Log.e(javaClass.name, "Error no listener of action "+action)
                    }
                    mOnGotFaceListListener?.onGotFaceList(faceList.FaceList)
                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            Action.streamResponse.id -> {
                val itemType = object : TypeToken<VideoFrame>() {}.type
                try {
                    val videoFrame = gson.fromJson<VideoFrame>(responseString, itemType)
                    if (mOnGotVideoFrameListener== null) {
                        Log.e(javaClass.name, "Error no listener of action "+action)
                    }
                    mOnGotVideoFrameListener?.onVideoFrame(videoFrame.Data)
                    log("got video frame")
                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            Action.notificationListResponse.id -> {
                Log.d("DeviceManager", "[Enter] when() -> Action.notificationListResponse")

//                responseString = createNotifyListString()
//                Log.d("DeviceManager", "responseString:　"+ responseString)

                val itemType = object : TypeToken<NotificationList>() {}.type
                try {
                    val list = gson.fromJson<NotificationList>(responseString, itemType)
                    log("got notification list, size: "+list.FaceList.size)
                    if (mOnGotNotificationListListener == null) {
                        Log.e(javaClass.name, "Error no listener of action "+action)
                    }
                    mOnGotNotificationListListener?.onGotNotificationList(list.FaceList)
                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            Action.notificationImageResponse.id -> {
                Log.d("DeviceManager", "[Enter] when() -> Action.notificationImageResponse")

                val itemType = object : TypeToken<NotificationImage>() {}.type
                try {
                    val response = gson.fromJson<NotificationImage>(responseString, itemType)
                    if (mOnGotNotificationImageListenerList.size == 0) {
                        Log.e(javaClass.name, "Error no listener of action "+action)
                    }
                    synchronized(mLock) {
                        var result = false
                        for(i in mOnGotNotificationImageListenerList.indices) {
                            result = result || mOnGotNotificationImageListenerList[i].onNotificationImage(response)
                        }
                        // for (listener in mOnGotFaceListenerList) result = result || listener.onFace(response)
                        if (result == false) {
                            Log.e(javaClass.name, "Error got notification image of "+response.ID+", but no one handle")
                        }
                    }
                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            Action.notification.id -> {
                Log.d("DeviceManager", "[Enter] when() -> Action.notification")
//                Log.d("DeviceManager", "responseString:　"+ responseString)

                val itemType = object : TypeToken<Notification>() {}.type
                try {
                    val response = gson.fromJson<Notification>(responseString, itemType)
                    doNotify(response)
                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            else -> {
                Log.e(javaClass.name, "unknown Action: "+action)
            }
        }

        return true
    }

    fun listenData () {
        val stringBuilder = StringBuilder();

        tcpClient.setOnReceivedDataListener(object: TCPClient.OnReceivedDataListener{
            override fun onData(data: CharArray, size: Int) {
//                Log.d("DeviceManager", "[Enter] TCPClient.OnReceivedDataListener.onData")
                stringBuilder.append(data, 0, size)
                log("onData, size = "+size)
//                Log.d("DeviceManager", "[Enter] OnReceivedDataListener.onData")

                while (processBuffer(stringBuilder))continue
            }
        })
    }

    private fun parseStickyPacketFirstPacketLength(content: String) : Int {
        var leftBigParanthesesCount = 0
        var rightBigParanthesesCount = 0
        for(i in content.indices) {
            if(content[i] == '{')leftBigParanthesesCount +=1
            if(content[i] == '}')rightBigParanthesesCount +=1
            if (leftBigParanthesesCount>0 && leftBigParanthesesCount==rightBigParanthesesCount) {
                return i+1
            }
        }
        return -1
    }

    fun subscribeConnectionStatus() : Observable<Status> {
        val handler: ObservableOnSubscribe<Status> = ObservableOnSubscribe<Status> { emitter ->
            var isConnected = tcpClient.isConnected()
            emitter.onNext(if (isConnected) Status.connected else Status.connecting)
            try {
                while(!emitter.isDisposed) {
                    if (tcpClient.isConnected() != isConnected) {
                        isConnected = tcpClient.isConnected()
                        emitter.onNext(if (isConnected) Status.connected else Status.connecting)
                    }
                    Thread.sleep(1000)
                }
            } catch (e: java.lang.Exception) {
                if (!emitter.isDisposed)emitter.onError(e)
            }
        }

        return io.reactivex.Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }

    /**
     *  Updating device if user add the same device and rename it.
     */
    fun updateDevice(d: Device) {
        device = d
    }

    private fun doNotify(notification: Notification) {
        Log.d("DeviceManager", "[Enter] doNotify")
        val isNotificationEnabled = AppPreference(mContext.getSharedPreferences("app", Context.MODE_PRIVATE)).isFeverNotificationEnabled()
        if (isNotificationEnabled) {
            val intent = Intent(mContext, NotificationActivity::class.java)
            intent.putExtra("notification", notification)
            NotificationCenter.shared.show2(mContext, intent, mContext.getString(R.string.title_alert_notification), notification)
        }
    }

}