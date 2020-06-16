package com.ulsee.thermalapp.data.services

import android.util.Log
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.response.Face
import com.ulsee.thermalapp.data.response.FaceList
import com.ulsee.thermalapp.data.response.TwoPicture
import com.ulsee.thermalapp.data.response.VideoFrame
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import io.realm.Realm
import org.json.JSONObject
import java.lang.StringBuilder
import kotlin.collections.ArrayList

class DeviceManager(device: Device) {
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

    interface OnGotTwoPictureListListener{
        fun onGotTwoPictureList(rgb: String, thermal: String)
    }
    var mOnGotTwoPictureListListener : OnGotTwoPictureListListener? = null
    fun setOnGotTwoPictureListListener(listener: OnGotTwoPictureListListener?) {
        if(listener != null && mOnGotTwoPictureListListener != null) Log.e(javaClass.name, "error set listener but already exists: setOnGotTwoPictureListListener")
        mOnGotTwoPictureListListener = listener
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
        fun onFace(face: Face) : Boolean
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

    val device = device
    var settings : Settings? = null
    var tcpClient = TCPClient(device.getIP(), 13888)

    init {
        listenData()
        keepConnection()
    }

    fun resetIP(ip: String) {
//        val realm = Realm.getDefaultInstance()
//        realm.beginTransaction()
        device.setIP(ip)
//        realm.commitTransaction()
        tcpClient = TCPClient(ip, 13888)
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
                Thread.sleep(1000)
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
                val itemType = object : TypeToken<Settings>() {}.type
                try {
                    settings = gson.fromJson<Settings>(responseString, itemType)
                    if (settings?.IsFirstSetting == true) {
                        val isJustJoinedDevice = Service.shared.justJoinedDeviceIDList.contains(device.getID())
                        Log.i(javaClass.name, "find first settings device: "+device.getID()+", is just joined:"+isJustJoinedDevice)
                        if (isJustJoinedDevice) {
                            Service.shared.requestTutorial(device.getID())
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
                    if (mOnGotTwoPictureListListener== null) {
                        Log.e(javaClass.name, "Error no listener of action "+action)
                    }
                    mOnGotTwoPictureListListener?.onGotTwoPictureList(twoPicture.Data_1, twoPicture.Data_2)
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
                val itemType = object : TypeToken<Face>() {}.type
                try {
                    val response = gson.fromJson<Face>(responseString, itemType)
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
                stringBuilder.append(data, 0, size)
                log("onData, size = "+size)

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

    fun subscribeStatus() : Observable<Status> {
        val handler: ObservableOnSubscribe<Status> = ObservableOnSubscribe<Status> { emitter ->
            var isConnected = tcpClient.isConnected()
            emitter.onNext(if (isConnected) Status.connected else Status.connecting)
            while(!emitter.isDisposed) {
                if (tcpClient.isConnected() != isConnected) {
                    isConnected = tcpClient.isConnected()
                    emitter.onNext(if (isConnected) Status.connected else Status.connecting)
                }
                Thread.sleep(1000)
            }
        }

        return io.reactivex.Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}