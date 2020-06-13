package com.ulsee.thermalapp.ui.tutorial

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.ui.device.CalibrationActivity

class TutorialStep2Activity : AppCompatActivity() {

    val REQUEST_CODE_NEXT_TUTORIAL = 1234
    val REQUEST_CODE_DO_TUTORIAL = 1235

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_tutorial_step2)

        findViewById<View>(R.id.button_next).setOnClickListener{doTutorial()}
    }

    fun doTutorial () {
        val intent = Intent(this, CalibrationActivity::class.java)
        intent.putExtra("device", Service.shared.tutorialDeviceID!!)
        startActivityForResult(intent, REQUEST_CODE_DO_TUTORIAL)
    }

    fun nextTutorial () {
        Service.shared.tutorialDeviceID = null
        val intent = Intent(this, MainActivity::class.java)
        startActivity(intent)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if(requestCode == REQUEST_CODE_DO_TUTORIAL) {
            if (resultCode == RESULT_OK) {
                setResult(RESULT_OK)
                finish()
            } else if (resultCode == Activity.RESULT_CANCELED) {
                Toast.makeText(this, "Canceled", Toast.LENGTH_LONG).show()
            } else {
                Toast.makeText(this, "Error: unknown activity result", Toast.LENGTH_LONG).show()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }
}