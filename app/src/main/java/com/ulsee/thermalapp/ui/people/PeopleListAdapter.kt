package com.ulsee.thermalapp.ui.people

import android.util.Base64
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Face
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import io.reactivex.disposables.Disposable

class PeopleListAdapter: RecyclerView.Adapter<PeopleListAdapter.ViewHolder>() {

    var faceList: List<Face> = ArrayList()
    fun setList(list: List<Face>) {
        faceList = list
        notifyDataSetChanged()
    }
    fun getList():List<Face> {
        return faceList
    }

    override fun getItemCount(): Int = this.faceList.size

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder?.bind(faceList[position])
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view =
            LayoutInflater.from(parent?.context).inflate(R.layout.item_list_people, parent, false)
        return ViewHolder(view)
    }

    inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val nameTV = itemView?.findViewById<TextView>(R.id.textView_peopleName)
        private val iv = itemView?.findViewById<ImageView>(R.id.imageView)
        private var disposable: Disposable? = null
        private var mFace: Face? = null

        fun bind(face: Face) {
            mFace = face
            disposable?.dispose()

            nameTV?.text = face.Name
//            Glide.with(itemView.context).load(people.AvatarURL).into(iv);

            val deviceManager = Service.shared.getFirstConnectedDeviceManager()
            if (deviceManager == null) {
                Toast.makeText(itemView.context, "Error: no device connected", Toast.LENGTH_SHORT).show()
                return
            }

            iv.setImageResource(0)
            if (face.Image.isNullOrEmpty() == false) {
                Glide.with(itemView.context).load(Base64.decode(face.Image, Base64.DEFAULT)).into(iv);
            } else {
                disposable = PeopleServiceTCP(deviceManager!!).getSingleFace(face).subscribe{
                    disposable = null
                    face.Image = it
                    Glide.with(itemView.context).load(Base64.decode(it, Base64.DEFAULT)).into(iv);
                }
            }

            //Glide.with(itemView.context).load(Base64.decode(people.AvatarURL, Base64.DEFAULT)).into(iv);
        }
    }
}
