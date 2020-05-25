package com.ulsee.thermalapp.ui.device

import android.content.Intent
import android.os.Bundle
import android.view.MotionEvent
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.ulsee.thermalapp.R

class StarterActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_starter)

        findViewById<Button>(R.id.button).setOnClickListener { startScan() }
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        startScan()
        return true
    }

    fun startScan () {
        startActivity(Intent(this, ScanActivity::class.java))
        finish()
    }
}