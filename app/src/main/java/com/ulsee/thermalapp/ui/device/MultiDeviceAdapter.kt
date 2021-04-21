package com.ulsee.thermalapp.ui.device

import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.lifecycle.LifecycleOwner
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.services.SettingsServiceTCP
import io.reactivex.disposables.Disposable

private val TAG = MultiDeviceAdapter::class.java.simpleName

class MultiDeviceAdapter(private val deviceList: List<Device>, private val viewModel: DeviceViewModel): RecyclerView.Adapter<MultiDeviceAdapter.ViewHolder>() {
    private val viewHolderList = ArrayList<ViewHolder>()

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        Log.d(TAG, "[Enter] onCreateViewHolder()")

        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_multi_device_live_view, parent, false)
        val viewHolder = ViewHolder(view)
        viewHolderList.add(viewHolder)
        return viewHolder
    }

    override fun onBindViewHolder(holder: ViewHolder, position: Int) = holder.bind(deviceList[position])
//override fun onBindViewHolder(holder: ViewHolder, position: Int) {
//    Log.d(TAG, "[Enter] onBindViewHolder() position: $position")
//    holder.bind(deviceList[position])
//}

    override fun getItemCount() = deviceList.size

    fun destroy() {
        for (viewHolder in viewHolderList)
            viewHolder.destroy()
    }

    inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val deviceNameTV = itemView.findViewById<TextView>(R.id.deviceName)
        private val surfaceView =  itemView.findViewById(R.id.streamingSurfaceView) as StreamingSurfaceView
        private val connectedView = itemView.findViewById<View>(R.id.view_connected)
        private val notConnectedView = itemView.findViewById<View>(R.id.view_not_connected)
        private var device: Device? = null
        private var disposable: Disposable? = null

        init {
            observeConnectionStatus()
        }

        fun bind(device: Device) {
            surfaceView.clear()
            this.device = device
            deviceNameTV.text = device.getName()
            startRGBStreaming()
            viewModel.getConnectionStatus(device)
        }

        fun destroy() {
            if(disposable?.isDisposed == false)
                disposable?.dispose()
        }

        private fun startRGBStreaming() {
            if(disposable?.isDisposed == false)
                disposable?.dispose()

            val deviceManager = device?.let { Service.shared.getManagerOfDeviceID(it.getID()) }

            disposable = SettingsServiceTCP(deviceManager!!).openRGBStream2().subscribe({ pair ->
                val base64Image = pair.first
                val deviceId = pair.second

                if (deviceId == device!!.getID()) {
                    surfaceView.draw(base64Image)
                }

            }, {
                Log.d(TAG, "[Enter] Exception e.message: ${it.message}")
                retry()
            })
        }

        private fun retry() {
            viewModel.retryRGBStreaming(this::startRGBStreaming)
        }

        private fun observeConnectionStatus() {
            val lifecycleOwner = itemView.context as LifecycleOwner
            viewModel.deviceStatus.observe(lifecycleOwner) { response ->
                if (response.deviceID == device?.getID()) {
                    displayConnectionStatus(response.isConnected)
                }
            }
        }

        private fun displayConnectionStatus(isConnected: Boolean) {
            connectedView.visibility = if (isConnected) View.VISIBLE else View.GONE
            notConnectedView.visibility = if (!isConnected) View.VISIBLE else View.GONE
        }

    }
}