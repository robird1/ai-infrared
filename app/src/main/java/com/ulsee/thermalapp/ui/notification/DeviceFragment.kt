package com.ulsee.thermalapp.ui.notification

import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.ui.people.DeviceFragment

class DeviceFragment: DeviceFragment() {
    override fun getAdapter() = DeviceAdapter(getViewModel())

    override fun getTitle() = getString(R.string.title_notification)
}