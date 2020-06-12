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

    // service會一直監聽 UDPBroadcastService 掃到的裝置，同時儲存尚未註冊但是已經掃到的device，但3秒後沒有回應時就丟棄
    // 當 scanActivity 有需求時，就會去跟 service要device

    // todo: udp broadcast

    var mUDPSocket = DatagramSocket()
    var mBroadcaseSendCounter = 1 // 數到0就送出
    var mBroadcaseSendInterval = 3 // 數幾下才送出，平常是3，已經掃到有效的QRCode時是1

    var shouldBroadcasting = false

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
        val message = "ULSEE-Thermal-Searching"
        val BROADCAST_PORT = 8829

        mUDPSocket.broadcast = true
        val sendData: ByteArray = message.toByteArray()
        val sendPacket = DatagramPacket(
            sendData,
            sendData.size,
            getBroadcastAddress(ctx),
            BROADCAST_PORT
        )

        // 1. keep send
        Thread(Runnable {
            while (!mUDPSocket.isClosed) {
                try {
                    if(shouldBroadcasting && --mBroadcaseSendCounter==0) {
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
                while (!mUDPSocket.isClosed) {
                    mUDPSocket.receive(dp);
                    val receivedMessage = String(lMsg, 0, dp.length)
                    Log.i(javaClass.name, "got: "+receivedMessage)

                    val isValidDeviceResponse = receivedMessage.startsWith("ULSEE") // TODO: validate message
                    if (isValidDeviceResponse) {

                        val deviceID = receivedMessage.substring(6)
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