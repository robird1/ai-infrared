package com.ulsee.thermalapp.ui.device

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import androidx.fragment.app.Fragment
import com.ulsee.thermalapp.R

class SettingsNumberPadFragment : Fragment() {

    var initValue = ""
    var input : EditText? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_settings_numberpad, container, false)
        input = root.findViewById(R.id.editText)
        if (!initValue.isEmpty()) input!!.setText(initValue)
        return root
    }

    fun getValue () : Double {
        return input!!.text.toString().toDouble()
    }

    fun setValue (value: Double) {
        if (input == null) {
            initValue = value.toString()
        } else {
            input!!.setText(value.toString())
        }
    }
}