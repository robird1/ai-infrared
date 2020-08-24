package com.ulsee.thermalapp.ui.device

import android.content.Intent
import android.os.Bundle
import android.view.MotionEvent
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service

class StarterActivity : AppCompatActivity() {

    var isInitiatingScanner = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_starter)

        findViewById<View>(R.id.button).setOnClickListener {
//            startScan()
            AddDeviceController(this).execute()
        }

        Service.shared.isStarterActivity = true
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
//        startScan()
        AddDeviceController(this).execute()
        return true
    }

    fun startScan () {
        if (isInitiatingScanner) return
        isInitiatingScanner = true
        startActivity(Intent(this, ScanActivity::class.java))
        finish()
    }
}