package com.ulsee.thermalapp.ui.device

import android.content.Context
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.os.Bundle
import android.util.Base64
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.view.View
import android.widget.ImageView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
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
    lateinit var imageView: ImageView
    var disposable: Disposable? = null
    var streamType = StreamType.RGB

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_streaming)

        imageView = findViewById(R.id.imageView)

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

        // keep draw
        val surfaceView = findViewById<StreamingSurfaceView>(R.id.surfaceView) as StreamingSurfaceView
        disposable = SettingsServiceTCP(deviceManager).openRGBStream().subscribe({
            surfaceView.draw(it)
//                Glide.with(this).load(Base64.decode(it.data, Base64.DEFAULT)).into(imageView)
        }, {})

        if (Service.shared.tutorialDeviceID != null) {
            findViewById<View>(R.id.button_next).visibility = View.VISIBLE
            findViewById<View>(R.id.button_next).setOnClickListener{setResult(RESULT_OK);finish()}
        }
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
}

