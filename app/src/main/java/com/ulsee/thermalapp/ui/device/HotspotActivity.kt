package com.ulsee.thermalapp.ui.device

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.net.wifi.WifiManager
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.data.model.WIFIInfo
import com.ulsee.thermalapp.ui.network.NetworkController
import com.ulsee.thermalapp.ui.network.NetworkUtils
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport

private val TAG = "HotspotActivity"

class HotspotActivity : AppCompatActivity() {

    lateinit var recyclerView: RecyclerView
    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var mProgressView: ConstraintLayout
    private var mHotspotList: ArrayList<WIFIInfo>?= null


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_ipclist)
        val title = findViewById<TextView>(R.id.textView_toolbar_title)
        title.text = "Hotspots"
        initRecyclerView()
        registerWIFIBroadcast ()
//        loadWIFIList()
    }

    override fun onResume() {
        super.onResume()
        loadWIFIList()
    }

    override fun onDestroy() {
        unregisterWIFIBroadcast()
        super.onDestroy()
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == NetworkUtils.REQUEST_LOCATION_SETTINGS) {
            if (resultCode == RESULT_OK) {
                loadWIFIList()
            } else {
                finish()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun initRecyclerView() {
        swipeRefreshLayout = findViewById(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadWIFIList(true) }

        recyclerView = findViewById(R.id.recyclerView)
        recyclerView.adapter = HotspotListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(this)
        mProgressView = findViewById(R.id.progress_view)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, _ ->
            val hotspot = (recyclerView.adapter as HotspotListAdapter).getList()[position]
            startConnect(hotspot)
        }
    }

    private fun startConnect(hotspot: WIFIInfo) {
        if (switchWifi(hotspot)) {
            Log.d(TAG, "[Success] switchWifi()")
            val device = Device()
            device.setID(hotspot.ssid)
            device.setIP("10.42.0.1")
            AddDeviceController(this).askDeviceName(device)
        } else {
            Log.d(TAG, "[Failed] switchWifi()")
        }
    }

    private fun switchWifi(hotspot: WIFIInfo): Boolean {
        hotspot.password = "ulsee168"
        return NetworkController(this).requestWifi(hotspot)
    }

    private fun loadWIFIList(isSwipeRefresh: Boolean = false) {
        if (!isSwipeRefresh)
            mProgressView.visibility = View.VISIBLE

        NetworkUtils.checkLocationSetting(this)

        val wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
        val success = wifiManager.startScan()
        if (!success) {
            swipeRefreshLayout.isRefreshing = false
            mProgressView.visibility = View.INVISIBLE
            Toast.makeText(this, "Failed to scan Wi-Fi", Toast.LENGTH_LONG).show()
            Log.d("WIFIListActivity", "[loadWIFIList] Failed to scan Wi-Fi")
        }
    }

    private fun registerWIFIBroadcast () {
        val intentFilter = IntentFilter(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)
        registerReceiver(wifiScanReceiver, intentFilter)
    }

    private fun unregisterWIFIBroadcast () {
        unregisterReceiver(wifiScanReceiver)
    }

    private val wifiScanReceiver = object : BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            val success = intent.getBooleanExtra(WifiManager.EXTRA_RESULTS_UPDATED, false)
            if (success) {
                val wifiManager = applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
                val results = wifiManager.scanResults
                mHotspotList = ArrayList()
                for (result in results) {
                    if(result.SSID.isNullOrEmpty()) continue

                    if (result.SSID.startsWith("ULS-")) {
                        val wifiInfo = WIFIInfo()
                        wifiInfo.ssid = result.SSID
                        wifiInfo.bssid = result.BSSID
                        wifiInfo.capabilities = result.capabilities
                        mHotspotList!!.add(wifiInfo)
//                        Log.d(TAG, String.format("got Wi-Fi, ssid=%s, bssid=%s, capabilities=%s", result.SSID, result.BSSID, result.capabilities))
                    }
                }

                (recyclerView.adapter as HotspotListAdapter).setList(mHotspotList!!)

                if (results.size == 0) {
                    Log.d(TAG, "[wifiScanReceiver.onReceive] There is no Wi-Fi scanned")
                    Toast.makeText(this@HotspotActivity, "There is no Wi-Fi scanned", Toast.LENGTH_SHORT).show()
                }
            } else {
                Log.d(TAG, "[wifiScanReceiver.onReceive] Failed to scan Wi-Fi")
                Toast.makeText(this@HotspotActivity, "Failed to scan Wi-Fi", Toast.LENGTH_LONG).show()
            }

            mProgressView.visibility = View.INVISIBLE
            swipeRefreshLayout.isRefreshing = false

        }
    }


    class HotspotListAdapter  : RecyclerView.Adapter<HotspotListAdapter.ViewHolder>() {

        private var mHotspotList : List<WIFIInfo> = ArrayList()
        fun setList(list: List<WIFIInfo>) {
            mHotspotList = list
            notifyDataSetChanged()
        }
        fun getList():List<WIFIInfo> {
            return mHotspotList
        }

        override fun getItemCount(): Int = this.mHotspotList.size

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            holder.bind(mHotspotList[position])
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            val view = LayoutInflater.from(parent.context).inflate(R.layout.item_list_ipc, parent, false)
            return ViewHolder(view)
        }

        class ViewHolder(itemView: View): RecyclerView.ViewHolder(itemView) {
            private val nameTV = itemView.findViewById<TextView>(R.id.textView_ipc)

            fun bind(ipc: WIFIInfo) {
                nameTV?.text = ipc.ssid
            }
        }
    }
}
