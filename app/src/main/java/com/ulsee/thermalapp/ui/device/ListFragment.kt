package com.ulsee.thermalapp.ui.device

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Device
import io.realm.Realm
import io.realm.kotlin.where

class ListFragment : Fragment() {

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_device_list, container, false)
        val recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = DeviceListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(context)

        val realm = Realm.getDefaultInstance()
        val results = realm.where<Device>().findAll()
        (recyclerView.adapter as DeviceListAdapter).setList(results)

        return root
    }
}
