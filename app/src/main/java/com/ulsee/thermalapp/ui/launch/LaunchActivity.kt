package com.ulsee.thermalapp.ui.launch

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import java.util.*

class LaunchActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_launch)

        Timer().schedule(object : TimerTask() {
            override fun run() {
                runOnUiThread {
                    startActivity(Intent(this@LaunchActivity, MainActivity::class.java))
                }
            }
        }, 200)
    }

}