package com.ulsee.thermalapp.ui.device

import android.content.Context
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.os.Bundle
import android.util.Base64
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.SwitchCompat
import androidx.appcompat.widget.Toolbar
import androidx.fragment.app.FragmentManager
import androidx.fragment.app.FragmentTransaction
import com.bumptech.glide.Glide
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.services.SettingsServiceTCP
import io.reactivex.disposables.Disposable


class StreamingActivity : AppCompatActivity() {
    enum class StreamType() {
        RGB(),
        Thermal()
    }
    lateinit var deviceID : String
    lateinit var thermalSwitch: SwitchCompat
    lateinit var surfaceView: StreamingSurfaceView
    var disposable: Disposable? = null
    var streamType = StreamType.RGB

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_streaming)

        thermalSwitch = findViewById(R.id.switch_thermal)
        thermalSwitch.setOnCheckedChangeListener { buttonView, isChecked ->
            if (isChecked) {
                switchToThermalStreaming()
            } else {
                switchToRGBStreaming()
            }
        }
        surfaceView =  findViewById<StreamingSurfaceView>(R.id.surfaceView) as StreamingSurfaceView

        val toolbar = findViewById<Toolbar>(R.id.toolbar)
        setSupportActionBar(toolbar);

        if(!intent.hasExtra("device")) {
            Toast.makeText(this, "Error: no specified device", Toast.LENGTH_LONG).show()
            finish()
            return
        }
        deviceID = intent.getStringExtra("device")
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        if (deviceManager == null) {
            Toast.makeText(this, "Error: device not found", Toast.LENGTH_LONG).show()
            finish()
            return
        }
        if (deviceManager.settings == null) {
            Toast.makeText(this, "Error: device setting not found", Toast.LENGTH_LONG).show()
            finish()
            return
        }

        findViewById<TextView>(R.id.textView_toolbar_title).text = deviceManager.device.getName()

        if (Service.shared.tutorialDeviceID != null) {
            findViewById<View>(R.id.button_next).visibility = View.VISIBLE
            findViewById<View>(R.id.button_next).setOnClickListener{setResult(RESULT_OK);finish()}
            thermalSwitch.isChecked = true
        }

        if (thermalSwitch.isChecked) {
            startThermalStreaming()
        } else {
            startRGBStreaming()
        }

//        val fragment = SettingsFragment(deviceID, false)
//        val fragmentManager: FragmentManager = supportFragmentManager
//        val fragmentTransaction: FragmentTransaction = fragmentManager.beginTransaction()
//        fragmentTransaction.replace(R.id.frameLayout, fragment)
//        fragmentTransaction.commit()
    }

    override fun finish() {
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        if (streamType == StreamType.RGB) {
            SettingsServiceTCP(deviceManager!!).closeRGBStream().subscribe({}, {})
        } else  {
            SettingsServiceTCP(deviceManager!!).closeThermaltream().subscribe({}, {})
        }
        disposable?.dispose()
        super.finish()
    }

    fun startRGBStreaming() {
        if(disposable?.isDisposed == false)disposable?.dispose()
        streamType = StreamType.RGB
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        disposable = SettingsServiceTCP(deviceManager!!).openRGBStream().subscribe({
            surfaceView.draw(it)
        }, {it.printStackTrace()})
    }
    fun startThermalStreaming() {
        if(disposable?.isDisposed == false)disposable?.dispose()
        streamType = StreamType.Thermal
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        disposable = SettingsServiceTCP(deviceManager!!).openThermaltream().subscribe({
            surfaceView.draw(it)
        }, {it.printStackTrace()})
    }

    fun switchToThermalStreaming () {
        if (streamType == StreamType.Thermal) {
            Log.e(javaClass.name, "Error switch streaming to thermal, but already thermal")
            return
        }
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        SettingsServiceTCP(deviceManager!!).closeRGBStream().subscribe({
            startThermalStreaming()
        }, {
            startThermalStreaming()
        })
    }

    fun switchToRGBStreaming () {
        if (streamType == StreamType.RGB) {
            Log.e(javaClass.name, "Error switch streaming to rgb, but already rgb")
            return
        }
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        SettingsServiceTCP(deviceManager!!).closeThermaltream().subscribe({
            startRGBStreaming()
        }, {
            startRGBStreaming()
        })
    }
}

