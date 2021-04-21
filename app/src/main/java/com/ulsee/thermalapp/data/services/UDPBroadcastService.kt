package com.ulsee.thermalapp.data.services

import android.content.Context
import android.util.Log
import com.ulsee.thermalapp.data.model.Device
import io.reactivex.Emitter
import io.reactivex.Observable
import io.reactivex.ObservableOnSubscribe
import io.reactivex.android.schedulers.AndroidSchedulers
import io.reactivex.schedulers.Schedulers
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress


class UDPBroadcastService {

    companion object {
        const val UDP_BROADCAST_MOBILE_MESSAGE = "ULSee search"
        const val UDP_BROADCAST_OLD_PORT = 2020
        const val UDP_BROADCAST_IPC_MESSAGE_PREFIX = "ULSee:"
        const val UDP_BROADCAST_PORT = 2021
    }

    var mUDPSocket = DatagramSocket(UDP_BROADCAST_PORT)
//    var mUDPServerSocket = DatagramSocket(UDP_BROADCAST_IPC_MESSAGE_PORT)

//    var mBroadcaseSendCounter = 1 // 數到0就送出
//    var mBroadcaseSendInterval = 1 // 數幾下才送出，平常是3，已經掃到有效的QRCode時是1

//    var shouldBroadcasting = false


    fun initialize(ctx: Context) {
        mUDPSocket.broadcast = true

        Thread {
            listenResponse()
        }.start()

        Thread {
            sendUDPBroadcast(initPacket(ctx), initPacketOld(ctx))
        }.start()
    }

    private fun initPacket(ctx: Context): DatagramPacket {
        val sendData = UDP_BROADCAST_MOBILE_MESSAGE.toByteArray()
        return DatagramPacket(
            sendData,
            sendData.size,
            getBroadcastAddress(ctx),
            UDP_BROADCAST_PORT
        )
    }

    private fun initPacketOld(ctx: Context): DatagramPacket {
        val sendData = UDP_BROADCAST_MOBILE_MESSAGE.toByteArray()
        return DatagramPacket(
            sendData,
            sendData.size,
            getBroadcastAddress(ctx),
            UDP_BROADCAST_OLD_PORT
        )
    }

    private fun listenResponse() {
        val lMsg = ByteArray(4096)
        val dp = DatagramPacket(lMsg, lMsg.size)
        try {
            while (!mUDPSocket.isClosed) {
                mUDPSocket.receive(dp)
                val receivedMessage = String(lMsg, 0, dp.length)
                Log.i(javaClass.name, "got: " + receivedMessage + ", from " + dp.address.hostAddress)

                checkResponse(receivedMessage, dp)
            }
            Log.w(javaClass.name, "UDP Broadcast closed")
        } catch (e: Exception) {
            e.printStackTrace()
            Log.d(javaClass.name, e.message)
        }
    }

    private fun sendUDPBroadcast(sendPacket: DatagramPacket, sendPacketOld: DatagramPacket) {
        while (!mUDPSocket.isClosed) {
            try {
//                if(shouldBroadcasting && --mBroadcaseSendCounter==0) {
                mUDPSocket.send(sendPacket)
                mUDPSocket.send(sendPacketOld)
//                }
//                log("sent: "+String(sendData))
            } catch (e: Exception) {
                e.printStackTrace()
            } finally {
                Thread.sleep(2000)
//                if (mBroadcaseSendCounter <= 0) mBroadcaseSendCounter = mBroadcaseSendInterval
            }
        }
    }

    private fun getBroadcastAddress(ctx: Context): InetAddress? {
//        val wifi = ctx.getSystemService(Context.WIFI_SERVICE) as WifiManager
//        val dhcp = wifi.dhcpInfo
//        // handle null somehow
//        val broadcast = dhcp.ipAddress and dhcp.netmask or dhcp.netmask.inv()
//        val quads = ByteArray(4)
//        for (k in 0..3) quads[k] = (broadcast shr k * 8 and 0xFF).toByte()
//        val inetAddr = InetAddress.getByAddress(quads)
//        Log.d("UDPBroadcastService", "inetAddr: $inetAddr")
//        Log.d("UDPBroadcastService", "inetAddr.hostAddress: ${inetAddr.hostAddress}")
        return InetAddress.getByName("255.255.255.255")
    }

    private fun checkResponse(receivedMessage: String, dp: DatagramPacket) {
        val isValidDeviceResponse = receivedMessage.startsWith(
            UDP_BROADCAST_IPC_MESSAGE_PREFIX
        )
        if (isValidDeviceResponse) {
            val deviceID =
                receivedMessage.substring(UDP_BROADCAST_IPC_MESSAGE_PREFIX.length)
            Log.i(javaClass.name, "parsed id: $deviceID")
            val device = Device()
            device.setID(deviceID)
            device.setIP(dp.address.hostAddress)
            device.setCreatedAt(System.currentTimeMillis())
            if (mEmitter != null) mEmitter?.onNext(device)
            else Log.e(javaClass.name, "Error: UDP received device, but no listener")
        }
    }

//    fun deinit () {
//        if (!mUDPSocket.isClosed)mUDPSocket.close()
//        if (!mUDPServerSocket.isClosed)mUDPServerSocket.close()
//    }

    var mEmitter : Emitter<Device>? = null

    fun subscribeSearchedDevice() : Observable<Device> {
        val handler = ObservableOnSubscribe<Device> { emitter ->
            mEmitter = emitter
        }

        return Observable.create(handler).subscribeOn(Schedulers.newThread())
            .observeOn(AndroidSchedulers.mainThread())
    }
}