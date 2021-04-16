package com.ulsee.thermalapp.ui.people

import android.Manifest
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.os.Environment
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
import androidx.core.content.FileProvider
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.facecode.Facecode
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.utils.FilePickerHelper
import com.ulsee.thermalapp.utils.PermissionController
import java.io.ByteArrayOutputStream
import java.io.File

private val TAG = EditorActivity::class.java.simpleName

class EditorActivity : AppCompatActivity() {
    private lateinit var recyclerView: RecyclerView
    private lateinit var toolbar: Toolbar
    private lateinit var mAddFaceBtn: ImageView
    private lateinit var mProgressView: ConstraintLayout
    private var takePhotoIntentUri: Uri? = null
    private var isPhotoTaken: Boolean = false
    private var mImageBase64: String = ""        // The value is not empty in the following two cases: 1. add a face 2. modify the existing face
    private val isEditingMode : Boolean
        get() {
            return intent.getBooleanExtra("is_edit_mode", true)
        }
    private val deviceId: String
        get() {
            return intent.getStringExtra("device_id")!!
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
//            pickImageFromTakePhoto()
            PermissionController().requestPermission(this@EditorActivity, Manifest.permission.CAMERA)
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
                        Toast.makeText(this, R.string.activity_editor_toast1, Toast.LENGTH_LONG)
                            .show()
                        finish()
                    }
                    mProgressView.visibility = View.INVISIBLE
                }
            }.start()
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String?>,
        grantResults: IntArray
    ) {
        Log.d(TAG, "[Enter] onRequestPermissionsResult")
        val lambda = {
            pickImageFromTakePhoto()
        }
        PermissionController().onRequestPermissionsResult(requestCode, permissions, grantResults, this, lambda)
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
        val isFaceExisting = imageToFaceCode()
        if (isFaceExisting) {
            startActivityForResult(Intent(this, DeviceSyncActivity::class.java), REQUEST_SYNC_PROFILE)
        }
    }

    private fun editPeople () {
        val deviceManager = Service.shared.getManagerOfDeviceID(deviceId)
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
            if (!isPhotoTaken)
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

        if (requestCode == REQUEST_TAKE_PHOTO) {
            if (resultCode == RESULT_OK) {
                val file = FilePickerHelper.shared().putPickedFile(this, takePhotoIntentUri!!)

                val bm = BitmapFactory.decodeFile(file.path)
                val bOut = ByteArrayOutputStream()
                bm.compress(Bitmap.CompressFormat.JPEG, 100, bOut)
                val imageBase64 = Base64.encodeToString(
                    bOut.toByteArray(),
                    Base64.DEFAULT
                )
                mImageBase64 = imageBase64

                mAddFaceBtn.setImageBitmap(bm)
                isPhotoTaken = true
            } else {
                Toast.makeText(this, R.string.activity_scan_cancelled, Toast.LENGTH_LONG).show()
            }
        } else if (requestCode == REQUEST_SYNC_PROFILE) {
            if (resultCode == RESULT_OK) {
                setResult(RESULT_OK)
                finish()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun pickImageFromTakePhoto () {

        val imageFileName = "take_photo" //make a better file name

//        val storageDir: File = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES)
        val storageDir: File = this.getExternalFilesDir(Environment.DIRECTORY_PICTURES)!!

        val image: File = File.createTempFile(
            imageFileName,
            ".jpg",
            storageDir
        )

        val cameraIntent = Intent(MediaStore.ACTION_IMAGE_CAPTURE)
        //takePhotoIntentUri = Uri.fromFile(image);
        takePhotoIntentUri = FileProvider.getUriForFile(
            this,
            getPackageName() + ".fileprovider",
            image)
        cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, takePhotoIntentUri)
        startActivityForResult(cameraIntent, REQUEST_TAKE_PHOTO)
    }

    fun pickImageFromAlbum () {
        val getIntent = Intent(Intent.ACTION_GET_CONTENT)
        getIntent.type = "image/*"
        val pickIntent = Intent(
            Intent.ACTION_PICK,
            MediaStore.Images.Media.EXTERNAL_CONTENT_URI
        )
        pickIntent.type = "image/*"
        val chooserIntent = Intent.createChooser(getIntent, "Select Image")
        chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, arrayOf(pickIntent))
        startActivityForResult(chooserIntent, REAQUEST_CODE_PICK_IMAGE)
    }

    private fun showToast(msg: String) {
        Toast.makeText(this, msg, Toast.LENGTH_LONG).show()
    }

    private fun showToast(resId: Int) {
        Toast.makeText(this, getString(resId), Toast.LENGTH_LONG).show()
    }

    companion object {
        const val REQUEST_TAKE_PHOTO = 1235
        const val REAQUEST_CODE_PICK_IMAGE = 1236
        const val REQUEST_SYNC_PROFILE = 1237
    }

}