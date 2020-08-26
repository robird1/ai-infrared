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
import android.widget.EditText
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.core.content.FileProvider
import com.bumptech.glide.Glide
import com.uls.multifacetrackerlib.bean.FaceInfo
import com.ulsee.facecode.Facecode
import com.ulsee.sdk.faceverification.ULSeeFaceVerificationMgr
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Face
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import com.ulsee.thermalapp.utils.FilePickerHelper
import java.io.ByteArrayOutputStream
import java.io.File
import java.io.InputStream
import java.lang.Exception


class EditorActivity : AppCompatActivity() {

    var REAQUEST_CODE_PICK_IMAGE = 1234
    var REAQUEST_CODE_TAKE_PHOTO = 1235

    var oldValue : Face? = null
//    var imageBase64 : String? = null
    var newFaceImageBase64: String? = null
    var facecodeBase64 : String? = null
    lateinit var imageView : ImageView
    lateinit var nameInput : EditText
    lateinit var toolbar : Toolbar
    lateinit var mProgressDialog : ProgressDialog
    val isEditing : Boolean
        get() {
            return oldValue != null && oldValue!!.Name.isNotEmpty()
        }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_people_editor)

        mProgressDialog = ProgressDialog(this)

        // 1. init
        if (!Service.shared.isULSeeFaceVerificationManagerInited) {
            mProgressDialog.show()
            Thread(Runnable {
                val initResult = Service.shared.ULSeeFaceVerificationManager?.init("==============$$$================")
                runOnUiThread {
                    if (initResult != 0) {
                        Toast.makeText(this, "failed to init sdk", Toast.LENGTH_LONG).show()
                        finish()
                    }
                    mProgressDialog.dismiss()
                }
            }).start()
        }

        toolbar = findViewById<Toolbar>(R.id.toolbar)
        setSupportActionBar(toolbar);

        if (intent.hasExtra("people")) {
            oldValue = intent.getSerializableExtra("people") as Face
        }

        imageView = findViewById<ImageView>(R.id.imageView)
        nameInput = findViewById<EditText>(R.id.editText)
        findViewById<View>(R.id.layout_image).setOnClickListener { pickImage() }
        findViewById<View>(R.id.button_edit).setOnClickListener { pickImage() }
        findViewById<View>(R.id.button_save).setOnClickListener { save() }
        findViewById<View>(R.id.button_delete).setOnClickListener { confirmDelete() }

        if (isEditing) {
            findViewById<TextView>(R.id.textView_toolbar_title).text = "Edit People"
            findViewById<View>(R.id.button_delete).visibility = View.VISIBLE
        }

        if (oldValue != null) {
            nameInput.setText(oldValue!!.NameWihtoutFileType)

            if (oldValue!!.Image.isNullOrEmpty() == false) {
                if (!isEditing) {
                    verifyImageBase64(oldValue!!.Image!!)
                } else {
                    Glide.with(this).load(Base64.decode(oldValue!!.Image, Base64.DEFAULT)).into(imageView);
                }
            } else {
                val deviceManager = Service.shared.getFirstConnectedDeviceManager()
                if (deviceManager == null) {
                    Toast.makeText(this, "Error: no device connected", Toast.LENGTH_LONG).show()
                    finish()
                    return
                }
                PeopleServiceTCP(deviceManager!!).getSingleFace(oldValue!!).subscribe{
                    oldValue!!.Image = it
                    Glide.with(this).load(Base64.decode(it, Base64.DEFAULT)).into(imageView);
                }
            }
            //Glide.with(this).load(Base64.decode(oldValue!!.AvatarURL, Base64.DEFAULT)).into(imageView);
        }
    }

    fun pickImage () {
        pickImageFromTakePhoto()
    }

    lateinit var takePhotoIntentUri: Uri
    fun pickImageFromTakePhoto () {

        val imageFileName = "take_photo" //make a better file name

        val storageDir: File =
            Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES)
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
        startActivityForResult(cameraIntent, REAQUEST_CODE_TAKE_PHOTO)
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

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        /*if (requestCode == REAQUEST_CODE_PICK_IMAGE) {
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

                imageBase64 = stringBuilder.toString()
                Glide.with(this).load("data:image/jpg;base64,"+imageBase64).into(imageView)
            }

        }
        else */if (requestCode == REAQUEST_CODE_TAKE_PHOTO) {
            if (resultCode == RESULT_OK) {
                val file = FilePickerHelper.shared().putPickedFile(this, takePhotoIntentUri);

                val bm = BitmapFactory.decodeFile(file.path)
                val bOut = ByteArrayOutputStream()
                bm.compress(Bitmap.CompressFormat.JPEG, 100, bOut)
                val imageBase64 = Base64.encodeToString(
                    bOut.toByteArray(),
                    Base64.DEFAULT
                )

                verifyImageBase64(imageBase64)
            } else {
                Toast.makeText(this, "Cancelled", Toast.LENGTH_LONG).show()
            }
//            val photo = data!!.extras!!["data"] as Bitmap?

        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun verifyImageBase64(imageBase64: String) {
        mProgressDialog.show()
        Thread(Runnable {
            try {
                facecodeBase64 = imageBase64TOFaceCodeBase64(imageBase64)
                runOnUiThread {
                    Glide.with(this).load(Base64.decode(imageBase64, Base64.DEFAULT)).into(imageView);
                }
                newFaceImageBase64 = imageBase64
            } catch(e: Exception) {
                e.printStackTrace()
                if (!isEditing && facecodeBase64 == null) {
                    runOnUiThread {
                        Toast.makeText(this, "Failed to detect face", Toast.LENGTH_LONG).show()
                        pickImageFromTakePhoto()
                    }
                }
            } finally {
                runOnUiThread {
                    mProgressDialog.dismiss()
                }
            }
        }).start()
    }

    private fun imageBase64TOFaceCodeBase64 (imageBase64: String) : String {
        val decodedString: ByteArray = Base64.decode(imageBase64, Base64.DEFAULT)
        val selectedBitmap = BitmapFactory.decodeByteArray(decodedString, 0, decodedString.size)

        // 1. base64 -> get 256 features
        val faceInfos = Service.shared.ULSeeFaceVerificationManager?.extractFeature(selectedBitmap!!)
        val features = faceInfos!![0].features
        val faceCode = Facecode.shared.generateFacecode(features)
        return faceCode
    }

    private fun save () {
        if (newFaceImageBase64 == null && oldValue == null) {
            Toast.makeText(this, getString(R.string.please_select_image), Toast.LENGTH_SHORT).show()
            return
        }
        if (nameInput.text.isEmpty()) {
            Toast.makeText(this, getString(R.string.please_enter_name), Toast.LENGTH_SHORT).show()
            return
        }

        val name = nameInput.text.toString()
        if (name.contains(".", false) || name.contains("{", false) || name.contains("}", false)) {
            Toast.makeText(this, getString(R.string.please_not_enter_special_characters), Toast.LENGTH_SHORT).show()
            return
        }

//        var uploadingImageBase64 : String? = null
//        if (isEditing) {
//            uploadingImageBase64 = imageBase64
//        } else {
//            uploadingImageBase64 = oldValue?.Image
//            if (imageBase64 != null) uploadingImageBase64 = imageBase64
//        }
        if (newFaceImageBase64 != null) {
            try {
                facecodeBase64 = imageBase64TOFaceCodeBase64(newFaceImageBase64!!)
                // todo 2. use facecode instead upload image base64 (modify api)
                // todo 3. extract 256 features every time while take picture, and make sure faceinfo extracted
            } catch(e: Exception) {
                e.printStackTrace()
            }
        }

        if (isEditing) {
//            val base64 : String? = if(imageBase64 == null)  null else imageBase64
            val people = Face()
            people.ID = oldValue!!.ID
            people.Name = name
//            people.Image = base64!!
            people.Image = facecodeBase64!!
            people.oldName = oldValue!!.Name
            editPeople(people)
        } else {
            val people = Face()
            people.ID = "99999"           // TODO
            people.Name = name
//            people.Image = oldValue!!.Image
//            if (imageBase64 != null) people.Image = imageBase64!!
            people.Image = facecodeBase64!!
            addPeople(people)
        }
    }

    private fun addPeople (face: Face) {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressDialog.show()
        PeopleServiceTCP(selectedTCPClient).create(face)
            .subscribe({ newPeople ->
                Toast.makeText(this, getString(R.string.create_successfully), Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                finish()
                mProgressDialog.dismiss()
            }, { error: Throwable ->
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                mProgressDialog.dismiss()
            })
    }

    private fun editPeople (face: Face) {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressDialog.show()
        PeopleServiceTCP(selectedTCPClient).update(face)
            .subscribe({
                Toast.makeText(this, getString(R.string.update_successfully), Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                finish()
                mProgressDialog.dismiss()
            }, { error: Throwable ->
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                mProgressDialog.dismiss()
            })
    }

    private fun confirmDelete () {
        val ctx = this

        AlertDialog.Builder(ctx)
            .setMessage(getString(R.string.confirm_remove_data))
            .setPositiveButton(getString(R.string.remove)
            ) { dialog, whichButton ->
               deletePeople(oldValue!!)
            }
            .setNegativeButton(getString(R.string.cancel)
            ) { dialog, whichButton ->
                dialog.dismiss()
            }
            .create()
            .show()
    }

    private fun deletePeople (face: Face) {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressDialog.show()
        PeopleServiceTCP(selectedTCPClient).delete(face)
            .subscribe({
                Toast.makeText(this, getString(R.string.remove_successfully), Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                finish()
                mProgressDialog.dismiss()
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                mProgressDialog.dismiss()
            })
    }
}