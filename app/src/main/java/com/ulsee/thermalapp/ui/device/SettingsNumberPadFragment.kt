package com.ulsee.thermalapp.ui.device

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.EditText
import android.widget.TextView
import android.widget.Toast
import androidx.fragment.app.Fragment
import com.ulsee.thermalapp.R

class SettingsNumberPadFragment : Fragment() {

    var initValue = ""
    var input : TextView? = null

    interface OnChangedListener {
        fun onChanged(value: Double)
    }
    var onChangedListener : OnChangedListener? = null

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_settings_numberpad, container, false)
        input = root.findViewById(R.id.textView)
        if (initValue.isNotEmpty()) input!!.text = initValue

        root.findViewById<View>(R.id.imageButton_up).setOnClickListener {
            var value = 0.0
            try {
                value = getValue()
            } catch (e: Exception) {
            }
            setValue(value + 0.1)
        }
        root.findViewById<View>(R.id.imageButton_down).setOnClickListener {
            var value = 0.0
            try {
                value = getValue()
            } catch (e: Exception) {
            }
            setValue(value - 0.1)
        }
        return root
    }

    fun getValue () : Double {
        if (input == null) return initValue.toDouble()
        return input!!.text.toString().toDouble()
    }

    fun setValue (value: Double) {
        // 避免小數過多......
        if (input == null) {
            initValue = "%.1f".format(value)
        } else {
            input!!.text = "%.1f".format(value)
        }
        onChangedListener?.onChanged(value)
    }
}