package com.ulsee.thermalapp.ui.device

import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.Fragment
import androidx.viewpager2.adapter.FragmentStateAdapter

class SettingsNumberPadAdapter(fragment: Fragment): FragmentStateAdapter(fragment) {

    val fragments = listOf(SettingsNumberPadFragment(), SettingsNumberPadFragment())

    override fun getItemCount() = fragments.size

    override fun createFragment(position: Int): Fragment {
        return fragments[position]
    }
}