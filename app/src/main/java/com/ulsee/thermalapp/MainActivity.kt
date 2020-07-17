package com.ulsee.thermalapp

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Base64
import android.util.Log
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.google.gson.Gson
import com.google.gson.reflect.TypeToken
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.model.Notification2
import com.ulsee.thermalapp.data.model.RealmDevice
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.ui.notification.NotificationActivity
import com.ulsee.thermalapp.ui.notification.NotificationActivity2
import com.ulsee.thermalapp.ui.tutorial.TutorialStep1Activity
import com.ulsee.thermalapp.utils.NotificationCenter
import io.realm.Realm
import io.realm.RealmResults
import io.realm.kotlin.where
import org.json.JSONArray
import org.json.JSONObject
import java.util.*


val MainActivityTag = "MainActivity"

class MainActivity : AppCompatActivity() {

    var timer : Timer? = null
    lateinit var navView : BottomNavigationView

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        Log.d("MainActivity", "[Enter] onCreate")
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

        if (containFRInvisible()) {
//            navView.menu.removeItem(R.id.navigation_people)
            navView.menu.findItem(R.id.navigation_people).isVisible = false
        }
        registerReceiver(deviceChangedReceiver, IntentFilter("Device removed"))

        keepCheckingTutorialDevice()
        keepCheckingNotification()

//        val fakeResponse = createNotifyString()
//        doNotify(fakeResponse)
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
        Log.d("MainActivity", "[Enter] onResume")

        timer = Timer()
        keepCheckingTutorialDevice()
        val isFromScanActivity = intent.getBooleanExtra("isFromScanActivity", false)
        Log.d("MainActivity", "isFromScanActivity: $isFromScanActivity")
        if (isFromScanActivity) {
            if (containFRInvisible()) {
//                navView.menu.removeItem(R.id.navigation_people)
                navView.menu.findItem(R.id.navigation_people).isVisible = false
            }
        }
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
                            override fun onNotification(notification: Notification2) {
//                                Log.d("MainActivity", "[Enter] onNotification")
//                                Log.d("MainActivity", "Notification.Name: "+ notification.Name)
//                                Log.d("MainActivity", "Notification.image: "+ notification.Data)
//                                Log.d("MainActivity", "Notification.temprature: "+ notification.TempValue)
//                                Log.d("MainActivity", "Notification.tempratureUnit:　"+ notification.TempUnit)

                                if (!AppPreference(this@MainActivity.getSharedPreferences("app", Context.MODE_PRIVATE)).isFeverNotificationEnabled()) return
                                val intent = Intent(this@MainActivity, NotificationActivity2::class.java)
                                intent.putExtra("notification", notification)

                                var bitImage : Bitmap? = null
                                try {
                                    val decodedString: ByteArray = Base64.decode(notification.Data, Base64.DEFAULT)
                                    bitImage = BitmapFactory.decodeByteArray(
                                        decodedString,
                                        0,
                                        decodedString.size
                                    )
                                } catch(e:Exception) {

                                }

                                Log.d("MainActivity", "NotificationCenter.shared.show")
//                                NotificationCenter.shared.show(this@MainActivity, intent, this@MainActivity.getString(R.string.title_alert_notification),notification.Name, bitImage)
                                NotificationCenter.shared.show2(this@MainActivity, intent, this@MainActivity.getString(R.string.title_alert_notification), notification)
                            }
                        })
                    }
                }
            }
        }).start()
    }

    private fun containFRInvisible() : Boolean {
        val devices: RealmResults<RealmDevice> = Realm.getDefaultInstance().where<RealmDevice>().findAll()
        for (d in devices) {
            if (!d.getIsFRVisible()) {
                return true
            }
        }
        return false
    }

    private fun isAllFRVisible() : Boolean {
        val devices: RealmResults<RealmDevice> = Realm.getDefaultInstance().where<RealmDevice>().findAll()
        for (d in devices) {
            if (!d.getIsFRVisible()) {
                return true
            }
        }
        return false
    }

    fun createNotifyString () : String {
        val obj = JSONObject()
        obj.put("Action", 18)
        obj.put("ID", -1)
        obj.put("Name", "Steve")
        obj.put("TempUnit", 0)
        obj.put("TempValue", 100)
        obj.put("IsMask", false)
        obj.put("Time", "2020-06-20 12:02:21")
        obj.put("Data", "asdfaheasdfasdfafs")
        return obj.toString()
    }

    fun doNotify(responseString: String) {
        Log.d("MainActivity", "[Enter] doNotify")

        val itemType = object : TypeToken<Notification2>() {}.type
        val gson = Gson()
        try {
            val notification = gson.fromJson<Notification2>(responseString, itemType)
            Log.d("MainActivity", "Notification.ID: "+ notification.ID)
            Log.d("MainActivity", "Notification.Name: "+ notification.Name)
            Log.d("MainActivity", "Notification.Data: "+ notification.Data)
            Log.d("MainActivity", "Notification.temprature: "+ notification.TempValue)
            Log.d("MainActivity", "Notification.tempratureUnit:　"+ notification.TempUnit)
            Log.d("MainActivity", "Notification.IsMask:　"+ notification.IsMask)
            Log.d("MainActivity", "Notification.Time:　"+ notification.Time)

            if (!AppPreference(this@MainActivity.getSharedPreferences("app", Context.MODE_PRIVATE)).isFeverNotificationEnabled()) return
            val intent = Intent(this@MainActivity, NotificationActivity2::class.java)
            intent.putExtra("notification", notification)

            Log.d("MainActivity", "[Enter] before NotificationCenter.shared.show2")
            NotificationCenter.shared.show2(this@MainActivity, intent, this@MainActivity.getString(R.string.title_alert_notification), notification)
            Log.d("MainActivity", "[Enter] after NotificationCenter.shared.show2")


        } catch(e: java.lang.Exception) {
            e.printStackTrace()
        }
    }

}
