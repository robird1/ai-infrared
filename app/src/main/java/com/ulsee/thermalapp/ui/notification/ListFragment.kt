package com.ulsee.thermalapp.ui.notification

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.*
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.FilteredRecord
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport
import java.io.Serializable

class ListFragment  : Fragment() {

    lateinit var recyclerView : RecyclerView
    lateinit var swipeRefreshLayout: SwipeRefreshLayout
//    private val args: ListFragmentArgs by navArgs()

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_notification_list, container, false)

        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadNotifications() }

        recyclerView = root.findViewById(R.id.recyclerView)
        recyclerView.adapter = NotificationListAdapter(requireActivity())
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
        when (item.itemId) {
            android.R.id.home -> {
                findNavController().popBackStack()
                return true
            }
            R.id.filter -> {
                startActivityForResult(Intent(requireContext(), RecordFilterActivity::class.java), REQUEST_CODE_FILTER)
            }
        }
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
//        val deviceManager = Service.shared.getManagerOfDeviceID(args.deviceID)
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager != null) {
            registerCallback(deviceManager)
            try {
                deviceManager.getAllNotifications()

            } catch (e: Exception) {
                swipeRefreshLayout.isRefreshing = false
                showToast(e.localizedMessage!!)
            }

        } else {
            swipeRefreshLayout.isRefreshing = false
            showToast(R.string.toast_no_connected_device)
        }
    }

    private fun loadFilteredRecords(data: FilteredRecord) {
//        val deviceManager = Service.shared.getManagerOfDeviceID(args.deviceID)
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager != null) {
            registerCallback(deviceManager)
            try {
                deviceManager.loadFilteredRecords(data)

            } catch (e: Exception) {
                swipeRefreshLayout.isRefreshing = false
                showToast(e.localizedMessage!!)
            }

        } else {
            swipeRefreshLayout.isRefreshing = false
            showToast(R.string.toast_no_connected_device)
        }

    }

    private fun registerCallback(deviceManager: DeviceManager) {
        deviceManager.setOnGotNotificationListListener(object :
            DeviceManager.OnGotNotificationListListener {
            override fun onGotNotificationList(faceList: List<Notification>) {
                activity?.runOnUiThread {
                    if (activity != null) {
                        (activity as MainActivity).setTitle("${getString(R.string.title_notification)}(${faceList.size})")
                    }

                    val sortedList = faceList.sortedByDescending {
                        it.timeDate.time
                    }
                    (recyclerView.adapter as NotificationListAdapter).setList(sortedList)
                    swipeRefreshLayout.isRefreshing = false

                    deviceManager.setOnGotNotificationListListener(null)
                }
            }
        })
    }

    private fun show(notification: Notification) {
        val intent = Intent(context, NotificationActivity::class.java)
        if (notification != null) {
            intent.putExtra("notification", notification as Serializable)
        }
//        intent.putExtra("device_id", args.deviceID)
        startActivity(intent)
    }

    private fun showToast(msg: String) {
        Toast.makeText(requireContext(), msg, Toast.LENGTH_LONG).show()
    }

    private fun showToast(resId: Int) {
        Toast.makeText(requireContext(), getString(resId), Toast.LENGTH_LONG).show()
    }


    companion object {
        const val REQUEST_CODE_FILTER = 777
        const val KEY_INTENT_FILTER = "filter_vales"
    }

}
