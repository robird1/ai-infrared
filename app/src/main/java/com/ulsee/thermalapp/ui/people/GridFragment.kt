package com.ulsee.thermalapp.ui.people

import android.app.Activity.RESULT_OK
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.provider.MediaStore
import android.util.Base64
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Toast
import androidx.core.content.FileProvider
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.MainActivityTag
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Face
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import com.ulsee.thermalapp.utils.FilePickerHelper
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport
import java.io.ByteArrayOutputStream
import java.io.File
import java.io.Serializable

class GridFragment : Fragment() {

    val REQUEST_CODE_ACTIVITY_EDITOR = 1234
    var REAQUEST_CODE_TAKE_PHOTO = 1235

    lateinit var recyclerView: RecyclerView
    lateinit var swipeRefreshLayout: SwipeRefreshLayout
    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        val root = inflater.inflate(R.layout.fragment_people_grid, container, false)

        swipeRefreshLayout = root.findViewById<SwipeRefreshLayout>(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadPeopleList() }
        recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
//        recyclerView.adapter = PeopleListAdapter(this)
        recyclerView.layoutManager = GridLayoutManager(context, 3)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, _ ->
            val people = (recyclerView.adapter as PeopleListAdapter).getList()[position]
            openEditor(people)
        }

        loadPeopleList();

        root.findViewById<View>(R.id.fab).setOnClickListener { pickImageFromTakePhoto() }

        (activity as MainActivity).setTitle("People Management")

        return root
    }

    lateinit var takePhotoIntentUri: Uri
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
            requireContext(),
            context?.getPackageName() + ".fileprovider",
            image);
        cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, takePhotoIntentUri);
        startActivityForResult(cameraIntent, REAQUEST_CODE_TAKE_PHOTO)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        if (requestCode == REQUEST_CODE_ACTIVITY_EDITOR) {
            if (resultCode == RESULT_OK) {
                loadPeopleList()
            }
        } else if (requestCode == REAQUEST_CODE_TAKE_PHOTO) {
            if (resultCode == RESULT_OK) {
                val file = FilePickerHelper.shared().putPickedFile(requireContext(), takePhotoIntentUri);

                val bm = BitmapFactory.decodeFile(file.path)
                val bOut = ByteArrayOutputStream()
                bm.compress(Bitmap.CompressFormat.JPEG, 100, bOut)
                val imageBase64 = Base64.encodeToString(
                    bOut.toByteArray(),
                    Base64.DEFAULT
                )
                val face = Face()
                face.Image = imageBase64
                openEditor(face)
            } else {
                Toast.makeText(context, "Cancelled", Toast.LENGTH_LONG).show()
            }
        }
        super.onActivityResult(requestCode, resultCode, data)
    }

    private fun openEditor (face: Face? = null) {
        val intent = Intent(context, EditorActivity::class.java)
        if (face != null) {
            intent.putExtra("people", face as Serializable)
        }
        startActivityForResult(intent, REQUEST_CODE_ACTIVITY_EDITOR)
    }

    private fun loadPeopleList () {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(context, "no connected device", Toast.LENGTH_LONG).show()
            return
        }
        PeopleServiceTCP(selectedTCPClient).getAll()
            .subscribe({ faceList: List<Face> ->
                (recyclerView.adapter as PeopleListAdapter).setList(faceList)
                swipeRefreshLayout.isRefreshing = false
            }, { error: Throwable ->
                Log.d(MainActivityTag, error.localizedMessage)
                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                swipeRefreshLayout.isRefreshing = false
            })
    }

}
