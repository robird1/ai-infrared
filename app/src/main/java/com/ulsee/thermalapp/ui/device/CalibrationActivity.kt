package com.ulsee.thermalapp.ui.device

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Point
import android.graphics.drawable.BitmapDrawable
import android.graphics.drawable.Drawable
import android.os.Bundle
import android.util.Base64
import android.util.Log
import android.util.Size
import android.view.MotionEvent
import android.view.ScaleGestureDetector
import android.view.View
import android.widget.ImageView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.bumptech.glide.Glide
import com.bumptech.glide.load.DataSource
import com.bumptech.glide.load.engine.GlideException
import com.bumptech.glide.request.RequestListener
import com.bumptech.glide.request.target.Target
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.request.UpdateCalibration
import com.ulsee.thermalapp.data.services.SettingsServiceTCP
import kotlin.math.max
import kotlin.math.min

class CalibrationActivity : AppCompatActivity() {

    val Tag = "CalibrationActivity"

    lateinit var rgbIV : ImageView
    lateinit var thermalIV : ImageView
    lateinit var deviceID : String

    var rgbLoaded = false
    var thermalLoaded = false
    var rgbOriginalImageSize  = Size(1000, 1011)
    var thermalOriginalImageSize  = Size(537, 352)
    var rgbSize = Size(0, 0)
    var thermalSize = Size(0, 0)
    var mInitThermalIVSize = Size(0, 0)

    // drag
    var lastPoint = Point()

    // scale
    private var mScaleFactor = 1f
    var mIsScaling = false

    private val scaleListener = object : ScaleGestureDetector.SimpleOnScaleGestureListener() {
        override fun onScale(detector: ScaleGestureDetector): Boolean {
            mScaleFactor *= detector.scaleFactor
//            Log.i(Tag, "onScale = "+(detector.scaleFactor) +", new factor="+mScaleFactor)
            mScaleFactor = max(0.3f, min(mScaleFactor, 5.0f))

            val layoutParams = thermalIV.layoutParams
            layoutParams.width = (mInitThermalIVSize.width * mScaleFactor).toInt()
            layoutParams.height = (mInitThermalIVSize.height * mScaleFactor).toInt()
            thermalIV.layoutParams = layoutParams
            return true
        }

        override fun onScaleBegin(detector: ScaleGestureDetector?): Boolean {
            mIsScaling = true
            return true
        }

        override fun onScaleEnd(detector: ScaleGestureDetector?) {
            mIsScaling = false
            super.onScaleEnd(detector)
        }
    }

