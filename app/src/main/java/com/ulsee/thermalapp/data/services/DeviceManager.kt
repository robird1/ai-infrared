package com.ulsee.thermalapp.data.services

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.model.Notification2
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.response.*
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import org.json.JSONArray
import org.json.JSONObject
import java.lang.StringBuilder
import kotlin.collections.ArrayList

class DeviceManager(device: Device) {

    companion object {
        val TCP_PORT = 13888
    }
    enum class Status(status: Int) {
        connecting(0), connected(1)
    }
    enum class Action {
        putSettings,
        calibration,
        updateFace,
        requestTwoPicture,
        requestStreaming,
        requestFaceList,
        settingsResponse,
        faceListResponse,
        pictureResponse,
        streamResponse, // 9
        requestFace,
        faceResponse,
        modifyWifi, // 12
        modifyWifiACK,
        requestNotificationList,
        notificationListResponse,
        requestNotificationImage,
        notificationImageResponse,
        notification,// 18
        notifyActivated,
//        requestFaceDB, // 20
//        requestFaceList, // 21
//        faceListResponse // 22
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

    interface OnGotTwoPictureListener{
        fun onGotTwoPicture(twoPicture: TwoPicture)
    }
    var mOnGotTwoPictureListener : OnGotTwoPictureListener? = null
    fun setOnGotTwoPictureListener(listener: OnGotTwoPictureListener?) {
        if(listener != null && mOnGotTwoPictureListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnGotTwoPictureListener")
        mOnGotTwoPictureListener = listener
    }

    interface OnGotVideoFrameListener{
        fun onVideoFrame(frame: String)
    }
    var mOnGotVideoFrameListener : OnGotVideoFrameListener? = null
    fun setOnGotVideoFrameListener(listener: OnGotVideoFrameListener?) {
        if(listener != null && mOnGotVideoFrameListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnGotVideoFrameListener")
        mOnGotVideoFrameListener = listener
    }

    interface OnGotFaceListener{
        fun onFace(face: com.ulsee.thermalapp.data.model.Face) : Boolean
    }
    var mOnGotFaceListener : OnGotFaceListener? = null
    fun setOnGotFaceListener(listener: OnGotFaceListener?) {
        if(listener != null && mOnGotFaceListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnGotFaceListener")
        mOnGotFaceListener = listener
    }

    var mOnGotFaceListenerList = ArrayList<OnGotFaceListener>()
    fun addOnGotFaceListener(listener: OnGotFaceListener) {
        synchronized(mLock) {
            mOnGotFaceListenerList.add(listener)
        }
    }
    fun removeOnGotFaceListener(listener: OnGotFaceListener) {
        synchronized(mLock) {
            mOnGotFaceListenerList.remove(listener)
        }
    }

    interface OnGotNotificationListListener{
        fun onGotNotificationList(falceList: List<com.ulsee.thermalapp.data.model.Notification2>)
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

    interface OnNotificationListener{
        fun onNotification(notification: Notification2)
    }
    var mOnNotificationListener : OnNotificationListener? = null
    fun setOnNotificationListener(listener: OnNotificationListener?) {
        if(listener != null && mOnNotificationListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnNotificationListener")
        mOnNotificationListener = listener
    }
    val haveOnNotificationListener : Boolean
        get() = mOnNotificationListener != null

    val device = device
    var settings : Settings? = null
    var tcpClient = TCPClient(device.getIP(), DeviceManager.TCP_PORT)
    var mIsIDNotMatched = false

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

    fun keepConnection () {
        Thread(Runnable {
            connectUntilSuccess()
            while(true) {
                // log("isConnected: "+(if(tcpClient.isConnected())"Y" else "N"))
                if (!tcpClient.isConnected()) connectUntilSuccess()
                Thread.sleep(if (mIsIDNotMatched) 10000 else 1000)
            }
        }).start()
    }

    fun connectUntilSuccess () {
        while(true) {
            try {
                tcpClient.connect()
                log("device connected!!!:"+tcpClient.ip)
                break;
            } catch(e:Exception) {
//                Log.e(javaClass.name, "connectUntilSuccess error:"+tcpClient.ip)
//                e.printStackTrace()
                Thread.sleep(1000)
            }
        }
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
        Log.d("DeviceManager", "onData, got action"+action)
        Log.d("DeviceManager", "responseString: "+responseString)

        // 3. parse content
        when(action) {
            Action.putSettings.ordinal -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.calibration.ordinal -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.updateFace.ordinal -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.requestTwoPicture.ordinal -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.requestStreaming.ordinal -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.requestFaceList.ordinal -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.requestFace.ordinal -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.modifyWifi.ordinal -> { Log.e(javaClass.name, "received unexpected Action (should not be sent by ipc): "+action) }
            Action.settingsResponse.ordinal -> {
                Log.d("DeviceManager", "[Enter] when() -> Action.settingsResponse")

                val itemType = object : TypeToken<Settings>() {}.type
                try {
                    settings = gson.fromJson<Settings>(responseString, itemType)
                    if (settings!!.Deviation > 10) settings?.Deviation = 0.0
                    if (settings!!.Deviation < -10) settings?.Deviation = 0.0
                    if (settings!!.ID != device.getID()) {
                        Log.w(javaClass.name, "device connected but id not match...")
                        mIsIDNotMatched = true;
                        tcpClient.close();
                        return true;
                    }
                    if (settings?.IsFirstActivate == true) {
                        val isJustJoinedDevice = Service.shared.justJoinedDeviceIDList.contains(device.getID())
                        Log.i(javaClass.name, "find first settings device: "+device.getID()+", is just joined:"+isJustJoinedDevice)
                        if (isJustJoinedDevice) {
                            Service.shared.requestTutorial(device.getID())
                            Service.shared.justJoinedDeviceIDList.remove(device.getID())
                        }
                    }
                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            Action.faceListResponse.ordinal -> {
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
            Action.pictureResponse.ordinal -> {
                val itemType = object : TypeToken<TwoPicture>() {}.type
                try {
                    val twoPicture = gson.fromJson<TwoPicture>(responseString, itemType)
                    if (mOnGotTwoPictureListener== null) {
                        Log.e(javaClass.name, "Error no listener of action "+action)
                    }
                    mOnGotTwoPictureListener?.onGotTwoPicture(twoPicture)
                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            Action.streamResponse.ordinal -> {
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
            Action.faceResponse.ordinal -> {
                val itemType = object : TypeToken<com.ulsee.thermalapp.data.model.Face>() {}.type
                try {
                    val response = gson.fromJson<com.ulsee.thermalapp.data.model.Face>(responseString, itemType)
//                            if (mOnGotFaceListener== null) {
//                                Log.e(javaClass.name, "Error no listener of action "+action)
//                            }
//                            mOnGotFaceListener?.onFace(response)
                    if (mOnGotFaceListenerList.size == 0) {
                        Log.e(javaClass.name, "Error no listener of action "+action)
                    }
                    synchronized(mLock) {
                        var result = false
                        for(i in mOnGotFaceListenerList.indices) {
                            result = result || mOnGotFaceListenerList[i].onFace(response)
                        }
                        // for (listener in mOnGotFaceListenerList) result = result || listener.onFace(response)
                        if (result == false) {
                            Log.e(javaClass.name, "Error got face image of "+response.Name+", but no one handle")
                        }
                    }
                } catch(e: java.lang.Exception) {
                    Log.e(javaClass.name, "Error parse action "+action)
                    e.printStackTrace()
                }
            }
            Action.notificationListResponse.ordinal -> {
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
            Action.notificationImageResponse.ordinal -> {
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
            Action.notification.ordinal -> {
                Log.d("DeviceManager", "[Enter] when() -> Action.notification")
                Log.d("DeviceManager", "responseString:　"+ responseString)

                val itemType = object : TypeToken<Notification2>() {}.type
                try {
                    val response = gson.fromJson<Notification2>(responseString, itemType)
                    if (mOnNotificationListener== null) {
                        Log.e(javaClass.name, "Error no listener of action "+action)
                    }
                    mOnNotificationListener?.onNotification(response)
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

    fun createNotifyListString () : String {
        val obj = JSONObject()
        obj.put("ID", 1)
        obj.put("Name", "Steve")
        obj.put("TempUnit", 0)
        obj.put("TempValue", 40)
        obj.put("IsMask", false)
        obj.put("Time", "2020-06-20 12:02:22")

        val obj2 = JSONObject()
        obj2.put("ID", 2)
        obj2.put("Name", "Steve_su")
        obj2.put("TempUnit", 1)
        obj2.put("TempValue", 50)
        obj2.put("IsMask", true)
        obj2.put("Time", "2020-06-22 11:01:11")

        val ja = JSONArray()
        ja.put(obj)
        ja.put(obj2)

        val mainObj = JSONObject()
        mainObj.put("FaceList", ja)

        return mainObj.toString()
    }

    fun listenData () {
        val stringBuilder = StringBuilder();

        tcpClient.setOnReceivedDataListener(object: TCPClient.OnReceivedDataListener{
            override fun onData(data: CharArray, size: Int) {
                Log.d("DeviceManager", "[Enter] TCPClient.OnReceivedDataListener.onData")
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
}