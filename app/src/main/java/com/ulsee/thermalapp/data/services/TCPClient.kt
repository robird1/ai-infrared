package com.ulsee.thermalapp.data.services

import android.util.Log
import java.io.BufferedReader
import java.io.BufferedWriter
import java.io.InputStreamReader
import java.io.OutputStreamWriter
import java.lang.Exception
import java.net.Socket

class TCPClient(ip: String, port: Int) {

    val ip = ip
    val port = port

    interface OnReceivedDataListener {
        fun onData(data: CharArray, size: Int)
    }

    private var TCPClientSocket : Socket? = null
    private var bufferedWriter : BufferedWriter? = null
    private var onReceivedDataListener: OnReceivedDataListener? = null

    fun connect () {
        TCPClientSocket = Socket(ip, port)
        TCPClientSocket?.keepAlive = true
        listenData()
    }

    fun listenData () {
        Thread(Runnable {
            var bufferedReader : BufferedReader? = null
            var buffer = CharArray(4096)
            var readLen = 0
            while(isConnected()) {
                if (bufferedReader == null) {
                    bufferedReader = BufferedReader(InputStreamReader(TCPClientSocket?.getInputStream()))
                }
                readLen = bufferedReader.read(buffer, 0, buffer.size)
                if (readLen == -1) {
                    Log.i(javaClass.name, "readLen-1, close socket")
                    TCPClientSocket?.close()
                    break
                }
                onReceivedDataListener?.onData(buffer, readLen)
                if (onReceivedDataListener == null) Log.e(javaClass.name, "Error: TCPClient receive message, but onReceivedDataListener is null");
            }
        }).start()
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
        if (TCPClientSocket?.isConnected == false) {
            throw Exception("error socket not connected")
        }
        if (bufferedWriter == null) {
            bufferedWriter = BufferedWriter(OutputStreamWriter(TCPClientSocket?.getOutputStream()));
        }
        bufferedWriter?.write(str)
        bufferedWriter?.flush()
    }
}