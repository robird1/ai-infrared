package com.ulsee.thermalapp.ui.device

import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.viewpager2.widget.ViewPager2
import com.addisonelliott.segmentedbutton.SegmentedButtonGroup
import com.google.android.material.tabs.TabLayout
import com.google.android.material.tabs.TabLayoutMediator
import com.ulsee.thermalapp.MainActivityTag
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Settings

class SettingsActivity : AppCompatActivity() {

    var settings: Settings? = null
    lateinit var segmentedButtonGroup : SegmentedButtonGroup
    lateinit var settingsNumberPadAdapter : SettingsNumberPadAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_settings)

        segmentedButtonGroup = findViewById<SegmentedButtonGroup>(R.id.segmentedButton)

        val tabLayout = findViewById<TabLayout>(R.id.tabLayout)
        val viewPager = findViewById<ViewPager2>(R.id.viewPager)
        settingsNumberPadAdapter = SettingsNumberPadAdapter(this)
        viewPager.adapter = settingsNumberPadAdapter
        TabLayoutMediator(tabLayout, viewPager) { tab, position ->
            tab.text = when (position) {
                0 -> "Alarm"
                1 -> "Deviation"
                else -> null
            }
        }.attach()

        findViewById<View>(R.id.button_save).setOnClickListener { save() }

        fetchSettings()
    }

    private fun showSettings () {
        val segmentPosition = if(settings!!.TemperatureUnit == "C") 0 else 1
        segmentedButtonGroup.setPosition(segmentPosition, false)
        settingsNumberPadAdapter.fragments[0].setValue(settings!!.AlarmThreshold)
        settingsNumberPadAdapter.fragments[1].setValue(settings!!.TemperatureOffset)
    }

    private fun save () {
        settings!!.TemperatureUnit = if(segmentedButtonGroup.position == 0) "C" else "F"
        try {
            settings!!.AlarmThreshold = settingsNumberPadAdapter.fragments[0].getValue()
        } catch (e: Exception) {
            Toast.makeText(this, "數字錯誤!", Toast.LENGTH_SHORT).show()
            segmentedButtonGroup.setPosition(0, true)
            return
        }
        try {
            settings!!.TemperatureOffset = settingsNumberPadAdapter.fragments[1].getValue()
        } catch (e: Exception) {
            Toast.makeText(this, "數字錯誤!", Toast.LENGTH_SHORT).show()
            segmentedButtonGroup.setPosition(1, true)
            return
        }
        updateSettings(settings!!)
    }

    // settings
    private fun fetchSettings () {
        Service.shared.settings.get()
            .subscribe({ settings: Settings ->
                this.settings = settings
                showSettings()
                true
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                finish()
            })
    }

    private fun updateSettings (settings: Settings) {
        Service.shared.settings.update(settings)
            .subscribe({
                Toast.makeText(this, "更新成功!", Toast.LENGTH_LONG).show()
                // finish()
                true
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                finish()
            })
    }

    private fun updateTemperatureUnit (value: String) {
        Service.shared.settings.updateTemperatureUnit(value)
            .subscribe({
                true
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }
}