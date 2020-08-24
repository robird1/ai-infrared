package com.ulsee.thermalapp.ui.people

import android.app.Activity
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.os.Environment
import android.provider.MediaStore
import android.util.Base64
import android.util.Log
import android.view.*
import android.widget.Toast
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.content.FileProvider
import androidx.fragment.app.Fragment
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.swiperefreshlayout.widget.SwipeRefreshLayout
import com.ulsee.thermalapp.MainActivity
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Face
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import com.ulsee.thermalapp.utils.FilePickerHelper
import com.ulsee.thermalapp.utils.RecyclerViewItemClickSupport
import java.io.ByteArrayOutputStream
import java.io.File


// TODO: Rename parameter arguments, choose names that match
// the fragment initialization parameters, e.g. ARG_ITEM_NUMBER
private const val ARG_PARAM1 = "param1"
private const val ARG_PARAM2 = "param2"

private val TAG = "ListFragment"

/**
 * A simple [Fragment] subclass.
 * Use the [ListFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class ListFragment : Fragment() {
    // TODO: Rename and change types of parameters
    private var param1: String? = null
    private var param2: String? = null
    lateinit var recyclerView: RecyclerView
    lateinit var swipeRefreshLayout: SwipeRefreshLayout

    lateinit var mMenuSearchItem: MenuItem
    lateinit var mSearchView: PeopleSearchView
    lateinit var takePhotoIntentUri: Uri
    private lateinit var mProgressView: ConstraintLayout


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            param1 = it.getString(ARG_PARAM1)
            param2 = it.getString(ARG_PARAM2)
        }

    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        Log.d(TAG, "[Enter] onCreateView")

        // Inflate the layout for this fragment
        val root = inflater.inflate(R.layout.fragment_people_list, container, false)

        swipeRefreshLayout = root.findViewById<SwipeRefreshLayout>(R.id.swipeRefreshLayout)
        swipeRefreshLayout.setOnRefreshListener { loadPeopleList() }
        recyclerView = root.findViewById<RecyclerView>(R.id.recyclerView)
        recyclerView.adapter = PeopleListAdapter(this)
        recyclerView.layoutManager = LinearLayoutManager(context)
        mProgressView = root.findViewById(R.id.progress_view)

        val support: RecyclerViewItemClickSupport = RecyclerViewItemClickSupport.addTo(recyclerView)
        support.setOnItemClickListener { recyclerView, position, _ ->
            val people = (recyclerView.adapter as PeopleListAdapter).getList()[position]

            openEditor(people, true)
        }

        setHasOptionsMenu(true)

        loadPeopleList()

        root.findViewById<View>(R.id.fab).setOnClickListener {
            pickImageFromTakePhoto()
        }

        (activity as MainActivity).setTitle("People Management")

        return root
    }

    override fun onCreateOptionsMenu(menu: Menu, inflater: MenuInflater) {
        Log.d(TAG, "[Enter] onCreateOptionsMenu")
        inflater.inflate(R.menu.people_option_menu, menu)
        configSearchView(menu)
//        super.onCreateOptionsMenu(menu, inflater)
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        Log.d(TAG, "[Enter] onOptionsItemSelected")
        return super.onOptionsItemSelected(item)
    }

    override fun onPrepareOptionsMenu(menu: Menu) {
        Log.d(TAG, "[Enter] onPrepareOptionsMenu")
//        configSearchView(menu)
//        super.onPrepareOptionsMenu(menu)
    }

    private fun configSearchView(menu: Menu) {
        mMenuSearchItem = menu.findItem(R.id.my_search)
        // Get the SearchView and set the searchable configuration
        mSearchView = mMenuSearchItem.actionView as PeopleSearchView
//        mSearchView.setActivity(this)
//        val searchManager = requireActivity().getSystemService(Context.SEARCH_SERVICE) as SearchManager
        // Assumes current activity is the searchable activity
//        mSearchView.setSearchableInfo(searchManager.getSearchableInfo(requireActivity().componentName))
//        mSearchView.isIconifiedByDefault = true
        mSearchView.setOnQueryTextListener(mSearchView)
        mMenuSearchItem.setOnActionExpandListener(mSearchView)
    }

    private fun loadPeopleList () {
        Log.d("ListFragment", "[Enter] loadPeopleList")
//        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
//        if (selectedTCPClient == null) {
//            Toast.makeText(context, "no connected device", Toast.LENGTH_LONG).show()
//            return
//        }
//        PeopleServiceTCP(selectedTCPClient).getAll()
//            .subscribe({ faceList: List<Face> ->
////                (recyclerView.adapter as PeopleListAdapter).setList(faceList)
//                swipeRefreshLayout.isRefreshing = false
//            }, { error: Throwable ->
//                Log.d("ListFragment", "[Error] PeopleServiceTCP(selectedTCPClient).getAll()")
//
//                Log.d(MainActivityTag, error.localizedMessage)
//                Toast.makeText(context, "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
//                swipeRefreshLayout.isRefreshing = false
//            })

        val list = ArrayList<Face>()
        val face = Face()
        face.Name = "steve"
        face.Age = 33
        face.Gender = "male"
        face.Birthdate = "1985/01/01"
        val face1 = Face()
        face1.Name = "steve1"
        face1.Age = 34
        face1.Gender = "male"
        face1.Birthdate = "1986/02/02"
        val face2 = Face()
        face2.Name = "steve2"
        face2.Age = 35
        face2.Gender = "female"
        face2.Birthdate = "1987/03/03"
        list.add(face)
        list.add(face1)
        list.add(face2)
        (recyclerView.adapter as PeopleListAdapter).setList(list)

    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        Log.d(TAG, "[Enter] onActivityResult")

        if (requestCode == REQUEST_ACTIVITY_EDITOR) {
            if (resultCode == Activity.RESULT_OK) {
                loadPeopleList()
            }
        } else if (requestCode == EditorActivity2.REQUEST_TAKE_PHOTO) {
            if (resultCode == Activity.RESULT_OK) {
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
                openEditor(face, false)
            } else {
                Toast.makeText(context, "Cancelled", Toast.LENGTH_LONG).show()
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
            requireContext(),
            context?.getPackageName() + ".fileprovider",
            image);
        cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, takePhotoIntentUri);
        startActivityForResult(cameraIntent, EditorActivity2.REQUEST_TAKE_PHOTO)
    }


    private fun openEditor (face: Face? = null, isEditMode: Boolean) {
        Log.d(TAG, "[Enter] openEditor")

        val intent = Intent(context, EditorActivity2::class.java)
        if (face != null) {
//            intent.putExtra("people", face as Serializable)
            if (!isEditMode) {
                AttributeType.clearAttributeData()
            } else {
                // TODO refactor
                AttributeType.NAME.isInputValid = true
            }
            AttributeType.setAttributeData(face)
            intent.putExtra("is_edit_mode", isEditMode)
        }
        startActivityForResult(intent, REQUEST_ACTIVITY_EDITOR)
    }

    fun deletePeople (face: Face) {
        val selectedTCPClient = Service.shared.getFirstConnectedDeviceManager()
        if (selectedTCPClient == null) {
            Toast.makeText(requireContext(), "no connected device", Toast.LENGTH_LONG).show()
            return
        }

        mProgressView.visibility = View.VISIBLE
//        mProgressDialog.show()
        PeopleServiceTCP(selectedTCPClient).delete(face)
            .subscribe({
                loadPeopleList()
                Toast.makeText(requireContext(), getString(R.string.remove_successfully), Toast.LENGTH_LONG).show()
                mProgressView.visibility = View.INVISIBLE
            }, { error: Throwable ->
                error.printStackTrace()
                Log.d(javaClass.name, error.localizedMessage)
                Toast.makeText(requireContext(), "Error ${error.localizedMessage}", Toast.LENGTH_LONG).show()
                mProgressView.visibility = View.INVISIBLE
            })
    }


    companion object {
        const val REQUEST_ACTIVITY_EDITOR = 1234
//        const val REQUEST_TAKE_PHOTO = 1235

        /**
         * Use this factory method to create a new instance of
         * this fragment using the provided parameters.
         *
         * @param param1 Parameter 1.
         * @param param2 Parameter 2.
         * @return A new instance of fragment ListFragment.
         */
        // TODO: Rename and change types and number of parameters
        @JvmStatic
        fun newInstance(param1: String, param2: String) =
            ListFragment().apply {
                arguments = Bundle().apply {
                    putString(ARG_PARAM1, param1)
                    putString(ARG_PARAM2, param2)
                }
            }
    }
}