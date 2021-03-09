package com.ulsee.thermalapp.ui.device

import android.os.Bundle
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentTransaction
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service


class SettingsActivity : AppCompatActivity() {

    lateinit var deviceID : String

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_settings)

        if(!intent.hasExtra("device")) {
            Toast.makeText(this, R.string.activity_device_settings_toast1, Toast.LENGTH_LONG).show()
            finish()
            return
        }
        deviceID = intent.getStringExtra("device")
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        if (deviceManager == null) {
            Toast.makeText(this, R.string.activity_device_settings_toast2, Toast.LENGTH_LONG).show()
            finish()
            return
        }
        if (deviceManager.settings == null) {
            Toast.makeText(this, R.string.activity_device_settings_toast3, Toast.LENGTH_LONG).show()
            finish()
            return
        }

        val fragment = SettingsFragment(deviceID, true)
        val fragmentManager: FragmentManager = supportFragmentManager
        val fragmentTransaction: FragmentTransaction = fragmentManager.beginTransaction()
        fragmentTransaction.replace(R.id.frameLayout, fragment)
        fragmentTransaction.commit()
    }
}