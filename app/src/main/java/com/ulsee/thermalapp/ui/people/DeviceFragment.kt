package com.ulsee.thermalapp.ui.people

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.LinearLayoutManager
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.databinding.FragmentProfileDevicesBinding

//open class DeviceFragment: Fragment() {
//    private lateinit var binding: FragmentProfileDevicesBinding
//    private lateinit var viewModel: DeviceViewModel
//
//    override fun onCreateView(
//        inflater: LayoutInflater,
//        container: ViewGroup?,
//        savedInstanceState: Bundle?
//    ): View {
//        binding = FragmentProfileDevicesBinding.inflate(inflater, container, false)
//        viewModel = ViewModelProvider(this, DeviceFactory(DeviceSyncRepository()))
//            .get(DeviceViewModel::class.java)
//
//        observeDeviceList()
//
//        binding.recyclerView.adapter = getAdapter()
//        binding.recyclerView.layoutManager = LinearLayoutManager(context)
//
//        (activity as MainActivity).setTitle(getTitle())
//
//        return binding.root
//    }
//
//    override fun onDestroyView() {
//        super.onDestroyView()
//        viewModel.cancelAllConnectionJobs()
//    }
//
//    open fun getAdapter() = DeviceAdapter(getViewModel())
//
//    open fun getTitle() = "ID Management"
//
//    fun getViewModel() = viewModel
//
//    private fun observeDeviceList() {
//        viewModel.deviceList.observe(viewLifecycleOwner) {
//            (binding.recyclerView.adapter as DeviceAdapter).setList(it)
//        }
//    }
//
//}