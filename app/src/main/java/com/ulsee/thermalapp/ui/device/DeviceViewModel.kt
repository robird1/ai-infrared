package com.ulsee.thermalapp.ui.device

import androidx.lifecycle.ViewModel
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.viewModelScope
import com.ulsee.thermalapp.ui.people.DeviceSyncRepository
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

private val TAG = DeviceViewModel::class.java.simpleName

class DeviceViewModel(private val repo: DeviceSyncRepository): com.ulsee.thermalapp.ui.people.DeviceViewModel(repo) {

    fun retryRGBStreaming(RGBStreaming: () -> Unit) {
        viewModelScope.launch {
            delay(3000)
            RGBStreaming()
        }
    }

}

class DeviceFactory(private val repository: DeviceSyncRepository) : ViewModelProvider.Factory {
    override fun <T : ViewModel> create(modelClass: Class<T>): T {
        if (modelClass.isAssignableFrom(DeviceViewModel::class.java)) {
            @Suppress("UNCHECKED_CAST")
            return DeviceViewModel(repository) as T
        }
        throw IllegalArgumentException("Unknown ViewModel class")
    }
}