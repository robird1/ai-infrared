package com.ulsee.thermalapp.ui.device

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.ui.people.DeviceSyncRepository

class MultiDeviceActivity: AppCompatActivity() {
    private lateinit var recyclerView: RecyclerView
    private lateinit var viewModel: DeviceViewModel

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_multi_device)
        initViewModel()
        observeDeviceList()
    }

    override fun onDestroy() {
        (recyclerView.adapter as MultiDeviceAdapter).destroy()
        super.onDestroy()
    }

    private fun initViewModel() {
        viewModel = ViewModelProvider(this, DeviceFactory(DeviceSyncRepository()))
            .get(DeviceViewModel::class.java)
    }

    private fun observeDeviceList() {
        viewModel.deviceList.observe(this) {
            initRecyclerView(it)
        }
    }

    private fun initRecyclerView(devices: List<Device>) {
        recyclerView = findViewById(R.id.recyclerView)
        recyclerView.adapter = MultiDeviceAdapter(devices, viewModel)
        recyclerView.layoutManager = GridLayoutManager(this, 2)
    }

}