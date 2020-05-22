package com.ulsee.thermalapp

import android.Manifest
import android.content.Intent
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.provider.MediaStore
import android.util.Base64
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupActionBarWithNavController
import androidx.navigation.ui.setupWithNavController
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.People
import com.ulsee.thermalapp.data.model.Settings
import java.io.InputStream


val MainActivityTag = "MainActivity"

class MainActivity : AppCompatActivity() {

    var REAQUEST_CODE_PICK_IMAGE = 1234

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val navView: BottomNavigationView = findViewById(R.id.nav_view)

        val navController = findNavController(R.id.nav_host_fragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        val appBarConfiguration = AppBarConfiguration(setOf(
                R.id.navigation_home, R.id.navigation_dashboard, R.id.navigation_notifications))
        setupActionBarWithNavController(navController, appBarConfiguration)
        navView.setupWithNavController(navController)

        // permission
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(arrayOf(Manifest.permission.READ_EXTERNAL_STORAGE), 999)
        };

        updateTemperatureUnit("C")
        fetchSettings()

        getPeople()
        deletePeople(1)
        // addPeople()
        // updatePeople()

        // 1. pick image
        pickImage()
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

                val imageBase64 = stringBuilder.toString()
                Log.i(MainActivityTag, imageBase64)

                // ...
                var people = People(0, "路人甲", imageBase64)
                addPeople(people)

            }

        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    // settings
    private fun fetchSettings () {
        Service.shared.settings.get()
            .subscribe({ settings: Settings ->
                print(settings)
                true
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }

    private fun updateTemperatureUnit (value: String) {
        Service.shared.settings.updateTemperatureUnit(value)
            .subscribe({ v ->
                true
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }

    // people
    private fun getPeople () {
        Service.shared.people.getAll()
            .subscribe({ peopleList: List<People> ->
                print(peopleList)
                true
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }
    private fun deletePeople (id: Int) {
        Service.shared.people.delete(id)
            .subscribe({ _ ->
                print("delete ok")
                true
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }
    private fun addPeople (people: People) {
        Service.shared.people.create(people)
            .subscribe({ newPeople ->
                print("add ok")
                true
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(this, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
            })
    }
}
