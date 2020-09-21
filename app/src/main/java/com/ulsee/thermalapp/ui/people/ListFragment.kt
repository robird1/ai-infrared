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
import androidx.recyclerview.widget.ItemTouchHelper
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Face
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import com.ulsee.thermalapp.ui.notification.RecordFilterActivity
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport


private val TAG = "ListFragment"


class ListFragment : Fragment(), RecyclerItemTouchHelper.ItemTouchListener, SearchView.OnQueryTextListener {
    lateinit var recyclerView: RecyclerView
    lateinit var swipeRefreshLayout: SwipeRefreshLayout

    private lateinit var mMenuSearchItem: MenuItem
    private lateinit var mSearchView: PeopleSearchView
    lateinit var mProgressView: ConstraintLayout

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_people_list, container, false)

        swipeRefreshLayout = root.findViewById(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadPeopleList() }
        recyclerView = root.findViewById(R.id.recyclerView)
        recyclerView.adapter = PeopleListAdapter(this)
        recyclerView.layoutManager = LinearLayoutManager(context)
        mProgressView = root.findViewById(R.id.progress_view)

        ItemTouchHelper(getItemTouchCallback()).attachToRecyclerView(recyclerView)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, _ ->
            val people = (recyclerView.adapter as PeopleListAdapter).getList()[position]

            openEditor(people, true)
        }

        setHasOptionsMenu(true)

        loadPeopleList()

        root.findViewById<View>(R.id.fab).setOnClickListener {
            openEditor(Face(), false)
        }

        (activity as MainActivity).setTitle("People Management")

        return root
    }

    override fun onCreateOptionsMenu(menu: Menu, inflater: MenuInflater) {
        Log.d(TAG, "[Enter] onCreateOptionsMenu")
        inflater.inflate(R.menu.people_option_menu, menu)
        configSearchView(menu)
//        super.onCreateOptionsMenu(menu, inflater)
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        Log.d(TAG, "[Enter] onOptionsItemSelected")
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
        val mSearchView = (mMenuSearchItem.actionView as PeopleSearchView).apply {
            setOnQueryTextListener(this@ListFragment)
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
        val deletedItem: Face = (recyclerView.adapter as PeopleListAdapter).faceList[position]
        deletePeople(deletedItem, position)
    }

    override fun onQueryTextSubmit(query: String?): Boolean {
        Log.d(TAG, "[Enter] onQueryTextSubmit")
        if (query.isNullOrEmpty()) return true

        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            swipeRefreshLayout?.isRefreshing = false
            Toast.makeText(context, "no connected device", Toast.LENGTH_LONG).show()
            return true
        }
        Log.d(TAG, "[Enter]  PeopleServiceTCP(selectedTCPClient).search(query)")

        PeopleServiceTCP(selectedTCPClient).search(query)
            .subscribe({ faceList: List<Face> ->
                Log.d(TAG, "[Success]  PeopleServiceTCP(selectedTCPClient).search(query)")

                mProgressView.visibility = View.INVISIBLE
                (recyclerView.adapter as PeopleListAdapter).setList(faceList)
            }, { error: Throwable ->
                Log.d(TAG, "[Failed]  PeopleServiceTCP(selectedTCPClient).search(query)")

                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                mProgressView.visibility = View.INVISIBLE
            })

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
        }
        startActivityForResult(intent, REQUEST_ACTIVITY_EDITOR)
    }

    private fun loadPeopleList () {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            swipeRefreshLayout.isRefreshing = false
            Toast.makeText(context, "no connected device", Toast.LENGTH_LONG).show()
            return
        }
        PeopleServiceTCP(selectedTCPClient).getAll()
            .subscribe({ faceList: List<Face> ->
                (recyclerView.adapter as PeopleListAdapter).setList(faceList)
                swipeRefreshLayout.isRefreshing = false
            }, { error: Throwable ->
                Log.d(TAG, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                swipeRefreshLayout.isRefreshing = false
            })
    }

    fun deletePeople(face: Face, position: Int) {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(requireContext(), "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressView.visibility = View.VISIBLE
        PeopleServiceTCP(selectedTCPClient).delete(face)
            .subscribe({
//                loadPeopleList()
                (recyclerView.adapter as PeopleListAdapter).removeItem(position)

                Toast.makeText(
                    requireContext(),
                    getString(R.string.remove_successfully),
                    Toast.LENGTH_LONG
                ).show()
                mProgressView.visibility = View.INVISIBLE
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(
                    requireContext(),
                    "Error ${error.localizedMessage}",
                    Toast.LENGTH_LONG
                ).show()
                mProgressView.visibility = View.INVISIBLE
            })
    }

    private fun getItemTouchCallback(): ItemTouchHelper.SimpleCallback {
        return RecyclerItemTouchHelper(0, ItemTouchHelper.LEFT or ItemTouchHelper.RIGHT, this)
    }


    companion object {
        const val REQUEST_ACTIVITY_EDITOR = 1234
//        const val REQUEST_TAKE_PHOTO = 1235
    }

}