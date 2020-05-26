package com.ulsee.thermalapp.ui.device

import android.app.Activity.RESULT_OK
import android.content.Intent
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Device
import io.realm.Realm
import io.realm.kotlin.where

class ListFragment : Fragment() {

    val REQUEST_CODE_ACTIVITY_SCAN = 1234
    lateinit var recyclerView : RecyclerView

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_device_list, container, false)
        recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = DeviceListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(context)

        val realm = Realm.getDefaultInstance()
        val results = realm.where<Device>().findAll()
        (recyclerView.adapter as DeviceListAdapter).setList(results)

        root.findViewById<View>(R.id.fab).setOnClickListener { openScanner() }

        (activity as MainActivity).setTitle("Device")
        return root
    }

    private fun openScanner () {
        startActivityForResult(Intent(context, ScanActivity::class.java), REQUEST_CODE_ACTIVITY_SCAN)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == REQUEST_CODE_ACTIVITY_SCAN) {
            if (resultCode == RESULT_OK) {
                val realm = Realm.getDefaultInstance()
                val results = realm.where<Device>().findAll()
                (recyclerView.adapter as DeviceListAdapter).setList(results)
            }
            return
        }
        super.onActivityResult(requestCode, resultCode, data)
    }
}
