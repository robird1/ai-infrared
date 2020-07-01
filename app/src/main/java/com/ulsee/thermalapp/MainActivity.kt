package com.ulsee.thermalapp

import android.app.SearchManager
import android.content.Context
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Base64
import android.util.Log
import android.view.Menu
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.SearchView
import androidx.appcompat.widget.Toolbar
import androidx.fragment.app.FragmentActivity
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

    private var mMenu : Menu? = null

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

        navController.addOnDestinationChangedListener { controller, destination, arguments ->
            mMenu?.getItem(0)?.isVisible = destination.id == R.id.navigation_people
        }

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
        toolbar.findViewById<TextView>(R.id.textView_toolbar_title).text = title
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

                                var bitImage : Bitmap? = null
                                try {
                                    val decodedString: ByteArray = Base64.decode(notification.Image, Base64.DEFAULT)
                                    bitImage = BitmapFactory.decodeByteArray(
                                        decodedString,
                                        0,
                                        decodedString.size
                                    )
                                } catch(e:Exception) {
                                }
                                NotificationCenter.shared.show(this@MainActivity, intent, this@MainActivity.getString(R.string.title_alert_notification),notification.Name, bitImage)
                            }
                        })
                    }
                }
            }
        }).start()
    }

    interface OnSearchListener {
        fun search(query:String?)
    }
    var onSearchListener : OnSearchListener? = null

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        getMenuInflater().inflate(R.menu.menu_search, menu);
        mMenu = menu
        mMenu?.getItem(0)?.isVisible = false
        val searchManager = getSystemService(Context.SEARCH_SERVICE) as SearchManager
        val searchView: SearchView = menu!!.findItem(R.id.search).actionView as SearchView
        searchView.setSearchableInfo(searchManager.getSearchableInfo(componentName))

        searchView.setOnQueryTextListener(object : SearchView.OnQueryTextListener {
            override fun onQueryTextSubmit(query: String?): Boolean {
                onSearchListener?.search(query)
                Log.i(javaClass.name, query)
                Toast.makeText(this@MainActivity, query, Toast.LENGTH_SHORT).show()
                return false
            }

            override fun onQueryTextChange(newText: String?): Boolean {
                Log.i(javaClass.name, newText)
                return false
            }
        })
        searchView.setOnCloseListener {
            Log.i(javaClass.name, "on close")
            onSearchListener?.search(null)
            false
        }
        return true
    }
}
