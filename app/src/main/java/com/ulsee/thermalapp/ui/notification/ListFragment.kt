package com.ulsee.thermalapp.ui.notification

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.*
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.MainActivityTag
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.FilteredRecord
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.services.NotificationServiceTCP
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport
import java.io.Serializable

private val TAG = ListFragment::class.java.simpleName

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

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, _ ->
            val notification = (recyclerView.adapter as NotificationListAdapter).getList()[position]
            show(notification)
        }

        setHasOptionsMenu(true)

        loadNotifications()

        (activity as MainActivity).setTitle(getString(R.string.title_notification))

        return root
    }

    override fun onCreateOptionsMenu(menu: Menu, inflater: MenuInflater) {
        inflater.inflate(R.menu.record_option_menu, menu)
        super.onCreateOptionsMenu(menu, inflater)
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        startActivityForResult(Intent(requireContext(), RecordFilterActivity::class.java), REQUEST_CODE_FILTER)
        return super.onOptionsItemSelected(item)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == REQUEST_CODE_FILTER) {
            if (resultCode == Activity.RESULT_OK) {
                val item = data?.getParcelableExtra(KEY_INTENT_FILTER) as FilteredRecord

                loadFilteredRecords(item)
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun loadNotifications() {
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager == null) {
            swipeRefreshLayout.isRefreshing = false
            Toast.makeText(context, R.string.toast_no_connected_device, Toast.LENGTH_LONG).show()
            return
        }
        Log.i(javaClass.name, "call list")
        NotificationServiceTCP(deviceManager).getAll()
            .subscribe({ notificationList: List<Notification> ->
                Log.d("ListFragment", "[Enter] OnNext()")

                if (activity != null) {
                    (activity as MainActivity).setTitle("${getString(R.string.title_notification)}(${notificationList.size})")
                }
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
                if (context != null) {
                    Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                }
                swipeRefreshLayout.isRefreshing = false
            })
    }

    private fun loadFilteredRecords(data: FilteredRecord) {
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager == null) {
            swipeRefreshLayout.isRefreshing = false
            Toast.makeText(context, R.string.toast_no_connected_device, Toast.LENGTH_LONG).show()
            return
        }

        NotificationServiceTCP(deviceManager).getFilteredRecord(data)
            .subscribe({ notificationList: List<Notification> ->
                (activity as MainActivity).setTitle("${getString(R.string.title_notification)}(${notificationList.size})")
                val sortedList = notificationList.sortedByDescending {
                    it.timeDate.time
                }
                (recyclerView.adapter as NotificationListAdapter).setList(sortedList)
                swipeRefreshLayout.isRefreshing = false
            }, { error: Throwable ->
                Log.d("ListFragment", "[Enter] OnError()")
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                swipeRefreshLayout.isRefreshing = false
            })

    }

    private fun show(notification: Notification) {
        val intent = Intent(context, NotificationActivity::class.java)
        if (notification != null) {
            intent.putExtra("notification", notification as Serializable)
        }
        startActivity(intent)
    }

    companion object {
        const val REQUEST_CODE_FILTER = 777
        const val KEY_INTENT_FILTER = "filter_vales"
    }

}
