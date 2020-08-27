package com.ulsee.thermalapp.ui.people

import android.app.ProgressDialog
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
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.content.FileProvider
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import com.ulsee.facecode.Facecode
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import com.ulsee.thermalapp.utils.FilePickerHelper
import java.io.ByteArrayOutputStream
import java.io.File
import java.io.InputStream
import java.lang.Exception

private val TAG = "EditorActivity2"

class EditorActivity2 : AppCompatActivity() {
    lateinit var recyclerView: RecyclerView
    lateinit var toolbar: Toolbar
    private lateinit var mProgressView: ConstraintLayout
    lateinit var takePhotoIntentUri: Uri
    private val isEditingMode : Boolean
        get() {
            return intent.getBooleanExtra("is_edit_mode", true)
        }

//    interface OnEditTextChanged {
//        fun onTextChanged(position: Int, charSeq: String?, isValid: Boolean)
//    }
//
//    private val onTextChangedListener = object : OnEditTextChanged {
//        override fun onTextChanged(position: Int, charSeq: String?, isValid: Boolean) {
//            Log.d(TAG, "[Enter] onTextChanged fieldName: "+ AttributeType.fromPosition(position).fieldName+
//                    " value: "+ charSeq+ " isValid: "+ isValid)
//        }
//    }

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

        findViewById<View>(R.id.save_btn).setOnClickListener { save() }

        if (isEditingMode) {
            findViewById<TextView>(R.id.textView_toolbar_title).text = "Edit People"
        }


        //  init FR
        if (!Service.shared.isULSeeFaceVerificationManagerInited) {
            mProgressView.visibility = View.VISIBLE
            Thread(Runnable {
                val initResult = Service.shared.ULSeeFaceVerificationManager?.init("==============$$$================")
                runOnUiThread {
                    if (initResult != 0) {
                        Toast.makeText(this, "failed to init sdk", Toast.LENGTH_LONG).show()
                        finish()
                    }
                    mProgressView.visibility = View.INVISIBLE
                }
            }).start()
        }
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
        val faceCode = Facecode.shared.generateFacecode(features)
        return faceCode
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

    private fun addPeople () {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressView.visibility = View.VISIBLE

        val face = AttributeType.getAttributeData()
        if (!face.Image.isEmpty()) {
            val facecode = imageBase64TOFaceCodeBase64(AttributeType.FACE.inputValue)
            if (facecode == null) {
                Toast.makeText(this, "no face detected", Toast.LENGTH_LONG).show()
                return
            }
            face.Image = facecode!!
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
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressView.visibility = View.VISIBLE

        val face = AttributeType.getAttributeData()
        if (!face.Image.isEmpty()) {
            val facecode = imageBase64TOFaceCodeBase64(AttributeType.FACE.inputValue)
            if (facecode == null) {
                Toast.makeText(this, "no face detected", Toast.LENGTH_LONG).show()
                return
            }
            face.Image = facecode!!
        }
        PeopleServiceTCP(selectedTCPClient).update(face)
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
            if (attribute.fieldName != AttributeType.FACE.fieldName) {
                Log.d(
                    TAG,
                    attribute.fieldName + ": " + attribute.inputValue + " isValid: " + attribute.isInputValid
                )
            }
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
                val file = FilePickerHelper.shared().putPickedFile(this, takePhotoIntentUri);

                val bm = BitmapFactory.decodeFile(file.path)
                val bOut = ByteArrayOutputStream()
                bm.compress(Bitmap.CompressFormat.JPEG, 100, bOut)
                val imageBase64 = Base64.encodeToString(
                    bOut.toByteArray(),
                    Base64.DEFAULT
                )

//                mFaceImageView.setImageBitmap(bm)
                AttributeType.FACE.inputValue = imageBase64
                recyclerView.adapter?.notifyItemChanged(AttributeType.FACE.position)
            } else {
                Toast.makeText(this, "Cancelled", Toast.LENGTH_LONG).show()
            }
//            val photo = data!!.extras!!["data"] as Bitmap?

        }
        if (requestCode == REAQUEST_CODE_PICK_IMAGE) {
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
                val bm = BitmapFactory.decodeStream(inputStream);
                val bOut = ByteArrayOutputStream()
                bm.compress(Bitmap.CompressFormat.JPEG, 100, bOut)
                val imageBase64 = Base64.encodeToString(
                    bOut.toByteArray(),
                    Base64.DEFAULT
                )

//                val stringBuilder = StringBuilder()
//                var readLen = 0
//                val bufferLen = 1026 // 為了base64, bufferLen*4/3 必須是4的倍數
//                val buffer = ByteArray(bufferLen)
//
//                while (true) {
//                    readLen = inputStream.read(buffer, 0, bufferLen)
//                    if (readLen < 0) break;
//                    val base64 = Base64.encodeToString(buffer, 0, readLen, Base64.NO_WRAP)// comment or Base64.URL_SAFE
//                    stringBuilder.append(base64)
//                }
//
//                while (stringBuilder.length %4 != 0) {
//                    stringBuilder.append("=")
//                }
//
//                val imageBase64 = stringBuilder.toString()
                AttributeType.FACE.inputValue = imageBase64
                recyclerView.adapter?.notifyItemChanged(AttributeType.FACE.position)
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
        startActivityForResult(cameraIntent, EditorActivity2.REQUEST_TAKE_PHOTO)
    }

    companion object {
        const val REQUEST_TAKE_PHOTO = 1235
        const val REAQUEST_CODE_PICK_IMAGE = 1236
    }



}