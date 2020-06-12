package com.ulsee.thermalapp.ui.network

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.WIFIInfo

class WIFIListActivity : AppCompatActivity() {

    lateinit var recyclerView : RecyclerView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_network_wifilist)

        // todo: get wifi list

        recyclerView = findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = WIFIListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(this)

        loadWIFIList()

    }
    private fun loadWIFIList() {
        val results = ArrayList<WIFIInfo>()
        val wifiInfo = WIFIInfo()
        results.add(wifiInfo)
        (recyclerView.adapter as WIFIListAdapter).setList(results)
    }
}