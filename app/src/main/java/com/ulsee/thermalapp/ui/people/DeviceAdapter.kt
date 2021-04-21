package com.ulsee.thermalapp.ui.people

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import android.widget.Toast
import androidx.lifecycle.LifecycleOwner
import androidx.navigation.findNavController
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Device

//open class DeviceAdapter(private val viewModel: DeviceViewModel) : RecyclerView.Adapter<RecordViewHolder>() {
//
//    private var deviceList: List<Device> = ArrayList()
//    fun setList(list: List<Device>) {
//        deviceList = list
//        notifyDataSetChanged()
//    }
//
//    override fun getItemCount(): Int = this.deviceList.size
//
//    override fun onBindViewHolder(holder: RecordViewHolder, position: Int) {
//        holder.bind(deviceList[position])
//    }
//
//    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecordViewHolder {
//        val view =
//            LayoutInflater.from(parent.context).inflate(R.layout.item_list_profile_device, parent, false)
//        return RecordViewHolder(view, viewModel)
//    }
//}
//
//
//open class RecordViewHolder(itemView: View, private val viewModel: DeviceViewModel) : RecyclerView.ViewHolder(itemView) {
//    private val nameTV = itemView.findViewById<TextView>(R.id.device_name)
//    private val ipTV = itemView.findViewById<TextView>(R.id.device_ip)
//    private val connectedView = itemView.findViewById<View>(R.id.view_connected)
//    private val notConnectedView = itemView.findViewById<View>(R.id.view_not_connected)
//    var device: Device? = null
//    private var deviceID = ""
//
//    init {
//        observeConnectionStatus()
//        configOnClickListener()
//    }
//
//    private fun observeConnectionStatus() {
//        val lifecycleOwner = itemView.context as LifecycleOwner
//        viewModel.deviceStatus.observe(lifecycleOwner) { response ->
//            if (response.deviceID == device?.getID()) {
//                displayConnectionStatus(response.isConnected)
//            }
//        }
//    }
//
//    private fun configOnClickListener() {
//        itemView.setOnClickListener {
//            if (connectedView.visibility == View.VISIBLE) {
////                device?.let { itemView.findNavController().navigate(getNavigateAction()) }
//                itemView.findNavController().navigate(getNavigateAction())
//            } else {
//                Toast.makeText(itemView.context, itemView.context.getString(R.string.device_offline), Toast.LENGTH_SHORT).show()
//            }
//        }
//    }
//
//    fun bind(device: Device) {
//        this.device = device
//        deviceID = device.getID()
//        nameTV?.text = device.getName()
//        ipTV?.text = device.getIP()
//        viewModel.getConnectionStatus(device)
//    }
//
//    private fun displayConnectionStatus(isConnected: Boolean) {
//        connectedView.visibility = if (isConnected) View.VISIBLE else View.GONE
//        notConnectedView.visibility = if (!isConnected) View.VISIBLE else View.GONE
//    }
//
//    open fun getNavigateAction() = DeviceFragmentDirections.actionToNavigationPeopleList(device!!.getID(), device!!.getName())
//
//}
