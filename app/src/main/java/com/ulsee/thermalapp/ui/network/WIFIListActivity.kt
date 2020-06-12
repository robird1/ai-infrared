package com.ulsee.thermalapp.ui.network

import android.app.Activity
import android.app.ProgressDialog
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.model.WIFIInfo
import com.ulsee.thermalapp.ui.device.ScanActivity
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport

class WIFIListActivity : AppCompatActivity() {

    val REQUEST_CODE_SWITCH_WIFI = 1234

    lateinit var recyclerView : RecyclerView
    lateinit var mProgressDialog : ProgressDialog

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_network_wifilist)

        mProgressDialog = ProgressDialog(this)
        mProgressDialog.setMessage("connecting...")

        initRecyclerView()
        loadWIFIList()
    }

    private fun initRecyclerView () {
        recyclerView = findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = WIFIListAdapter()
        recyclerView.layoutManager = LinearLayoutManager(this)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, view ->
            val wifiInfo = (recyclerView.adapter as WIFIListAdapter).getList()[position]
            if(!wifiInfo.passwordRequired) {
                connectToWIFI(wifiInfo, null)
            } else {
                askPassword(wifiInfo)
            }
        }
    }

    private fun connectToWIFI(wifiInfo: WIFIInfo, password: String?) {
        wifiInfo.password = password
        mProgressDialog.show()
        // todo ... 送出請求
        // todo ... 監聽回覆
        // todo ... 成功後，切換網路後發送訊息
        switchToWIFI(wifiInfo)
    }

    private fun switchToWIFI(wifiInfo: WIFIInfo) {
        val intent = Intent(this, SettingsActivity::class.java)
        intent.putExtra("wifi", wifiInfo)
        startActivityForResult(intent, REQUEST_CODE_SWITCH_WIFI)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == REQUEST_CODE_SWITCH_WIFI) {
            if (resultCode == RESULT_OK) {
                startActivity(Intent(this, MainActivity::class.java))
                finish()
            } else if (resultCode != Activity.RESULT_CANCELED) {
                Toast.makeText(this, "Error: failed to switch to specified wifi", Toast.LENGTH_LONG).show()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun askPassword (wifiInfo: WIFIInfo) {
        val ctx = this

        val input = EditText(ctx)

        AlertDialog.Builder(ctx)
            .setTitle(wifiInfo.ssid)
            .setMessage("請輸入WIFI密碼")
            .setView(input)
            .setPositiveButton("Submit"
            ) { dialog, whichButton ->
                val result = input.text.toString()
                if (result.isEmpty()) {
                    Toast.makeText(ctx, "請輸入WIFI密碼!", Toast.LENGTH_SHORT).show()
                } else {
                    connectToWIFI(wifiInfo, result)
                }
            }
            .setNegativeButton("Cancel"
            ) { dialog, whichButton ->
                dialog.dismiss()
            }
            .create()
            .show()
    }

    private fun loadWIFIList() {
        val results = ArrayList<WIFIInfo>()
        val wifiInfo = WIFIInfo()
        results.add(wifiInfo)
        (recyclerView.adapter as WIFIListAdapter).setList(results)
    }
}