package com.ulsee.thermalapp.ui.notification

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.ui.people.DeviceAdapter
import com.ulsee.thermalapp.ui.people.DeviceViewModel
import com.ulsee.thermalapp.ui.people.RecordViewHolder

class DeviceAdapter(private val viewModel: DeviceViewModel): DeviceAdapter(viewModel) {
    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecordViewHolder {
        val view =
            LayoutInflater.from(parent.context).inflate(R.layout.item_list_profile_device, parent, false)
        return NotificationViewHolder(view, viewModel)
    }
}

class NotificationViewHolder(itemView: View, viewModel: DeviceViewModel): RecordViewHolder(itemView, viewModel) {
    override fun getNavigateAction()
        = DeviceFragmentDirections.actionToNavigationRecordList(device!!.getID(), device!!.getName())
}