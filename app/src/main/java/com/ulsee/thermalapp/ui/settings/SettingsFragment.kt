package com.ulsee.thermalapp.ui.settings

import android.content.Context
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.SwitchCompat
import androidx.fragment.app.Fragment
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.AppPreference

class SettingsFragment  : Fragment() {

    lateinit var notificationSwitch: SwitchCompat

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_settings, container, false)
        (activity as MainActivity).setTitle("Settings")

        notificationSwitch = root.findViewById(R.id.switch_notification)
        notificationSwitch.isChecked = AppPreference(requireContext().getSharedPreferences("app", Context.MODE_PRIVATE)).isFeverNotificationEnabled()
        notificationSwitch.setOnCheckedChangeListener { _, isChecked ->
            AppPreference(requireContext().getSharedPreferences("app", Context.MODE_PRIVATE)).setIsFeverNotificationEnabled(isChecked)
        }
        return root
    }
}