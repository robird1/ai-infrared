package com.ulsee.thermalapp.ui.notification

import android.os.Bundle
import android.util.Base64
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.bumptech.glide.Glide
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.services.NotificationServiceTCP

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


        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager == null) {
            Toast.makeText(this, "Error: no device connected", Toast.LENGTH_SHORT).show()
            return
        }

        val iv = findViewById<ImageView>(R.id.imageView)
        if (notification.Image.isNullOrEmpty() == false) {
            Glide.with(this).load(Base64.decode(notification.Image, Base64.DEFAULT)).into(iv);
        } else {
            NotificationServiceTCP(deviceManager!!).getSingleNotification(notification.Name).subscribe{
                notification.Image = it
                Glide.with(this).load(Base64.decode(it, Base64.DEFAULT)).into(iv);
            }
        }

        findViewById<TextView>(R.id.textView_name).text = notification.peopleName
        findViewById<TextView>(R.id.textView_temperature).text = notification.temprature + notification.tempratureUnit
        findViewById<TextView>(R.id.textView_time).text = notification.createdAt.toString()
        findViewById<TextView>(R.id.textView_mask).text = if(notification.hasMask) "Yes" else "No"

    }
}