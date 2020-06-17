package com.ulsee.thermalapp.ui.notification

import android.os.Bundle
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Notification

class NotificationActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_notification)

        if (!intent.hasExtra("notification")) {
            Toast.makeText(this, "Error: no notification data", Toast.LENGTH_LONG).show()
            finish()
            return
        }

        val notification = intent.getSerializableExtra("notification") as Notification

        findViewById<TextView>(R.id.textView_name).setText(notification.Name)
    }
}