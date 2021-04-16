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
import com.ulsee.thermalapp.data.response.NotificationImage
import com.ulsee.thermalapp.data.services.DeviceManager

class NotificationActivity : AppCompatActivity() {
    private val deviceManager: DeviceManager? by lazy {
        intent.getStringExtra("device_id")?.let {
            Service.shared.getManagerOfDeviceID(it)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.d("NotificationActivity", "[Enter] onCreate")
        setContentView(R.layout.activity_notification)

        if (!intent.hasExtra("notification")) {
            Toast.makeText(this, R.string.activity_notification_toast1, Toast.LENGTH_LONG).show()
            finish()
            return
        }

        val notification = intent.getSerializableExtra("notification") as Notification

        if (deviceManager == null) {
            Toast.makeText(this, R.string.toast_no_connected_device, Toast.LENGTH_SHORT).show()
            return
        }

        val iv = findViewById<ImageView>(R.id.imageView)
        if (notification.Data.isNotEmpty()) {
            Glide.with(this).load(Base64.decode(notification.Data, Base64.DEFAULT)).into(iv)
        } else {
            val listener = object: DeviceManager.OnGotNotificationImageListener{
                override fun onNotificationImage(image: NotificationImage): Boolean {
                    if (image.ID == notification.ID) {
                        runOnUiThread {
                            deviceManager!!.removeOnGotNotificationImageListener(this)
                            notification.Data = image.Data
                            Glide.with(this@NotificationActivity).load(Base64.decode(image.Data, Base64.DEFAULT)).into(iv)
                        }
                        return true
                    }
                    return false
                }
            }

            deviceManager!!.addOnGotNotificationImageListener(listener)
            deviceManager!!.getSingleNotification(notification.ID)
        }

        findViewById<TextView>(R.id.textView_name).text = notification.Name
        findViewById<TextView>(R.id.textView_temperature).text = notification.TempValue.toString() + notification.tempratureUnitString
        findViewById<TextView>(R.id.textView_time).text = notification.Time
        findViewById<TextView>(R.id.textView_mask).text = if(notification.IsMask) getString(R.string.record_filter_yes) else getString(R.string.record_filter_no)
    }
}