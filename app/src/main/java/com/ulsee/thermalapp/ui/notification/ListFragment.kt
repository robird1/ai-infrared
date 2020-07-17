package com.ulsee.thermalapp.ui.notification

import android.content.Intent
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
import com.ulsee.thermalapp.data.model.Notification2
import com.ulsee.thermalapp.data.services.NotificationServiceTCP
import com.ulsee.thermalapp.ui.device.DeviceListAdapter
import com.ulsee.thermalapp.ui.people.EditorActivity
import com.ulsee.thermalapp.ui.people.PeopleListAdapter
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport
import java.io.Serializable

class ListFragment  : Fragment() {

    lateinit var recyclerView : RecyclerView
    lateinit var swipeRefreshLayout: SwipeRefreshLayout

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        Log.d("ListFragment", "[Enter] onCreateView")
        val root = inflater.inflate(R.layout.fragment_notification_list, container, false)

        swipeRefreshLayout = root.findViewById<SwipeRefreshLayout>(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadNotifications() }

        recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = NotificationListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(context)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, _ ->
            val notification = (recyclerView.adapter as NotificationListAdapter).getList()[position]
            show(notification)
        }

        loadNotifications()

        (activity as MainActivity).setTitle("Records")

        return root
    }

    private fun loadNotifications() {
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager == null) {
            Toast.makeText(context, "no connected device", Toast.LENGTH_LONG).show()
            return
        }
        Log.i(javaClass.name, "call list")
        NotificationServiceTCP(deviceManager).getAll()
            .subscribe({ notificationList: List<Notification2> ->
                Log.d("ListFragment", "[Enter] OnNext()")

                (activity as MainActivity).setTitle("Records("+notificationList.size+")")
                Log.i(javaClass.name, "got list")
                val sortedList = notificationList.sortedByDescending {
                    it.timeDate.time
                }
                Log.i(javaClass.name, "sort list")
                (recyclerView.adapter as NotificationListAdapter).setList(sortedList)
                swipeRefreshLayout.isRefreshing = false
                Log.i(javaClass.name, "set list")
            }, { error: Throwable ->
                Log.d("ListFragment", "[Enter] OnError()")

                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                swipeRefreshLayout.isRefreshing = false
            })
    }

    private fun show(notification: Notification2) {
        Log.d("ListFragment", "[Enter] show")

        val intent = Intent(context, NotificationActivity2::class.java)
        if (notification != null) {
            intent.putExtra("notification", notification as Serializable)
        }
        startActivity(intent)
    }
}
