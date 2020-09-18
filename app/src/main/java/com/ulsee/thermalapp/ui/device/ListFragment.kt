package com.ulsee.thermalapp.ui.device

import android.app.Activity.RESULT_OK
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service

class ListFragment : Fragment() {

    val REQUEST_CODE_ACTIVITY_SCAN = 1234
    lateinit var recyclerView : RecyclerView
    private var deviceChangedReceiver: BroadcastReceiver? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_device_list, container, false)
        recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = DeviceListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(context)

        loadDevices()

        root.findViewById<View>(R.id.fab).setOnClickListener {
            AddDeviceController(requireActivity()).execute()
        }

        (activity as MainActivity).setTitle("Device")

        val intentFilter = IntentFilter("Device removed")
        deviceChangedReceiver = initDeviceChangedReceiver()
        context?.registerReceiver(deviceChangedReceiver, intentFilter)

        return root
    }

    override fun onDestroy() {
        if (deviceChangedReceiver != null) {
            context?.unregisterReceiver(deviceChangedReceiver)
            deviceChangedReceiver = null
        }
        super.onDestroy()
    }

    private fun unregisterDeviceHandler(intent: Intent?) {
        val deviceID = intent?.getStringExtra("device_id")
        Log.d("ListFragment", "[Enter] unregisterDeviceHandler() deviceID: $deviceID")
        Service.shared.getManagerOfDeviceID(deviceID!!)?.unregisterHandler()
    }

    private fun initDeviceChangedReceiver(): BroadcastReceiver {
        return object : BroadcastReceiver() {
            override fun onReceive(context: Context?, intent: Intent?) {
                unregisterDeviceHandler(intent)
                loadDevices()
            }
        }
    }

    private fun loadDevices() {
        val results = Service.shared.getDeviceList()
        (recyclerView.adapter as DeviceListAdapter).setList(results)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == REQUEST_CODE_ACTIVITY_SCAN) {
            if (resultCode == RESULT_OK) {
                loadDevices()
            }
            return
        }
        super.onActivityResult(requestCode, resultCode, data)
    }
}
