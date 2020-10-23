package com.ulsee.thermalapp

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Bundle
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.RealmDevice
import io.realm.Realm
import io.realm.RealmResults
import io.realm.kotlin.where
import java.util.*


val MainActivityTag = "MainActivity"

class MainActivity : AppCompatActivity() {

    var timer : Timer? = null
    lateinit var navView : BottomNavigationView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val toolbar = findViewById<Toolbar>(R.id.toolbar)
        setSupportActionBar(toolbar);

        navView = findViewById(R.id.nav_view)

        val navController = findNavController(R.id.nav_host_fragment)

        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        val appBarConfiguration = AppBarConfiguration(setOf(R.id.navigation_device, R.id.navigation_people, R.id.navigation_notification, R.id.navigation_settings))
        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)

        navController.addOnDestinationChangedListener { _, _, _ ->
            toolbar.collapseActionView()
        }

        if (containFRInvisible()) {
//            navView.menu.removeItem(R.id.navigation_people)
            navView.menu.findItem(R.id.navigation_people).isVisible = false
        }
        registerReceiver(deviceChangedReceiver, IntentFilter("Device removed"))

//        keepCheckingTutorialDevice()

        Service.shared.isStarterActivity = false
    }

    val deviceChangedReceiver = object: BroadcastReceiver(){
        override fun onReceive(context: Context?, intent: Intent?) {
            if (!containFRInvisible()) {
                navView.menu.findItem(R.id.navigation_people).isVisible = true
            }
        }
    }

    override fun onResume() {
        super.onResume()
        timer = Timer()
//        keepCheckingTutorialDevice()
    }

    override fun onPause() {
        timer?.cancel()
        timer = null
        super.onPause()
    }

    override fun onDestroy() {
        unregisterReceiver(deviceChangedReceiver)
        super.onDestroy()
    }

    fun setTitle (title: String) {
        val toolbar = findViewById<Toolbar>(R.id.toolbar)
        toolbar.findViewById<TextView>(R.id.textView_toolbar_title).setText(title)
    }

//    fun keepCheckingTutorialDevice () {
//        timer?.schedule(object: TimerTask() {
//            override fun run() {
////                Log.i(javaClass.name, "Service.shared.tutorialDeviceID = "+Service.shared.tutorialDeviceID)
//                if (Service.shared.tutorialDeviceID != null) {
//                    startActivity(Intent(this@MainActivity, TutorialStep1Activity::class.java))
//                }
//            }
//        }, 1000, 1000)
//    }

    private fun containFRInvisible() : Boolean {
        val devices: RealmResults<RealmDevice> = Realm.getDefaultInstance().where<RealmDevice>().findAll()
        for (d in devices) {
            if (!d.getIsFRVisible()) {
                return true
            }
        }
        return false
    }

}
