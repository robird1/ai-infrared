package com.ulsee.thermalapp.ui.notification

import android.os.Bundle
import android.util.Base64
import android.util.Log
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.bumptech.glide.Glide
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.model.Notification2
import com.ulsee.thermalapp.data.services.NotificationServiceTCP

class NotificationActivity2 : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.d("NotificationActivity2", "[Enter] onCreate")
        setContentView(R.layout.activity_notification)

        if (!intent.hasExtra("notification")) {
            Toast.makeText(this, "Error: no notification data", Toast.LENGTH_LONG).show()
            finish()
            return
        }

        val notification = intent.getSerializableExtra("notification") as Notification2


        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager == null) {
            Toast.makeText(this, "Error: no device connected", Toast.LENGTH_SHORT).show()
            return
        }

        val iv = findViewById<ImageView>(R.id.imageView)
        if (notification.Data.isNullOrEmpty() == false) {
            Glide.with(this).load(Base64.decode(notification.Data, Base64.DEFAULT)).into(iv);
        } else {
            NotificationServiceTCP(deviceManager!!).getSingleNotification(notification.ID).subscribe{
                notification.Data = it
                Glide.with(this).load(Base64.decode(it, Base64.DEFAULT)).into(iv);
            }
        }

        findViewById<TextView>(R.id.textView_name).text = notification.Name
        findViewById<TextView>(R.id.textView_temperature).text = notification.TempValue.toString() + notification.tempratureUnitString
        findViewById<TextView>(R.id.textView_time).text = notification.Time
        findViewById<TextView>(R.id.textView_mask).text = if(notification.IsMask) "Yes" else "No"
    }
}