package com.ulsee.thermalapp.data.services

import android.util.Log
import java.io.BufferedReader
import java.io.BufferedWriter
import java.io.InputStreamReader
import java.io.OutputStreamWriter
import java.lang.Exception
import java.net.InetSocketAddress
import java.net.Socket

val TAG ="TCPClient"
class TCPClient(ip: String, port: Int) {

    var ip = ip
    val port = port

    interface OnReceivedDataListener {
        fun onData(data: CharArray, size: Int)
    }

    private var TCPClientSocket : Socket? = null
    private var bufferedWriter : BufferedWriter? = null
    private var onReceivedDataListener: OnReceivedDataListener? = null
    var bufferedReader : BufferedReader? = null

    fun connect () {
//        Log.d(TAG, "[Enter] connect")
        val remoteAddress = InetSocketAddress(ip,port)
        val timeout = 3000
        TCPClientSocket = Socket()
        TCPClientSocket?.keepAlive = true
        Log.d(TAG, "[Before] TCPClientSocket?.connect(remoteAddress, timeout)")
        TCPClientSocket?.connect(remoteAddress, timeout)
        Log.d(TAG, "[After] TCPClientSocket?.connect(remoteAddress, timeout)")
        listenData()
    }

    fun close () {
//        Log.d(TAG, "[Enter] close socket")
        TCPClientSocket?.close()
        bufferedReader = null
        bufferedWriter = null
    }

    fun resetIP(ip: String) {
        Log.d(TAG, "[Enter] resetIP last ip: "+this.ip+ " set to: "+ip)
        close()
        this.ip = ip
    }

    fun listenData () {
        Log.d(TAG, "[Enter] listenData")
//        Thread(Runnable {
            var buffer = CharArray(4096)
            var readLen = 0
            while(isConnected()) {
                Log.d(TAG, "[Enter] isConnected() ip: $ip")
                try {
                    if (bufferedReader == null) {
                        bufferedReader = BufferedReader(InputStreamReader(TCPClientSocket?.getInputStream()))
                    }
                    Log.d(TAG, "[Before] bufferedReader!!.read()")
                    readLen = bufferedReader!!.read(buffer, 0, buffer.size)
                    Log.d(TAG, "[After] bufferedReader!!.read()")

                    if (readLen == -1) {
                        Log.i(javaClass.name, "readLen-1, close socket")
                        TCPClientSocket?.close()
                        Log.d(TAG, "readLen-1, close socket. TCPClientSocket?.close()")

                        bufferedReader = null
                        bufferedWriter = null
                        break
                    }
                    onReceivedDataListener?.onData(buffer, readLen)
                    if (onReceivedDataListener == null) {
                        Log.e(javaClass.name, "Error: TCPClient receive message, but onReceivedDataListener is null")
                    };
                } catch(e: Exception) {
                    if (e.message.equals("Connection reset"))break
                    Log.e(javaClass.name, "Error: TCPClient read from socket exception");
                    Log.d(TAG, "Error: TCPClient read from socket exception. e.message: "+ e.message)
                    e.printStackTrace()
                }
            }
//        }).start()
    }

    fun isConnected():Boolean {
        return TCPClientSocket?.isClosed == false && TCPClientSocket?.isConnected == true
    }

    fun reconnect () {
        var reconnectTimes = 0L
        while (!isConnected()) {
            if (reconnectTimes >= 5) {
                throw Exception("can not connect to device")
            }
            connect()
            reconnectTimes += 1
            Thread.sleep(100*reconnectTimes)
        }
    }

    fun setOnReceivedDataListener(listener: OnReceivedDataListener?) {
        if (listener!=null&&onReceivedDataListener != null) Log.e(javaClass.name, "Error: TCPClient setOnReceivedDataListener, but listener already exists");
        if (listener!=null&&onReceivedDataListener != null) throw Exception("Error: TCPClient setOnReceivedDataListener, but listener already exists");
        onReceivedDataListener = listener
    }

    fun send(str: String) {
        Log.d("TCPClient2", "[Enter] TCPClient.send() str: "+ str)
        if (TCPClientSocket?.isConnected == false) {
            throw Exception("error send: socket not connected")
        }
        if (TCPClientSocket?.isClosed != false) {
            throw Exception("error send: socket closed")
        }
        if (bufferedWriter == null) {
            bufferedWriter = BufferedWriter(OutputStreamWriter(TCPClientSocket?.getOutputStream()));
        }
        Log.d("TCPClient", "[Enter] bufferedWriter?.write(str): ")
        bufferedWriter?.write(str)
        bufferedWriter?.flush()
    }
}