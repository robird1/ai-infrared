package com.ulsee.thermalapp.ui.people

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.bumptech.glide.Glide
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.People

class PeopleListAdapter: RecyclerView.Adapter<PeopleListAdapter.ViewHolder>() {

    var peopleList: List<People> = ArrayList()
    fun setList(list: List<People>) {
        peopleList = list
        notifyDataSetChanged()
    }
    fun getList():List<People> {
        return peopleList
    }

    override fun getItemCount(): Int = this.peopleList.size

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder?.bind(peopleList[position])
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view =
            LayoutInflater.from(parent?.context).inflate(R.layout.item_list_people, parent, false)
        return ViewHolder(view)
    }

    inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val nameTV = itemView?.findViewById<TextView>(R.id.textView_peopleName)
        private val iv = itemView?.findViewById<ImageView>(R.id.imageView)

        fun bind(people: People) {
            nameTV?.text = people.Name
            Glide.with(itemView.context).load(people.AvatarURL).into(iv);
        }
    }
}
