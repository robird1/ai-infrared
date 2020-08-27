package com.ulsee.thermalapp.ui.people

import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.os.Environment
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
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import com.ulsee.thermalapp.utils.FilePickerHelper
import java.io.ByteArrayOutputStream
import java.io.File

private val TAG = "EditorActivity2"

class EditorActivity2 : AppCompatActivity() {
    lateinit var recyclerView: RecyclerView
    lateinit var toolbar: Toolbar
    private lateinit var mAddFaceBtn: ImageView
    private lateinit var mProgressView: ConstraintLayout
    lateinit var takePhotoIntentUri: Uri
    private var isPhotoTaken: Boolean = false
    private val isEditingMode : Boolean
        get() {
            return intent.getBooleanExtra("is_edit_mode", true)
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        Log.d(TAG, "[Enter] onCreate")

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_people_editor2)
        toolbar = findViewById(R.id.toolbar)
        setSupportActionBar(toolbar);

        recyclerView = findViewById(R.id.recyclerView)
        recyclerView.adapter = PeopleEditorAdapter(this, isEditingMode)
        recyclerView.layoutManager = LinearLayoutManager(this)
        mProgressView = findViewById(R.id.progress_view)
        mAddFaceBtn = findViewById(R.id.add_image)

        mAddFaceBtn.setOnClickListener {
            pickImageFromTakePhoto()
        }

        findViewById<View>(R.id.save_btn).setOnClickListener { save() }

        if (isEditingMode) {
            findViewById<TextView>(R.id.textView_toolbar_title).text = "Edit People"
            mAddFaceBtn.setImageResource(R.drawable.face_code_image)
        }

    }

    private fun save() {
        if (isInputValid()) {
            if (isEditingMode)
                editPeople()
            else
                addPeople()
        } else {
            Toast.makeText(this, "Please check your input information", Toast.LENGTH_LONG).show()
        }
    }


    private fun addPeople () {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressView.visibility = View.VISIBLE


        PeopleServiceTCP(selectedTCPClient).create(AttributeType.getAttributeData())
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
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressView.visibility = View.VISIBLE

        PeopleServiceTCP(selectedTCPClient).update(AttributeType.getAttributeData())
            .subscribe({
                Toast.makeText(this, getString(R.string.update_successfully), Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                finish()
                mProgressView.visibility = View.INVISIBLE
            }, { error: Throwable ->
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                mProgressView.visibility = View.INVISIBLE
            })
    }

    private fun isInputValid(): Boolean {
        for (attribute in AttributeType.values()) {
            if (!attribute.isInputValid || !isPhotoTaken)
                return false
        }
        return true
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        Log.d(TAG, "[Enter] onActivityResult")

        if (requestCode == REQUEST_TAKE_PHOTO) {
            if (resultCode == RESULT_OK) {
                val file = FilePickerHelper.shared().putPickedFile(this, takePhotoIntentUri);

                val bm = BitmapFactory.decodeFile(file.path)
                val bOut = ByteArrayOutputStream()
                bm.compress(Bitmap.CompressFormat.JPEG, 100, bOut)
                val imageBase64 = Base64.encodeToString(
                    bOut.toByteArray(),
                    Base64.DEFAULT
                )

                mAddFaceBtn.setImageBitmap(bm)
                isPhotoTaken = true
            } else {
                Toast.makeText(this, "Cancelled", Toast.LENGTH_LONG).show()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    fun pickImageFromTakePhoto () {

        val imageFileName = "take_photo" //make a better file name

        val storageDir: File = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES)
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
            image);
        cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, takePhotoIntentUri);
        startActivityForResult(cameraIntent, REQUEST_TAKE_PHOTO)
    }

    companion object {
        const val REQUEST_TAKE_PHOTO = 1235
    }

}