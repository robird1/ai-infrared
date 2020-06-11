package com.ulsee.thermalapp.ui.people

import android.content.Intent
import android.net.Uri
import android.os.Bundle
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
import com.bumptech.glide.Glide
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.People
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import com.ulsee.thermalapp.data.services.TCPClient
import java.io.InputStream

class EditorActivity : AppCompatActivity() {

    var REAQUEST_CODE_PICK_IMAGE = 1234

    var oldValue : People? = null
    var imageBase64 : String? = null
    lateinit var imageView : ImageView
    lateinit var nameInput : EditText
    lateinit var toolbar : Toolbar

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_people_editor)

        toolbar = findViewById<Toolbar>(R.id.toolbar)
        setSupportActionBar(toolbar);

        if (intent.hasExtra("people")) {
            oldValue = intent.getSerializableExtra("people") as People
        }

        imageView = findViewById<ImageView>(R.id.imageView)
        nameInput = findViewById<EditText>(R.id.editText)
        findViewById<View>(R.id.layout_image).setOnClickListener { pickImage() }
        findViewById<View>(R.id.button_edit).setOnClickListener { pickImage() }
        findViewById<View>(R.id.button_save).setOnClickListener { save() }
        findViewById<View>(R.id.button_delete).setOnClickListener { confirmDelete() }

        if (oldValue != null) {
            findViewById<TextView>(R.id.textView_toolbar_title).text = "Edit People"
            findViewById<View>(R.id.button_delete).visibility = View.VISIBLE
            nameInput.setText(oldValue!!.Name)
            // todo load face
            //Glide.with(this).load(Base64.decode(oldValue!!.AvatarURL, Base64.DEFAULT)).into(imageView);
        }
    }

    fun pickImage () {
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
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun save () {
        if (imageBase64 == null && oldValue == null) {
            Toast.makeText(this, "請選擇圖片!", Toast.LENGTH_SHORT).show()
            return
        }
        if (nameInput.text.isEmpty()) {
            Toast.makeText(this, "請輸入名稱!", Toast.LENGTH_SHORT).show()
            return
        }

        val name = nameInput.text.toString()
        if (oldValue != null) {
            val base64 : String? = if(imageBase64 == null)  null else imageBase64
            val people = People(oldValue!!.ID, name, base64, oldValue!!.Name)
            editPeople(people)
        } else {
            val people = People(0, name, imageBase64!!, null)
            addPeople(people)
        }
    }

    private fun addPeople (people: People) {
        val selectedTCPClient = getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        PeopleServiceTCP(selectedTCPClient).create(people)
            .subscribe({ newPeople ->
                Toast.makeText(this, "新增成功!!", Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                finish()
            }, { error: Throwable ->
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }

    private fun editPeople (people: People) {
        val selectedTCPClient = getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        PeopleServiceTCP(selectedTCPClient).update(people)
            .subscribe({
                Toast.makeText(this, "編輯成功!!", Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                finish()
            }, { error: Throwable ->
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }

    private fun confirmDelete () {
        val ctx = this

        AlertDialog.Builder(ctx)
            .setMessage("確定要刪除此筆資料?")
            .setPositiveButton("刪除"
            ) { dialog, whichButton ->
               deletePeople(oldValue!!)
            }
            .setNegativeButton("取消"
            ) { dialog, whichButton ->
                dialog.dismiss()
            }
            .create()
            .show()
    }

    private fun deletePeople (people: People) {
        val selectedTCPClient = getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(this, "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        PeopleServiceTCP(selectedTCPClient).delete(people)
            .subscribe({
                Toast.makeText(this, "刪除成功!!", Toast.LENGTH_LONG).show()
                setResult(RESULT_OK)
                finish()
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }

    private fun getFirstConnectedDeviceManager(): DeviceManager? {
        var result : DeviceManager? = null
        for (deviceManager in Service.shared.deviceManagerList) {
            if (deviceManager.tcpClient.isConnected() && deviceManager.status == DeviceManager.Status.connected) {
                result = deviceManager
                break
            }
        }
        return result
    }

    private fun getFirstConnectedClient(): TCPClient? {
        var selectedTCPClient : TCPClient? = null
        for (deviceManager in Service.shared.deviceManagerList) {
            if (deviceManager.tcpClient.isConnected() && deviceManager.status == DeviceManager.Status.connected) {
                selectedTCPClient = deviceManager.tcpClient
                break
            }
        }
        return selectedTCPClient
    }
}