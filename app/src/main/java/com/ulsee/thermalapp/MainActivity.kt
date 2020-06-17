package com.ulsee.thermalapp

import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.ui.notification.NotificationActivity
import com.ulsee.thermalapp.ui.tutorial.TutorialStep1Activity
import com.ulsee.thermalapp.utils.NotificationCenter
import java.util.*

val MainActivityTag = "MainActivity"

class MainActivity : AppCompatActivity() {

    var timer : Timer? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val toolbar = findViewById<Toolbar>(R.id.toolbar)
        setSupportActionBar(toolbar);

        val navView: BottomNavigationView = findViewById(R.id.nav_view)

        val navController = findNavController(R.id.nav_host_fragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        val appBarConfiguration = AppBarConfiguration(setOf(
                R.id.navigation_device, R.id.navigation_people, R.id.navigation_notification, R.id.navigation_settings))
        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)

        keepCheckingTutorialDevice()
        keepCheckingNotification()
    }

    override fun onResume() {
        super.onResume()
        timer = Timer()
        keepCheckingTutorialDevice()
    }

    override fun onPause() {
        timer?.cancel()
        timer = null
        super.onPause()
    }

    fun setTitle (title: String) {
        val toolbar = findViewById<Toolbar>(R.id.toolbar)
        toolbar.findViewById<TextView>(R.id.textView_toolbar_title).setText(title)
    }

    fun keepCheckingTutorialDevice () {
        timer?.schedule(object: TimerTask() {
            override fun run() {
//                Log.i(javaClass.name, "Service.shared.tutorialDeviceID = "+Service.shared.tutorialDeviceID)
                if (Service.shared.tutorialDeviceID != null) {
                    startActivity(Intent(this@MainActivity, TutorialStep1Activity::class.java))
                }
            }
        }, 1000, 1000)
    }

    fun keepCheckingNotification () {
        Thread(Runnable {
            if(!isFinishing) {
                for(deviceManager in Service.shared.deviceManagerList) {
                    if (!deviceManager.haveOnNotificationListener) {
                        deviceManager.setOnNotificationListener(object: DeviceManager.OnNotificationListener{
                            override fun onNotification(notification: Notification) {
                                if (!AppPreference(this@MainActivity.getSharedPreferences("app", Context.MODE_PRIVATE)).isFeverNotificationEnabled()) return
                                val intent = Intent(this@MainActivity, NotificationActivity::class.java)
                                intent.putExtra("notification", notification)
                                NotificationCenter.shared.show(this@MainActivity, intent,"通知",notification.Name)
                            }
                        })
                    }
                }
            }
        }).start()
    }
}
