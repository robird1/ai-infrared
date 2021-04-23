package com.ulsee.thermalapp.ui.people

import android.content.Intent
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.os.SystemClock
import android.provider.MediaStore
import android.util.Base64
import android.util.Log
import android.view.View
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import com.ulsee.facecode.Facecode
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import java.io.InputStream

private val TAG = EditorActivity::class.java.simpleName

class EditorActivity : AppCompatActivity() {
    private lateinit var recyclerView: RecyclerView
    private lateinit var toolbar: Toolbar
    private lateinit var mAddFaceBtn: ImageView
    private lateinit var mProgressView: ConstraintLayout
    private var isPhotoTakenOrSelected: Boolean = false
    private var mImageBase64: String = ""                 // The value is not empty in the following two cases: 1. add a face 2. modify the existing face
    private val isEditingMode : Boolean
        get() {
            return intent.getBooleanExtra("is_edit_mode", true)
        }
    private var lastClickTime: Long = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        Log.d(TAG, "[Enter] onCreate")

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_people_editor2)
        toolbar = findViewById(R.id.toolbar)
        setSupportActionBar(toolbar)

        recyclerView = findViewById(R.id.recyclerView)
        recyclerView.adapter = EditorAdapter(this, isEditingMode)
        recyclerView.layoutManager = LinearLayoutManager(this)
        mProgressView = findViewById(R.id.progress_view)
        mAddFaceBtn = findViewById(R.id.add_image)

        mAddFaceBtn.setOnClickListener {
            pickImageFromAlbum()
        }

        findViewById<View>(R.id.save_btn).setOnClickListener {
            addOrEdit()
        }

        if (isEditingMode) {
            findViewById<TextView>(R.id.textView_toolbar_title).text = "Edit Profile"
            mAddFaceBtn.setImageResource(R.drawable.face_code_image)
        }


        //  init FR
        if (!Service.shared.isULSeeFaceVerificationManagerInited) {
            mProgressView.visibility = View.VISIBLE
            Thread {
                val initResult =
                    Service.shared.ULSeeFaceVerificationManager?.init("==============$$$================")
                runOnUiThread {
                    if (initResult != 0) {
                        Toast.makeText(this, R.string.activity_editor_toast1, Toast.LENGTH_LONG).show()
                        finish()
                    }
                    mProgressView.visibility = View.INVISIBLE
                }
            }.start()
        }
    }

    override fun onDestroy() {
        AttributeType.clearAttributeData()
        super.onDestroy()
    }

    private fun imageBase64TOFaceCodeBase64 (imageBase64: String) : String? {
        val decodedString: ByteArray = Base64.decode(imageBase64, Base64.DEFAULT)
        val selectedBitmap = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.size)

        // 1. base64 -> get 256 features
        val faceInfos = Service.shared.ULSeeFaceVerificationManager?.extractFeature(selectedBitmap!!)
        if (faceInfos == null) {
            return null
        }
        if (faceInfos!!.size <= 0) {
            return null
        }
        val features = faceInfos!![0].features
        return Facecode.shared.generateFacecode(features)
    }

    private fun addOrEdit() {
        if (preventDoubleClickBtn()) return

        if (isInputValid()) {
            if (isEditingMode)
                editPeople()
            else
                addPeople()
        } else {
            showToast(R.string.activity_editor_toast2)
        }
    }

    private fun preventDoubleClickBtn(): Boolean {
        if (SystemClock.elapsedRealtime() - lastClickTime < 1000) {
            return true
        }
        lastClickTime = SystemClock.elapsedRealtime()
        return false
    }

    // TODO refactoring
    private fun imageToFaceCode(): Boolean {
        return if (mImageBase64.isNotEmpty()) {
            /* 1. The case of adding a profile
               2. The case of updating profile, and face has been modified */
            val faceCode = imageBase64TOFaceCodeBase64(mImageBase64)
            if (faceCode == null) {
                showToast(R.string.activity_editor_toast4)
                false
            } else {
                AttributeType.faceCode = faceCode
                true
            }
        } else {
            // The case of updating profile, and no modification to face
            true
        }
    }

    private fun addPeople () {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, R.string.toast_no_connected_device, Toast.LENGTH_LONG).show()
            return
        }

        mProgressView.visibility = View.VISIBLE

        val face = AttributeType.getAttributeData()
        if (mImageBase64?.isEmpty() == false) {
            val facecode = imageBase64TOFaceCodeBase64(mImageBase64!!)
            if (facecode == null) {
                Toast.makeText(this, R.string.activity_editor_toast4, Toast.LENGTH_LONG).show()
                return
            }
            face.Data = facecode!!
        }

        PeopleServiceTCP(selectedTCPClient).create(face)
            .subscribe({ newPeople ->
                Toast.makeText(this, getString(R.string.create_successfully), Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                finish()
                mProgressView.visibility = View.INVISIBLE
            }, { error: Throwable ->
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                mProgressView.visibility = View.INVISIBLE
            })
    }

    private fun editPeople () {
//        val deviceManager = Service.shared.getManagerOfDeviceID(deviceId)
        val deviceManager = Service.shared.getFirstConnectedDeviceManager()
        if (deviceManager != null) {
            mProgressView.visibility = View.VISIBLE
            try {
                val isFaceExisting = imageToFaceCode()
                if (isFaceExisting) {
                    deviceManager.updateProfile(AttributeType.getAttributeData())
                    showToast(R.string.update_successfully)
                    setResult(RESULT_OK)
                    finish()
                }
            } catch (e: Exception) {
                showToast(e.localizedMessage!!)
            }
            mProgressView.visibility = View.INVISIBLE

        } else {
            showToast(R.string.toast_no_connected_device)
        }
    }

    private fun isInputValid(): Boolean {
        if (!isEditingMode) {
            if (!isPhotoTakenOrSelected)
                return false
        }

        for (attribute in AttributeType.values()) {
            if (!attribute.isInputValid)
                return false
        }

        return true
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        Log.d(TAG, "[Enter] onActivityResult")

        if (requestCode == REQUEST_CODE_PICK_IMAGE) {
            if (resultCode != RESULT_OK) return;
            var uri : Uri? = data?.data
            if (uri == null) {
                Toast.makeText(this, "error get file uri", Toast.LENGTH_LONG ).show()
                return
            } else {
                val inputStream: InputStream? = getContentResolver().openInputStream(uri)
                if (inputStream == null) {
                    Toast.makeText(this, "error open input stream", Toast.LENGTH_LONG ).show()
                    return
                }

                val stringBuilder = StringBuilder()
                var readLen = 0
                val bufferLen = 1026 // 為了base64, bufferLen*4/3 必須是4的倍數
                val buffer = ByteArray(bufferLen)

                while (true) {
                    readLen = inputStream.read(buffer, 0, bufferLen)
                    if (readLen < 0) break;
                    val base64 = Base64.encodeToString(buffer, 0, readLen, Base64.NO_WRAP)// comment or Base64.URL_SAFE
                    stringBuilder.append(base64)
                }

                while (stringBuilder.length %4 != 0) {
                    stringBuilder.append("=")
                }

                mImageBase64 = stringBuilder.toString()

                Glide.with(this).load("data:image/jpg;base64,"+stringBuilder.toString()).into(mAddFaceBtn)
                isPhotoTakenOrSelected = true
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun pickImageFromAlbum () {
        val getIntent = Intent(Intent.ACTION_GET_CONTENT)
        getIntent.type = "image/*"
        val pickIntent = Intent(
            Intent.ACTION_PICK,
            MediaStore.Images.Media.EXTERNAL_CONTENT_URI
        )
        pickIntent.type = "image/*"
        val chooserIntent = Intent.createChooser(getIntent, "Select Image")
        chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, arrayOf(pickIntent))
        startActivityForResult(chooserIntent, REQUEST_CODE_PICK_IMAGE)
    }

    private fun showToast(msg: String) {
        Toast.makeText(this, msg, Toast.LENGTH_LONG).show()
    }

    private fun showToast(resId: Int) {
        Toast.makeText(this, getString(resId), Toast.LENGTH_LONG).show()
    }

    companion object {
        const val REQUEST_CODE_PICK_IMAGE = 1236
    }

}