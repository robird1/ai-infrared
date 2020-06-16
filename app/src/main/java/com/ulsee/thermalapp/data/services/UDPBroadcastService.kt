package com.ulsee.thermalapp.data.services

import android.content.Context
import android.net.wifi.WifiManager
import android.util.Log
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.request.GetFace
import com.ulsee.thermalapp.data.response.Face
import com.ulsee.thermalapp.ui.device.ScanActivity
import io.reactivex.Emitter
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress

class UDPBroadcastService {

    val UDP_BROADCAST_MOBILE_MESSAGE = "ULSee search"
    val UDP_BROADCAST_MOBILE_SEND_PORT = 2020
    val UDP_BROADCAST_IPC_MESSAGE_PREFIX = "ULSee:"
    val UDP_BROADCAST_IPC_MESSAGE_PORT = 2021

    var mUDPSocket = DatagramSocket()
    var mUDPServerSocket = DatagramSocket(UDP_BROADCAST_IPC_MESSAGE_PORT)
    var mBroadcaseSendCounter = 1 // 數到0就送出
    var mBroadcaseSendInterval = 3 // 數幾下才送出，平常是3，已經掃到有效的QRCode時是1

    var shouldBroadcasting = false

    val isDebug = false
    private fun log (str: String) {
        if (isDebug) Log.i(javaClass.name, str)
    }

    fun getBroadcastAddress(ctx: Context): InetAddress? {
        val wifi = ctx.getSystemService(Context.WIFI_SERVICE) as WifiManager
        val dhcp = wifi.dhcpInfo
        // handle null somehow
        val broadcast = dhcp.ipAddress and dhcp.netmask or dhcp.netmask.inv()
        val quads = ByteArray(4)
        for (k in 0..3) quads[k] = (broadcast shr k * 8 and 0xFF).toByte()
        return InetAddress.getByAddress(quads)
    }

    // 每3秒傳送廣播，如果掃到qrcode,無法匹配，跳出progress表示無法連線，並改為每1秒傳送
    fun initialize(ctx: Context) {

        // 2. keep recv
        Thread(Runnable {
            val lMsg = ByteArray(4096)
            val dp = DatagramPacket(lMsg, lMsg.size)
            try {
                while (!mUDPServerSocket.isClosed) {
                    mUDPServerSocket.receive(dp);
                    val receivedMessage = String(lMsg, 0, dp.length)
                    Log.i(javaClass.name, "got: "+receivedMessage+", from "+dp.address.hostAddress)

                    val isValidDeviceResponse = receivedMessage.startsWith(UDP_BROADCAST_IPC_MESSAGE_PREFIX)
                    if (isValidDeviceResponse) {
                        val deviceID = receivedMessage.substring(UDP_BROADCAST_IPC_MESSAGE_PREFIX.length)
                        Log.i(javaClass.name, "parsed id: "+deviceID)
                        val device = Device()
                        device.setID(deviceID)
                        device.setIP(dp.address.hostAddress)
                        device.setCreatedAt(System.currentTimeMillis())
                        if (mEmitter != null) mEmitter?.onNext(device)
                        else Log.e(javaClass.name, "Error: UDP received device, but no listener")
                    }
                }
                Log.i(javaClass.name, "Broadcast packet sent to: " + getBroadcastAddress(ctx)?.hostAddress)
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }).start()

        mUDPSocket.broadcast = true
        val sendData: ByteArray = UDP_BROADCAST_MOBILE_MESSAGE.toByteArray()
        val sendPacket = DatagramPacket(
            sendData,
            sendData.size,
            getBroadcastAddress(ctx),
            UDP_BROADCAST_MOBILE_SEND_PORT
        )

        // 1. keep send
        Thread(Runnable {
            while (!mUDPSocket.isClosed) {
                try {
                    if(shouldBroadcasting && --mBroadcaseSendCounter==0) {
                        mUDPSocket.send(sendPacket)
                        mBroadcaseSendCounter = mBroadcaseSendInterval
                    }
                    log("sent: "+String(sendData))
                    Thread.sleep(1000)
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }
        }).start()
    }

    fun deinit () {
        if (!mUDPSocket.isClosed)mUDPSocket.close()
        if (!mUDPServerSocket.isClosed)mUDPServerSocket.close()
    }

    var mEmitter : Emitter<Device>? = null

    fun subscribeSearchedDevice() : Observable<Device> {
        val handler: ObservableOnSubscribe<Device> = ObservableOnSubscribe<Device> { emitter ->
            mEmitter = emitter
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}