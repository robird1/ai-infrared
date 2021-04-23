package com.ulsee.thermalapp.ui.people

import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuInflater
import android.view.MenuItem
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.ViewModelProvider
import androidx.navigation.fragment.findNavController
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.databinding.ActivityDeviceSyncBinding
import com.ulsee.thermalapp.ui.notification.ListFragment
import com.ulsee.thermalapp.ui.notification.RecordFilterActivity

private val TAG = DeviceSyncActivity::class.java.simpleName

class DeviceSyncActivity: AppCompatActivity() {
    private lateinit var binding: ActivityDeviceSyncBinding
    private lateinit var viewModel: DeviceSyncViewModel
    private lateinit var recyclerView: RecyclerView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityDeviceSyncBinding.inflate(layoutInflater)
        setContentView(binding.root)
        setSupportActionBar(binding.toolbar)

        binding.progressView.visibility = View.VISIBLE
        viewModel = ViewModelProvider(this, DeviceSyncFactory(DeviceSyncRepository()))
            .get(DeviceSyncViewModel::class.java)

        recyclerView = binding.recyclerView
        recyclerView.adapter = DeviceSyncAdapter()
        recyclerView.layoutManager = LinearLayoutManager(this)

        binding.addBtn.setOnClickListener {
            add()
        }

        observeOnlineList()
        observeSyncDevices()

    }

//    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
//        super.onCreateOptionsMenu(menu)
//        menuInflater.inflate(R.menu.sync_device_menu, menu)
//        return true
//    }
//
//    override fun onOptionsItemSelected(item: MenuItem): Boolean {
//        when (item.itemId) {
//            R.id.select_all -> {
//
//            }
//        }
//        return super.onOptionsItemSelected(item)
//    }

    private fun add() {
        val selectedDevices = ArrayList<Device>()
        for (i in getSelectedItems()) {
            selectedDevices.add(i.value)
        }
        if (selectedDevices.size == 0) {
            Toast.makeText(this, "No device is selected", Toast.LENGTH_SHORT).show()
        } else {
            binding.progressView.visibility = View.VISIBLE
//            val people = AttributeType.getAttributeData()
//            viewModel.synFace(people, selectedDevices)
            viewModel.synFace(selectedDevices)
        }
    }

    private fun observeOnlineList() {
        viewModel.onlineList.observe(this) { list ->
            if (list.isNotEmpty()) {
                Log.d(TAG, "[Enter] observeOnlineList size: ${list.size}")
//                binding.progressView.visibility = View.INVISIBLE
                (recyclerView.adapter as DeviceSyncAdapter).setList(list)
            }
            binding.progressView.visibility = View.INVISIBLE
        }
    }

    private fun observeSyncDevices() {
        viewModel.syncResult.observe(this) {
            binding.progressView.visibility = View.INVISIBLE
            Toast.makeText(this, getString(R.string.create_successfully), Toast.LENGTH_SHORT).show()
            if (viewModel.syncFailedList.isEmpty()) {
                setResult(RESULT_OK)
                finish()
            } else {
                AlertDialog.Builder(this)
                    .setTitle(R.string.dialog_sync_device_title)
                    .setMessage(getString(R.string.dialog_sync_device_message) + getFailedDevices())
                    .setPositiveButton(getString(R.string.ok)) { dialog, _ ->
                        dialog.dismiss()
                        setResult(RESULT_OK)
                        finish()
                    }
                    .setCancelable(false)
                    .create()
                    .show()
            }
        }
    }

    private fun getFailedDevices(): String {
        val text = StringBuilder()
        for (deviceId in viewModel.syncFailedList) {
            Log.d(TAG, "deviceId: $deviceId")
            text.append("\n").append(deviceId)
        }
        Log.d(TAG, "text: $text")
        return text.toString()
    }

    private fun getSelectedItems(): HashMap<Int, Device> {
        return (recyclerView.adapter as DeviceSyncAdapter).selectedItems
    }

}