package com.ulsee.thermalapp.ui.device

import android.content.Intent
import android.util.Log
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.widget.PopupMenu
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.RealmDevice
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.data.services.SettingsServiceTCP
import com.ulsee.thermalapp.ui.network.WIFIListActivity
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
        private val hintTV = itemView?.findViewById<TextView>(R.id.textView_hint)
        private val connectedView = itemView?.findViewById<View>(R.id.view_connected)
        private val notConnectedView = itemView?.findViewById<View>(R.id.view_not_connected)
        private lateinit var mPopup : PopupMenu
        var device : Device? = null
        var deviceID = ""
        private val ctx = itemView.context

        init {
            val menuLayout = itemView?.findViewById<View>(R.id.layout_menu)
//            val menuBtn = itemView?.findViewById<View>(R.id.btn_menu)
//            menuBtn.bringToFront()

            mPopup = PopupMenu(ctx, menuLayout)
            mPopup.menu.add(1, 0, 0, R.string.fragment_device_menu_setting)
//            mPopup.menu.add(1, 1, 1, R.string.fragment_device_menu_wifi)
            mPopup.menu.add(1, 1, 1, R.string.fragment_device_menu_remove)

            mPopup.setOnMenuItemClickListener{ item: MenuItem? ->
                when (item!!.order) {
                    0 -> {
                        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
                        SettingsServiceTCP(deviceManager!!).getSettings()
                            .subscribe({
                                val intent = Intent(ctx, SettingsActivity::class.java)
                                intent.putExtra("device", deviceID)
                                ctx.startActivity(intent)

                            }, { error: Throwable ->
                                error.printStackTrace()
                                Toast.makeText(ctx, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                            })

                    }
//                    1 -> {
//                        val deviceManager = Service.shared.getManagerOfDevice(device!!)
//                        deviceManager?.let {
//                            if (!it.tcpClient.isConnected()) {
//                                Toast.makeText(ctx, R.string.fragment_device_toast, Toast.LENGTH_SHORT).show()
//                            } else {
//                                val intent = Intent(ctx, WIFIListActivity::class.java)
//                                intent.putExtra("device", deviceID)
//                                ctx.startActivity(intent)
//                            }
//                        }
//                    }
                    1 -> {
                        if (device == null) {
                            Toast.makeText(ctx, R.string.toast_no_specified_device, Toast.LENGTH_SHORT).show()
                        } else {
                            AlertDialog.Builder(ctx)
                                .setMessage(ctx.getString(R.string.confirm_remove_device))
                                .setPositiveButton(ctx.getString(R.string.remove)
                                ) { dialog, whichButton ->
                                    // 1. delete
                                    val realm = Realm.getDefaultInstance()
                                    realm.beginTransaction()
                                    val rows = realm.where(RealmDevice::class.java).equalTo("mID", device!!.getID()).findAll()
                                    rows.deleteAllFromRealm()
                                    realm.commitTransaction()
                                    realm.close()
                                    // 2. broadcast
                                    val intent = Intent("Device removed")
                                    intent.putExtra("device_id", device!!.getID())
                                    ctx.sendBroadcast(intent)
                                }
                                .setNegativeButton(R.string.cancel
                                ) { dialog, whichButton ->
                                    dialog.dismiss()
                                }
                                .create()
                                .show()
                        }
                    }
                }
                true
            }

//            menuBtn.setOnClickListener {
//                Log.i(javaClass.name, "on menu button clicked")
//                mPopup.show()
//            }
            menuLayout.setOnClickListener{
                Log.i(javaClass.name, "on menu layout clicked")
                mPopup.show()
            }
//            itemView?.findViewById<View>(R.id.layout_header).setOnClickListener{
//                Log.i(javaClass.name, "on menu header clicked")
//                mPopup.show()
//            }

            val thumbLayout = itemView?.findViewById<View>(R.id.layout_thumb)
            thumbLayout.setOnClickListener(object: View.OnClickListener{
                override fun onClick(v: View?) {
                    if (device == null) {
                        Toast.makeText(v?.context, R.string.toast_no_specified_device, Toast.LENGTH_SHORT).show()
                        return
                    }
                    val deviceManager = Service.shared.getManagerOfDevice(device!!)
                    if (deviceManager == null){
                        Toast.makeText(v?.context, "manager null", Toast.LENGTH_SHORT).show()
                        return
                    }
                    if (!deviceManager.tcpClient.isConnected()){
                        Toast.makeText(v?.context, R.string.fragment_device_toast, Toast.LENGTH_SHORT).show()
                        return
                    }

                    val intent = Intent(ctx, StreamingActivity::class.java)
                    intent.putExtra("device", deviceID)
                    ctx.startActivity(intent)
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
            disposable = deviceManager?.subscribeConnectionStatus()?.subscribe{
                displayConnectionStatus(it == DeviceManager.Status.connected)
            }
        }

        fun displayConnectionStatus(isConnected: Boolean) {
            connectedView.visibility = if(isConnected) View.VISIBLE else View.GONE
            notConnectedView.visibility = if(!isConnected) View.VISIBLE else View.GONE

            hintTV.text = if(isConnected) ctx.getString(R.string.device_connected_click_to_watch_camera) else ctx.getString(R.string.device_not_connected_yet)
        }
    }
}