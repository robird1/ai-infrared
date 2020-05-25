package com.ulsee.thermalapp.ui.device

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
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
        return ViewHolder(view)
    }

    inner class ViewHolder(itemView: View): RecyclerView.ViewHolder(itemView) {
        private val nameTV = itemView?.findViewById<TextView>(R.id.textView_deviceName)

        fun bind(device: Device) {
            nameTV?.text = device.getName()
        }
    }
}