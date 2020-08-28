package com.ulsee.thermalapp.ui.device

import android.os.Bundle
import android.os.Handler
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport

private val TAG = "IPCListActivity"

class IPCListActivity : AppCompatActivity() {

    lateinit var recyclerView: RecyclerView
    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var mProgressView: ConstraintLayout

    val mDeviceList = ArrayList<Device>() // IP, ID, Timestamp
    var mHandler: Handler?= null


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_ipclist)
        initRecyclerView()
        Service.shared.mDeviceSearchedListener2 = mOnDeviceSearchedListener
        mHandler = Handler()
    }

    val mOnDeviceSearchedListener = object : Service.DeviceSearchedListener2 {
        override fun onNewDevice(device: Device) {
            if (!isDeviceDuplicated(device)) {
                device.setCreatedAt(System.currentTimeMillis())
                mDeviceList.add(device)
            }
        }
    }

    override fun onResume() {
        super.onResume()
        loadIPCList()
    }

    override fun onDestroy() {
        mHandler = null
        Service.shared.mDeviceSearchedListener2 = null
        super.onDestroy()
    }

    private fun isDeviceDuplicated(device: Device): Boolean {
        for (d in mDeviceList) {
            if (d.getID().equals(device.getID()) && d.getIP().equals(device.getIP())) {
                return true
            }
        }
        return false
    }

    private fun initRecyclerView() {
        swipeRefreshLayout = findViewById(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadIPCList(true) }

        recyclerView = findViewById(R.id.recyclerView)
        recyclerView.adapter = IPCListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(this)
        mProgressView = findViewById(R.id.progress_view)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, _ ->
            val device = (recyclerView.adapter as IPCListAdapter).getList()[position]
            startConnect(device)
        }
    }

    private fun startConnect(device: Device) {
        AddDeviceController(this).askDeviceName(device)
    }

    private fun loadIPCList(isSwipeRefresh: Boolean = false) {
        if (!isSwipeRefresh)
            mProgressView.visibility = View.VISIBLE

        mDeviceList.clear()

        mHandler?.postDelayed({
            (recyclerView.adapter as IPCListAdapter).setList(mDeviceList)
            swipeRefreshLayout.isRefreshing = false
            mProgressView.visibility = View.INVISIBLE

        },5000)
    }

    class IPCListAdapter  : RecyclerView.Adapter<IPCListAdapter.ViewHolder>() {

        private var mIPCList : List<Device> = ArrayList()
        fun setList(list: List<Device>) {
            mIPCList = list
            notifyDataSetChanged()
        }
        fun getList():List<Device> {
            return mIPCList
        }

        override fun getItemCount(): Int = this.mIPCList.size

        override fun onBindViewHolder(holder: ViewHolder, position: Int) {
            holder.bind(mIPCList[position])
        }

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
            val view = LayoutInflater.from(parent.context).inflate(R.layout.item_list_ipc, parent, false)
            return ViewHolder(view)
        }

        inner class ViewHolder(itemView: View): RecyclerView.ViewHolder(itemView) {
            private val nameTV = itemView.findViewById<TextView>(R.id.textView_ipc)

            fun bind(ipc: Device) {
                nameTV?.text = ipc.getID()
            }
        }
    }
}
