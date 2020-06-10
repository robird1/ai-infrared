package com.ulsee.thermalapp.ui.device

import android.content.Context
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.graphics.Paint
import android.graphics.Rect
import android.util.AttributeSet
import android.util.Base64
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView

class StreamingSurfaceView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null, defStyle: Int = 0) : SurfaceView(context, attrs, defStyle) {

    var lastBase64ImageString : String? = null
    fun draw(base64Image: String) {
        lastBase64ImageString = base64Image
        invalidate()
    }

    init {
        setWillNotDraw(false)

        holder.addCallback(object: SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder, format: Int,
                                        width: Int, height: Int) {
                //setWillNotDraw(false)
            }

            override fun surfaceCreated(holder: SurfaceHolder) {
                //setWillNotDraw(false)
                var canvas: Canvas? = null
                try {
                    canvas = holder.lockCanvas(null)
                    synchronized(holder) { draw(canvas) }
                } catch (e: Exception) {
                    e.printStackTrace()
                } finally {
                    if (canvas != null) {
                        holder.unlockCanvasAndPost(canvas)
                    }
                }
            }

            override fun surfaceDestroyed(holder: SurfaceHolder) {
            }
        })
    }

    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
        if (lastBase64ImageString != null && canvas != null) {
            val decodedString: ByteArray = Base64.decode(lastBase64ImageString!!, Base64.DEFAULT)
            val bitmap = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.size)
            val bitmapRect = Rect(0, 0, bitmap.width, bitmap.height)
            val canvasRect = Rect(0, 0, canvas!!.width, bitmap.height * canvas!!.width / bitmap.width)
            canvas?.drawBitmap(bitmap, bitmapRect, canvasRect, Paint())
            lastBase64ImageString = null
        }
    }

//    override fun onAttachedToWindow() {
//        super.onAttachedToWindow()
//        setWillNotDraw(false)
//    }
}