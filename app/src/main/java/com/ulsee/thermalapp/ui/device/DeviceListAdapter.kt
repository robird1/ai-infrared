package com.ulsee.thermalapp.ui.device

import android.content.Context
import android.content.Intent
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.widget.PopupMenu
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.data.services.TCPClient
import io.reactivex.disposables.Disposable
import io.realm.Realm

class DeviceListAdapter : RecyclerView.Adapter<DeviceListAdapter.ViewHolder>() {

    var deviceList : List<Device> = ArrayList()
    fun setList(list: List<Device>) {
        deviceList = list
        notifyDataSetChanged()
    }

    override fun getItemCount(): Int = this.deviceList.size

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder?.bind(deviceList[position])
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view = LayoutInflater.from(parent?.context).inflate(R.layout.item_list_device, parent, false)
        val holder = ViewHolder(view)
        return holder
    }

    inner class ViewHolder(itemView: View): RecyclerView.ViewHolder(itemView) {
        private val nameTV = itemView?.findViewById<TextView>(R.id.textView_deviceName)
        private val connectedView = itemView?.findViewById<View>(R.id.view_connected)
        private val notConnectedView = itemView?.findViewById<View>(R.id.view_not_connected)
        var device : Device? = null
        var deviceID = ""
        val isDebug = true
        init {
            val menuBtn = itemView?.findViewById<View>(R.id.btn_menu)

            val popup = PopupMenu(itemView?.context, menuBtn)
            popup.menu.add("a").setTitle("Calibration")
            popup.menu.add("b").setTitle("Device Setting")
            if (isDebug) {
                popup.menu.add("c").setTitle("Modify IP")
            }

            popup.setOnMenuItemClickListener{ item: MenuItem? ->
                when (item!!.title) {
                    "Calibration" -> {
                        val intent = Intent(itemView.context, CalibrationActivity::class.java)
                        intent.putExtra("device", deviceID)
                        itemView.context.startActivity(intent)
                    }
                    "Device Setting" -> {
                        val intent = Intent(itemView.context, SettingsActivity::class.java)
                        intent.putExtra("device", deviceID)
                        itemView.context.startActivity(intent)
                    }
                    "Modify IP" -> {
                        askIP(device!!)
                    }
                }
                true
            }

            menuBtn.setOnClickListener {
                popup.show()
            }

            val thumbLayout = itemView?.findViewById<View>(R.id.layout_thumb)
            thumbLayout.setOnClickListener(object: View.OnClickListener{
                override fun onClick(v: View?) {
                    if (device == null) {
                        Toast.makeText(v?.context, "device null", Toast.LENGTH_SHORT).show()
                        return
                    }
                    val deviceManager = Service.shared.getManagerOfDevice(device!!)
                    if (deviceManager == null){
                        Toast.makeText(v?.context, "manager null", Toast.LENGTH_SHORT).show()
                        return
                    }
                    if (!deviceManager.tcpClient.isConnected()){
                        Toast.makeText(v?.context, "device not connected", Toast.LENGTH_SHORT).show()
                        return
                    }

                    val intent = Intent(itemView.context, StreamingActivity::class.java)
                    intent.putExtra("device", deviceID)
                    itemView.context.startActivity(intent)
                }
            })
        }

        var disposable : Disposable? = null

        fun bind(device: Device) {
            this.device = device
            disposable?.dispose()
            deviceID = device.getID()
            nameTV?.text = device.getName()

            val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
            disposable = deviceManager?.subscribeStatus()?.subscribe{
                displayConnectionStatus(it == DeviceManager.Status.connected)
            }
        }

        fun displayConnectionStatus(isConnected: Boolean) {
            connectedView.visibility = if(isConnected) View.VISIBLE else View.GONE
            notConnectedView.visibility = if(!isConnected) View.VISIBLE else View.GONE
        }

        private fun askIP (device: Device) {
            val ctx = itemView.context
            var message : String? = null
            val input = EditText(ctx)
            input.setText(device.getIP())

            val deviceManager = Service.shared.getManagerOfDeviceID(device.getID())
            if (deviceManager == null) {
                Toast.makeText(ctx, "系統錯誤: 找不到該 Device", Toast.LENGTH_SHORT).show()
                return
            }

            AlertDialog.Builder(ctx)
                .setTitle("請輸入新的IP")
                .setMessage(message)
                .setView(input)
                .setPositiveButton("Save"
                ) { dialog, whichButton ->
                    val realm = Realm.getDefaultInstance()
                    realm.beginTransaction()
                    device.setIP(input.text.toString())
                    realm.commitTransaction()
                    deviceManager.tcpClient = TCPClient(input.text.toString(), 13888)
                    Toast.makeText(ctx, "IP 已經更換為 :"+input.text.toString(), Toast.LENGTH_SHORT).show()
                }
                .setNegativeButton("Cancel"
                ) { dialog, whichButton ->
                    dialog.dismiss()
                }
                .create()
                .show()
        }
    }
}