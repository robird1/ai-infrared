package com.ulsee.thermalapp

import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Settings

val MainActivityTag = "MainActivity"

class MainActivity : AppCompatActivity() {

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
                R.id.navigation_device, R.id.navigation_people, R.id.navigation_settings))
        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)

        updateTemperatureUnit("C")
        fetchSettings()
    }

    // settings
    private fun fetchSettings () {
        Service.shared.settings.get()
            .subscribe({ settings: Settings ->
                print(settings)
                true
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }

    private fun updateTemperatureUnit (value: String) {
        Service.shared.settings.updateTemperatureUnit(value)
            .subscribe({
                true
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }
}
