package com.ulsee.thermalapp.ui.people

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.*
import android.widget.Toast
import androidx.appcompat.widget.SearchView
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.fragment.app.Fragment
import androidx.navigation.fragment.findNavController
import androidx.recyclerview.widget.ItemTouchHelper
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Face
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport


private val TAG = ProfileFragment::class.java.simpleName


class ProfileFragment : Fragment(), RecyclerItemTouchHelper.ItemTouchListener, SearchView.OnQueryTextListener {
    private lateinit var recyclerView: RecyclerView
    private lateinit var swipeRefreshLayout: SwipeRefreshLayout
    private lateinit var mMenuSearchItem: MenuItem
//    private lateinit var mSearchView: PeopleSearchView
    private lateinit var mProgressView: ConstraintLayout
//    private val args: ProfileFragmentArgs by navArgs()

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_people_list, container, false)

        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadPeopleList() }
        recyclerView = root.findViewById(R.id.recyclerView)
        recyclerView.adapter = ProfileAdapter(this)
        recyclerView.layoutManager = LinearLayoutManager(context)
        mProgressView = root.findViewById(R.id.progress_view)
        configItemClickListener()
        setHasOptionsMenu(true)
        loadPeopleList()
        configFabListener(root)

        (activity as MainActivity).setTitle("ID Management")

        return root
    }

    private fun configItemClickListener() {
        ItemTouchHelper(getItemTouchCallback()).attachToRecyclerView(recyclerView)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, _ ->
            val people = (recyclerView.adapter as ProfileAdapter).getList()[position]

            openEditor(people, true)
        }
    }

    private fun configFabListener(root: View) {
        root.findViewById<View>(R.id.fab).setOnClickListener {
            openEditor(Face(), false)
        }
    }

    override fun onCreateOptionsMenu(menu: Menu, inflater: MenuInflater) {
        Log.d(TAG, "[Enter] onCreateOptionsMenu")
        inflater.inflate(R.menu.people_option_menu, menu)
        configSearchView(menu)
//        super.onCreateOptionsMenu(menu, inflater)
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        Log.d(TAG, "[Enter] onOptionsItemSelected")
        if (item.itemId == android.R.id.home) {
            findNavController().popBackStack()
            return true
        }
        return super.onOptionsItemSelected(item)
    }

    override fun onPrepareOptionsMenu(menu: Menu) {
        Log.d(TAG, "[Enter] onPrepareOptionsMenu")
//        configSearchView(menu)
//        super.onPrepareOptionsMenu(menu)
    }

    private fun configSearchView(menu: Menu) {
        mMenuSearchItem = menu.findItem(R.id.my_search)
        // Get the SearchView and set the searchable configuration
        val mSearchView = (mMenuSearchItem.actionView as ProfileSearchView).apply {
            setOnQueryTextListener(this@ProfileFragment)
        }
        mMenuSearchItem.setOnActionExpandListener(mSearchView)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == REQUEST_ACTIVITY_EDITOR) {
            if (resultCode == Activity.RESULT_OK) {
                loadPeopleList()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    override fun onSwiped(viewHolder: RecyclerView.ViewHolder?, direction: Int, position: Int) {
        val deletedItem: Face = (recyclerView.adapter as ProfileAdapter).faceList[position]
        deletePeople(deletedItem, position)
    }

    override fun onQueryTextSubmit(query: String?): Boolean {
        if (query.isNullOrEmpty()) return true

//        val deviceManager = Service.shared.getManagerOfDeviceID(args.deviceID)
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager != null) {
            deviceManager.setOnGotFaceListListener(object: DeviceManager.OnGotFaceListListener{
                override fun onGotFaceList(faceList: List<Face>) {
                    activity?.runOnUiThread {
//                        Log.d(TAG, "[Enter] onGotFaceList() faceList.size: ${faceList.size}")
                        (recyclerView.adapter as ProfileAdapter).setList(faceList)
                        swipeRefreshLayout.isRefreshing = false

                        deviceManager.setOnGotFaceListListener(null)
                    }
                }
            })
            deviceManager.searchProfile(query)

        } else {
            swipeRefreshLayout.isRefreshing = false
            Toast.makeText(context, R.string.toast_no_connected_device, Toast.LENGTH_LONG).show()
        }

        return false
    }

    override fun onQueryTextChange(newText: String?): Boolean {
        return false
    }

    private fun openEditor(face: Face? = null, isEditMode: Boolean) {
        val intent = Intent(context, EditorActivity::class.java)
        if (face != null) {
            if (isEditMode) {
                // TODO refactor
                AttributeType.NAME.isInputValid = true
            }
            AttributeType.setAttributeData(face)
            intent.putExtra("is_edit_mode", isEditMode)
//            intent.putExtra("device_id", args.deviceID)
        }
        startActivityForResult(intent, REQUEST_ACTIVITY_EDITOR)
    }

    private fun loadPeopleList () {
//        Log.d(TAG,"[Enter] loadPeopleList() deviceID: ${args.deviceID}")
//        val deviceManager = Service.shared.getManagerOfDeviceID(args.deviceID)
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager != null) {
            deviceManager.setOnGotFaceListListener(object: DeviceManager.OnGotFaceListListener{
                override fun onGotFaceList(faceList: List<Face>) {
//                    Log.d(TAG,"[Enter] onGotFaceList() faceList.size: ${faceList.size}")

                    activity?.runOnUiThread {
                        (recyclerView.adapter as ProfileAdapter).setList(faceList)
                        swipeRefreshLayout.isRefreshing = false

                        deviceManager.setOnGotFaceListListener(null)
                    }
                }
            })

            try {
                deviceManager.getAllProfiles()
//                Log.d(TAG,"[Enter] deviceManager.getAllProfiles()")

            } catch (e: Exception) {
                swipeRefreshLayout.isRefreshing = false
                showToast(e.localizedMessage!!)
            }

        } else {
            swipeRefreshLayout.isRefreshing = false
            showToast(R.string.toast_no_connected_device)
        }
    }

    private fun deletePeople(face: Face, position: Int) {
//        val deviceManager = Service.shared.getManagerOfDeviceID(args.deviceID)
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager != null) {
            mProgressView.visibility = View.VISIBLE
            try {
                deviceManager.deleteProfile(face)
//                loadPeopleList()
                (recyclerView.adapter as ProfileAdapter).removeItem(position)

                showToast(R.string.remove_successfully)

            } catch (e: Exception) {
                showToast(e.localizedMessage!!)
            }
            mProgressView.visibility = View.INVISIBLE

        } else {
            swipeRefreshLayout.isRefreshing = false
            showToast(R.string.toast_no_connected_device)
        }

    }

    private fun getItemTouchCallback() =
        RecyclerItemTouchHelper(0, ItemTouchHelper.LEFT or ItemTouchHelper.RIGHT, this)


    private fun showToast(msg: String) {
        Toast.makeText(requireContext(), msg, Toast.LENGTH_LONG).show()
    }

    private fun showToast(resId: Int) {
        Toast.makeText(requireContext(), getString(resId), Toast.LENGTH_LONG).show()
    }

//    private fun getTitleText() = "${getString(R.string.profile_fragment_title)} (${args.deviceName})"

    companion object {
        const val REQUEST_ACTIVITY_EDITOR = 1234
//        const val REQUEST_TAKE_PHOTO = 1235
    }

}