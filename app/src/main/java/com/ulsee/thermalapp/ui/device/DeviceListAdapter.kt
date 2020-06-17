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
        init {
            val menuLayout = itemView?.findViewById<View>(R.id.layout_menu)
            val menuBtn = itemView?.findViewById<View>(R.id.btn_menu)

            mPopup = PopupMenu(itemView?.context, menuBtn)
            mPopup.menu.add("a").setTitle("Calibration")
            mPopup.menu.add("b").setTitle("Device Setting")
            mPopup.menu.add("c").setTitle("WIFI Setting")
            mPopup.menu.add("d").setTitle("Remove")

            mPopup.setOnMenuItemClickListener{ item: MenuItem? ->
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
                    "WIFI Setting" -> {
                        val intent = Intent(itemView.context, WIFIListActivity::class.java)
                        intent.putExtra("device", deviceID)
                        itemView.context.startActivity(intent)
                    }
                    "Remove" -> {
                        val ctx = itemView.context

                        if (device == null) {
                            Toast.makeText(ctx, "Error: no device specified", Toast.LENGTH_SHORT).show()
                        } else {
                            AlertDialog.Builder(ctx)
                                .setMessage("確定刪除此裝置?")
                                .setPositiveButton("刪除"
                                ) { dialog, whichButton ->
                                    // 1. delete
                                    val realm = Realm.getDefaultInstance()
                                    realm.beginTransaction()
                                    val rows = realm.where(RealmDevice::class.java).equalTo("mID", device!!.getID()).findAll()
                                    rows.deleteAllFromRealm()
                                    realm.commitTransaction()
                                    // 2. broadcast
                                    ctx.sendBroadcast(Intent("Device removed"))
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
                true
            }

            menuBtn.setOnClickListener {
                Log.i(javaClass.name, "on menu button clicked")
                mPopup.show()
            }
            menuLayout.setOnClickListener{
                Log.i(javaClass.name, "on menu layout clicked")
                mPopup.show()
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

            hintTV.setText(if(isConnected) "裝置已經連線\n點擊可觀看即時畫面" else "裝置尚未連線...")
        }
    }
}