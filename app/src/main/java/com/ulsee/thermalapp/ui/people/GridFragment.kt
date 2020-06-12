package com.ulsee.thermalapp.ui.people

import android.app.Activity.RESULT_OK
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.MainActivityTag
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Face
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import com.ulsee.thermalapp.data.services.TCPClient
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport
import java.io.Serializable

class GridFragment : Fragment() {

    val REQUEST_CODE_ACTIVITY_EDITOR = 1234

    lateinit var recyclerView: RecyclerView
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_people_grid, container, false)
        recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = PeopleListAdapter()
        recyclerView.layoutManager = GridLayoutManager(context, 3)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, view ->
            val people = (recyclerView.adapter as PeopleListAdapter).getList()[position]
            openEditor(people)
        }

        loadPeopleList();

        root.findViewById<View>(R.id.fab).setOnClickListener { openEditor() }

        (activity as MainActivity).setTitle("People Management")

        return root
    }

    private fun openEditor (face: Face? = null) {
        val intent = Intent(context, EditorActivity::class.java)
        if (face != null) {
            intent.putExtra("people", face!! as Serializable)
        }
        startActivityForResult(intent, REQUEST_CODE_ACTIVITY_EDITOR)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == REQUEST_CODE_ACTIVITY_EDITOR) {
            if (resultCode == RESULT_OK) {
                loadPeopleList()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun loadPeopleList () {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(context, "no connected device", Toast.LENGTH_LONG).show()
            return
        }
        PeopleServiceTCP(selectedTCPClient).getAll()
            .subscribe({ faceList: List<Face> ->
                (recyclerView.adapter as PeopleListAdapter).setList(faceList)
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }

}
