package com.ulsee.thermalapp.ui.people

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.MainActivityTag
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.People

class GridFragment : Fragment() {

    lateinit var recyclerView: RecyclerView
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_people_grid, container, false)
        recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = PeopleListAdapter()
        recyclerView.layoutManager = GridLayoutManager(context, 4)

        loadPeopleList();

        return root
    }

    private fun loadPeopleList () {
        Service.shared.people.getAll()
            .subscribe({ peopleList: List<People> ->
                (recyclerView.adapter as PeopleListAdapter).setList(peopleList)
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }

    private fun deletePeople (id: Int) {
        Service.shared.people.delete(id)
            .subscribe({ _ ->
                print("delete ok")
                true
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }
}
