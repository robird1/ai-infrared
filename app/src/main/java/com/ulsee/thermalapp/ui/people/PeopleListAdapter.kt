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
import com.ulsee.thermalapp.data.model.People
import com.ulsee.thermalapp.data.services.DeviceManager
import com.ulsee.thermalapp.data.services.PeopleServiceTCP
import io.reactivex.disposables.Disposable

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
        private var disposable: Disposable? = null
        private var mPeople: People? = null

        fun bind(people: People) {
            mPeople = people
            disposable?.dispose()

            nameTV?.text = people.Name
//            Glide.with(itemView.context).load(people.AvatarURL).into(iv);

            val deviceManager = getFirstConnectedDeviceManager()
            if (deviceManager == null) {
                Toast.makeText(itemView.context, "Error: no device connected", Toast.LENGTH_SHORT).show()
                return
            }

            if (people.Image.isNullOrEmpty() == false) {
                Glide.with(itemView.context).load(Base64.decode(people.Image, Base64.DEFAULT)).into(iv);
            } else {
                disposable = PeopleServiceTCP(getFirstConnectedDeviceManager()!!).getSingleFace(people.Name).subscribe{
                    disposable = null
                    people.Image = it
                    Glide.with(itemView.context).load(Base64.decode(it, Base64.DEFAULT)).into(iv);
                }
            }

            //Glide.with(itemView.context).load(Base64.decode(people.AvatarURL, Base64.DEFAULT)).into(iv);
        }
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
}
