package com.ulsee.thermalapp.ui.people

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.RelativeLayout
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import androidx.recyclerview.widget.RecyclerView
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.Service
import com.ulsee.thermalapp.data.model.Face
import io.reactivex.disposables.Disposable


class PeopleListAdapter(private val fragment: ListFragment): RecyclerView.Adapter<PeopleListAdapter.ViewHolder>() {

    var faceList: MutableList<Face> = ArrayList()

    fun setList(list: List<Face>) {
        faceList.clear()
        faceList.addAll(list)
        notifyDataSetChanged()
    }

    fun getList(): MutableList<Face> {
        return faceList
    }

    override fun getItemCount(): Int = this.faceList.size

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.bind(faceList[position])
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view =
            LayoutInflater.from(parent.context).inflate(R.layout.item_list_people3, parent, false)
        return ViewHolder(view)
    }

    fun removeItem(position: Int) {
        faceList.removeAt(position)
        notifyItemRemoved(position)
    }

    fun restoreItem(item: Face, position: Int) {
        faceList.add(position, item)
        notifyItemInserted(position)
    }


    inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val nameTV = itemView.findViewById<TextView>(R.id.textView_peopleName)
        private val ageTV = itemView.findViewById<TextView>(R.id.textView_age)
        private val genderTV = itemView.findViewById<TextView>(R.id.textView_gender)
        private var disposable: Disposable? = null
        private var mFace: Face? = null
//        private val deleteButton = itemView.findViewById<ImageView>(R.id.delete_icon)
        val viewForeground: RelativeLayout = itemView.findViewById(R.id.view_foreground)
        val deleteIconRight: ImageView = itemView.findViewById(R.id.delete_icon_right)

        fun bind(face: Face) {
            mFace = face
            disposable?.dispose()

            nameTV?.text = face.Name
            ageTV.text = getAgeText(face)
            genderTV.text = face.Gender

//            deleteButton.setOnClickListener {
//                confirmDelete(face)
//            }

            val deviceManager = Service.shared.getFirstConnectedDeviceManager()
            if (deviceManager == null) {
                Toast.makeText(itemView.context, "Error: no device connected", Toast.LENGTH_SHORT).show()
                return
            }
        }

        private fun getAgeText(face: Face): String {
            return if (face.Age == 0) {
                ""
            } else {
                face.Age.toString()
            }
        }

//        fun confirmDelete(face: Face) {
//            val ctx = itemView.context
//
//            AlertDialog.Builder(ctx)
//                .setMessage(ctx.getString(R.string.confirm_remove_data))
//                .setPositiveButton(
//                    ctx.getString(R.string.remove)
//                ) { _, _ ->
//                    fragment.deletePeople(face, this.adapterPosition)
//                }
//                .setNegativeButton(
//                    ctx.getString(R.string.cancel)
//                ) { dialog, _ ->
//                    dialog.dismiss()
//                }
//                .create()
//                .show()
//        }
    }
}