    private lateinit var mScaleDetector : ScaleGestureDetector

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_device_calibration)

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

        rgbIV = findViewById(R.id.imageView_rgb)
        thermalIV = findViewById(R.id.imageView_thermal)
        findViewById<View>(R.id.button_save).setOnClickListener { save() }

        mScaleDetector = ScaleGestureDetector(this, scaleListener)

        loadImages()
    }

    private fun loadImages () {
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        SettingsServiceTCP(deviceManager!!).getTwoPicture().subscribe({

            val decodedByte = Base64.decode(it.Data_1, 0);
            val btm: Bitmap? = BitmapFactory.decodeByteArray(decodedByte, 0, decodedByte.size)
            if (btm==null) {
                Toast.makeText(this, "Error: can not get rgb image size", Toast.LENGTH_LONG).show()
                finish()
            } else {
                rgbOriginalImageSize = Size(btm!!.width, btm!!.height)
                Glide.with(this).load(btm)
                    .listener(object : RequestListener<Drawable> {
                        override fun onLoadFailed(p0: GlideException?, p1: Any?, p2: Target<Drawable>?, p3: Boolean): Boolean {
                            Toast.makeText(this@CalibrationActivity, "載入 rgb 圖片異常!", Toast.LENGTH_LONG).show()
                            this@CalibrationActivity.finish()
                            return false
                        }
                        override fun onResourceReady(p0: Drawable?, p1: Any?, p2: Target<Drawable>, p3: DataSource?, p4: Boolean): Boolean {
                            rgbLoaded = true
                            rgbSize = Size((p0 as BitmapDrawable).bitmap.width, p0.bitmap.height)
                            if (rgbLoaded and thermalLoaded) alignImage()
                            return false
                        }
                    }).into(rgbIV)
            }

            val theDecodedByte = Base64.decode(it.Data_2, 0);
            val theBitmap: Bitmap? = BitmapFactory.decodeByteArray(theDecodedByte, 0, theDecodedByte.size)
            if (theBitmap==null) {
                Toast.makeText(this, "Error: can not get thermal image size", Toast.LENGTH_LONG).show()
                finish()
            } else {
                thermalOriginalImageSize = Size(theBitmap!!.width, theBitmap!!.height)
                Glide.with(this).load(theBitmap)
                    .listener(object : RequestListener<Drawable> {
                        override fun onLoadFailed(p0: GlideException?, p1: Any?, p2: Target<Drawable>?, p3: Boolean): Boolean {
                            Toast.makeText(this@CalibrationActivity, "載入 thermal 圖片異常!", Toast.LENGTH_LONG).show()
                            this@CalibrationActivity.finish()
                            return false
                        }
                        override fun onResourceReady(p0: Drawable?, p1: Any?, p2: Target<Drawable>, p3: DataSource?, p4: Boolean): Boolean {
                            thermalLoaded = true
                            thermalSize = Size((p0 as BitmapDrawable).bitmap.width, p0.bitmap.height)
                            if (rgbLoaded and thermalLoaded) alignImage()
                            return false
                        }
                    }).into(thermalIV)
            }
        })

    }

    private fun alignImage () {

        val screenWidth = thermalIV.measuredWidth
        val thermalHeight = thermalSize.height

        val initScale = 0.5
        // 1. 設定thermal圖片寬度是 rgb的一半，高度維持比例
        mInitThermalIVSize = Size((screenWidth*initScale).toInt(), (thermalHeight*initScale).toInt())
        val layoutParams = thermalIV.layoutParams
        layoutParams.width = mInitThermalIVSize.width
        layoutParams.height = mInitThermalIVSize.height
        thermalIV.layoutParams = layoutParams
        // 2. 設定xy 置中
        thermalIV.x = ((screenWidth - mInitThermalIVSize.width)/2).toFloat()
        thermalIV.y = ((rgbSize.height - mInitThermalIVSize.height)/2).toFloat()
    }

    override fun onTouchEvent(event: MotionEvent?): Boolean {
        val pointerCount = event?.pointerCount
//        val result = mScaleDetector.onTouchEvent(event)
//        Log.i(javaClass.name, "onTouchEvent, mScaleDetector return "+result)
//        return result

        val point = Point(event!!.rawX.toInt(), event!!.rawY.toInt())

        when (event!!.action and MotionEvent.ACTION_MASK) {

            MotionEvent.ACTION_DOWN -> {
                // 2. record the last touch point
                if (pointerCount == 1)
                    lastPoint = point
            }
            MotionEvent.ACTION_UP -> {
            }
            MotionEvent.ACTION_POINTER_DOWN -> {
            }
            MotionEvent.ACTION_POINTER_UP -> {
            }
            MotionEvent.ACTION_MOVE -> {
                if (pointerCount == 1) {
                    // 3. get the move offset
                    val offset = Point(point.x - lastPoint.x, point.y - lastPoint.y)
                    thermalIV.x = thermalIV.x + offset.x
                    thermalIV.y = thermalIV.y + offset.y
                    // 4. record the last touch point
                    lastPoint = point
                }
                if (pointerCount == 2) {
                    mScaleDetector.onTouchEvent(event)
                }
            }
        }
        return false
    }

    private fun save () {
        if (rgbIV.measuredWidth==0 || rgbIV.measuredHeight==0) {
            Toast.makeText(this, "Error: image error", Toast.LENGTH_SHORT).show()
            return
        }
        // 1. 取得圖片的位置
        val w = (thermalIV.layoutParams.width) * rgbOriginalImageSize.width / rgbIV.measuredWidth
        val h = (thermalIV.layoutParams.height) * rgbOriginalImageSize.height / rgbIV.measuredHeight
        val x = (thermalIV.x) * rgbOriginalImageSize.width / rgbIV.measuredWidth
        val y = (thermalIV.y) * rgbOriginalImageSize.height / rgbIV.measuredHeight
        updateCalibration(x.toInt(), y.toInt(), w, h)
    }

    private fun updateCalibration (x: Int, y: Int, w: Int, h: Int) {
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceID)
        SettingsServiceTCP(deviceManager!!).calibration(UpdateCalibration(x, y, w, h))
            .subscribe({
                Toast.makeText(this, "更新成功!", Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                // finish()
                true
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                finish()
            })
    }
}