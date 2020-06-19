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
    val mAutoFinish = false//autoFinish
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
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        settings = deviceManager!!.settings
        initUI(root)
        showSettings()
        addListeners()
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
        val segmentPosition = if(settings!!.TempUnit == 0) 0 else 1
        segmentedButtonGroup.setPosition(segmentPosition, false)
        settingsNumberPadAdapter.fragments[0].setValue(settings!!.TempAlarmValue)
        settingsNumberPadAdapter.fragments[1].setValue(settings!!.Deviation)

        faceRecognitionSwitch.isChecked = settings?.IsFR == true
        flipImageSwitch.isChecked = settings?.IsFlip == true
        onlyROISwitch.isChecked = settings?.IsOnlyROI == true
    }

    private fun addListeners () {

        faceRecognitionSwitch.setOnCheckedChangeListener { _, _ -> save()  }
        flipImageSwitch.setOnCheckedChangeListener { _, _ -> save()  }
        onlyROISwitch.setOnCheckedChangeListener { _, _ -> save()  }
        segmentedButtonGroup.setOnPositionChangedListener {
            // save()
            // 根據溫度單位，自動換算溫度
            if (segmentedButtonGroup.position == 0) {
                val f = settingsNumberPadAdapter.fragments[0].getValue()
                if (f in 95.0..113.0) {
                    val c = (f-32)*5/9
                    settingsNumberPadAdapter.fragments[0].setValue(c)
                } else {
                    save()
                }
            } else {
                val c = settingsNumberPadAdapter.fragments[0].getValue()
                if (c in 35.0..45.0) {
                    val f = c*9/5+32
                    settingsNumberPadAdapter.fragments[0].setValue(f)
                } else {
                    save()
                }
            }
        }

        settingsNumberPadAdapter.fragments[0].onChangedListener = object: SettingsNumberPadFragment.OnChangedListener{
            override fun onChanged(value: Double) {
                save()
            }
        }
        settingsNumberPadAdapter.fragments[1].onChangedListener = object: SettingsNumberPadFragment.OnChangedListener{
            override fun onChanged(value: Double) {
                save()
            }
        }
    }

    private fun save () {
        settings!!.TempUnit = if(segmentedButtonGroup.position == 0) 0 else 1
        try {
            settings!!.TempAlarmValue = settingsNumberPadAdapter.fragments[0].getValue()
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
            Toast.makeText(context, getString(R.string.number_error), Toast.LENGTH_SHORT).show()
            segmentedButtonGroup.setPosition(1, true)
            return
        }
        settings!!.IsFR = faceRecognitionSwitch.isChecked
        settings!!.IsFlip = flipImageSwitch.isChecked
        settings!!.IsOnlyROI = onlyROISwitch.isChecked
        updateSettings(settings!!)
    }

    private fun updateSettings (settings: Settings) {
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        SettingsServiceTCP(deviceManager!!).update(settings)
            .subscribe({
                Toast.makeText(context, getString(R.string.update_successfully), Toast.LENGTH_LONG).show()
                if(mAutoFinish) activity?.finish()
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                if(mAutoFinish) activity?.finish()
            })
    }
}