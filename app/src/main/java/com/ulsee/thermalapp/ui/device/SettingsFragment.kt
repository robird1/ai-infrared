package com.ulsee.thermalapp.ui.device

import android.os.Bundle
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.appcompat.widget.SwitchCompat
import androidx.fragment.app.Fragment
import androidx.viewpager2.widget.ViewPager2
import com.addisonelliott.segmentedbutton.SegmentedButtonGroup
import com.google.android.material.tabs.TabLayout
import com.google.android.material.tabs.TabLayoutMediator
import com.ulsee.thermalapp.MainActivityTag
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Settings
import com.ulsee.thermalapp.data.services.SettingsServiceTCP

class SettingsFragment(deviceID: String, autoFinish: Boolean) : Fragment() {

    var settings: Settings? = null
    var deviceID = deviceID
    val mAutoFinish = autoFinish
    lateinit var segmentedButtonGroup : SegmentedButtonGroup
    lateinit var settingsNumberPadAdapter : SettingsNumberPadAdapter
    lateinit var faceRecognitionSwitch: SwitchCompat
    lateinit var flipImageSwitch: SwitchCompat
    lateinit var onlyROISwitch: SwitchCompat

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_device_setting, container, false)
        initUI(root)
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        settings = deviceManager!!.settings
        showSettings()
        return root
    }

    private fun initUI (root: View) {
        faceRecognitionSwitch = root.findViewById(R.id.switch_facerecognition)
        flipImageSwitch = root.findViewById(R.id.switch_flipimage)
        onlyROISwitch =root.findViewById(R.id.switch_onlyroi)

        segmentedButtonGroup = root.findViewById<SegmentedButtonGroup>(R.id.segmentedButton)

        val tabLayout = root.findViewById<TabLayout>(R.id.tabLayout)
        val viewPager = root.findViewById<ViewPager2>(R.id.viewPager)
        settingsNumberPadAdapter = SettingsNumberPadAdapter(this)
        viewPager.adapter = settingsNumberPadAdapter
        TabLayoutMediator(tabLayout, viewPager) { tab, position ->
            tab.text = when (position) {
                0 -> "Alarm"
                1 -> "Deviation"
                else -> null
            }
        }.attach()
        root.findViewById<View>(R.id.button_save).setOnClickListener { save() }
    }

    private fun showSettings () {
        val segmentPosition = if(settings!!.TemperatureUnit == 0) 0 else 1
        segmentedButtonGroup.setPosition(segmentPosition, false)
        settingsNumberPadAdapter.fragments[0].setValue(settings!!.AlarmThreshold)
        settingsNumberPadAdapter.fragments[1].setValue(settings!!.Deviation)
    }

    private fun save () {
        settings!!.TemperatureUnit = if(segmentedButtonGroup.position == 0) 0 else 1
        try {
            settings!!.AlarmThreshold = settingsNumberPadAdapter.fragments[0].getValue()
        } catch (e: Exception) {
            e.printStackTrace()
            Toast.makeText(context, "數字錯誤!", Toast.LENGTH_SHORT).show()
            segmentedButtonGroup.setPosition(0, true)
            return
        }
        try {
            settings!!.Deviation = settingsNumberPadAdapter.fragments[1].getValue()
        } catch (e: Exception) {
            e.printStackTrace()
            Toast.makeText(context, "數字錯誤!", Toast.LENGTH_SHORT).show()
            segmentedButtonGroup.setPosition(1, true)
            return
        }
        settings!!.FaceRecognition = faceRecognitionSwitch.isChecked
        settings!!.FlipImage = flipImageSwitch.isChecked
        settings!!.OnlyROI = onlyROISwitch.isChecked
        updateSettings(settings!!)
    }

    private fun updateSettings (settings: Settings) {
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        SettingsServiceTCP(deviceManager!!).update(settings)
            .subscribe({
                Toast.makeText(context, "更新成功!", Toast.LENGTH_LONG).show()
                if(mAutoFinish) activity?.finish()
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                if(mAutoFinish) activity?.finish()
            })
    }
}