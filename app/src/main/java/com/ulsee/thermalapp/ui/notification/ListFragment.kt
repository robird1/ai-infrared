package com.ulsee.thermalapp.ui.notification

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.MainActivityTag
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.services.NotificationServiceTCP
import com.ulsee.thermalapp.ui.device.DeviceListAdapter

class ListFragment  : Fragment() {

    lateinit var recyclerView : RecyclerView
    lateinit var swipeRefreshLayout: SwipeRefreshLayout

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_notification_list, container, false)

        swipeRefreshLayout = root.findViewById<SwipeRefreshLayout>(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadNotifications() }

        recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = NotificationListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(context)

        loadNotifications()

        (activity as MainActivity).setTitle("Notification")

        return root
    }

    private fun loadNotifications() {
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager == null) {
            Toast.makeText(context, "no connected device", Toast.LENGTH_LONG).show()
            return
        }
        NotificationServiceTCP(deviceManager).getAll()
            .subscribe({ notificationList: List<Notification> ->
                (recyclerView.adapter as NotificationListAdapter).setList(notificationList)
                swipeRefreshLayout.isRefreshing = false
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                swipeRefreshLayout.isRefreshing = false
            })
    }
}
