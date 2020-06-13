package com.ulsee.thermalapp.ui.launch

import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.provider.Settings
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.AppPreference
import com.ulsee.thermalapp.data.model.Device
import com.ulsee.thermalapp.ui.device.StarterActivity
import com.ulsee.thermalapp.utils.PermissionAskPreference
import io.realm.Realm
import io.realm.kotlin.where
import java.util.*

class LaunchActivity : AppCompatActivity() {

    val NEEDED_PERMISSIONS = arrayOf<String>(
        android.Manifest.permission.READ_EXTERNAL_STORAGE,
        android.Manifest.permission.ACCESS_WIFI_STATE,
        android.Manifest.permission.INTERNET,
        android.Manifest.permission.CAMERA
    )

    val REQUEST_CODE_PERMISSION = 11111;
    val REQUEST_CODE_GO_PERMISSION_SETTINGS = 11112;

    lateinit var permissionAskPreference : PermissionAskPreference

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_launch)

        permissionAskPreference = PermissionAskPreference(getSharedPreferences("permission", Context.MODE_PRIVATE))

        // permission
        Timer().schedule(object: TimerTask() {
            override fun run() {
                validatePermissions()
            }
        }, 200)
    }

    private fun validatePermissions() {
        if (isPermissionsAllGranted()) {
            goNextPage()
        } else if (isUserRequestNeverAskPermissionAgain()) {
            Toast.makeText(
                this,
                "Miss permission, but You require never ask permission again",
                Toast.LENGTH_SHORT
            ).show()
            goPermissionSettings()
        } else {
            requestPermission()
        }
    }

    private fun goNextPage() {
        val appPreference = AppPreference(getSharedPreferences("app", Context.MODE_PRIVATE))
        val hasDevice = Realm.getDefaultInstance().where<Device>().findAll().size > 0
        if (appPreference.isOnceCreateFirstDevice() || hasDevice ) {
            startActivity(Intent(this, MainActivity::class.java))
        } else {
            startActivity(Intent(this, StarterActivity::class.java))
        }
        finish()
    }

    private fun goPermissionSettings() {
        val intent = Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS)
        val uri = Uri.fromParts("package", packageName, null)
        intent.data = uri
        startActivityForResult(
            intent,
            REQUEST_CODE_GO_PERMISSION_SETTINGS
        )
    }

    private fun getNotAllowedPermissions(): List<String> {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            return ArrayList()
        }
        val notAllowedPermisions: MutableList<String> =
            ArrayList()
        for (permission in NEEDED_PERMISSIONS) {
            if (ContextCompat.checkSelfPermission(
                    this,
                    permission
                ) != PackageManager.PERMISSION_GRANTED
            ) {
                notAllowedPermisions.add(permission)
            }
        }
        return notAllowedPermisions
    }

    private fun isPermissionsAllGranted(): Boolean {
        return if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            true
        } else getNotAllowedPermissions().size == 0
    }

    private fun isUserRequestNeverAskPermissionAgain(): Boolean {
        var isAnyonePermissionAskable = true
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            isAnyonePermissionAskable = false
            val notAllowedPermisions =
                getNotAllowedPermissions()
            for (permission in notAllowedPermisions) {
                val isPermissionEverAsked: Boolean =
                    permissionAskPreference.isPermissionEverAsked(permission)
                isAnyonePermissionAskable =
                    isAnyonePermissionAskable || !(shouldShowRequestPermissionRationale(permission!!) == false && isPermissionEverAsked)
                if (!isPermissionEverAsked) {
                    permissionAskPreference.setPermissionAsked(permission)
                }
            }
        }
        return !isAnyonePermissionAskable
    }

    private fun requestPermission(): Boolean {
        val notAllowedPermisions =
            getNotAllowedPermissions()
        ActivityCompat.requestPermissions(
            this,
            notAllowedPermisions.toTypedArray(),
            REQUEST_CODE_PERMISSION
        )
        return false
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String?>,
        grantResults: IntArray
    ) {
        when (requestCode) {
            REQUEST_CODE_PERMISSION -> {
                validatePermissions()
            }
        }
    }

}