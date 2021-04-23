package com.ulsee.thermalapp.ui.tutorial

import android.app.Activity
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.ui.device.StreamingActivity

class TutorialStep1Activity : AppCompatActivity() {

    val REQUEST_CODE_NEXT_TUTORIAL = 1234
    val REQUEST_CODE_DO_TUTORIAL = 1235

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_tutorial_step1)

        findViewById<View>(R.id.button_next).setOnClickListener{doTutorial()}
        findViewById<View>(R.id.imageView).setOnClickListener{doTutorial()}
        findViewById<View>(R.id.layout_next_hint).setOnClickListener{doTutorial()}
    }

    override fun onBackPressed() {
        Toast.makeText(this, "Please complete settings!", Toast.LENGTH_SHORT).show()
    }

    fun doTutorial () {
        val intent = Intent(this, StreamingActivity::class.java)
        intent.putExtra("device", Service.shared.tutorialDeviceID!!)
        intent.putExtra("is_tutorial", true)
        startActivityForResult(intent, REQUEST_CODE_DO_TUTORIAL)
    }

    fun nextTutorial () {
        val intent = Intent(this, TutorialStep2Activity::class.java)
        startActivityForResult(intent, REQUEST_CODE_NEXT_TUTORIAL)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if(requestCode == REQUEST_CODE_DO_TUTORIAL) {
            if (resultCode == RESULT_OK) {
                nextTutorial()
            } else if (resultCode == Activity.RESULT_CANCELED) {
                Toast.makeText(this, "Canceled", Toast.LENGTH_LONG).show()
            } else {
                Toast.makeText(this, "Error: unknown activity result", Toast.LENGTH_LONG).show()
            }
        } else if(requestCode == REQUEST_CODE_NEXT_TUTORIAL) {
            if (resultCode == RESULT_OK) {
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