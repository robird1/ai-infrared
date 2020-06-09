package com.ulsee.thermalapp.ui.device

import android.content.Intent
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.widget.PopupMenu
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device

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
        var device : Device? = null
        var deviceID = ""
        init {
            val menuBtn = itemView?.findViewById<View>(R.id.btn_menu)

            val popup = PopupMenu(itemView?.context, menuBtn)
            popup.menu.add("a").setTitle("Calibration")
            popup.menu.add("b").setTitle("Device Setting")

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

        fun bind(device: Device) {
            this.device = device
            deviceID = device.getID()
            nameTV?.text = device.getName()
        }
    }
}