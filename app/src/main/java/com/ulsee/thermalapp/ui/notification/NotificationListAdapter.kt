package com.ulsee.thermalapp.ui.notification

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
import com.ulsee.thermalapp.data.model.Notification
import com.ulsee.thermalapp.data.services.NotificationServiceTCP
import io.reactivex.disposables.Disposable

class NotificationListAdapter: RecyclerView.Adapter<NotificationListAdapter.ViewHolder>() {

    var notificationList: List<Notification> = ArrayList()
    fun setList(list: List<Notification>) {
        notificationList = list
        notifyDataSetChanged()
    }
    fun getList():List<Notification> {
        return notificationList
    }

    override fun getItemCount(): Int = this.notificationList.size

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder?.bind(notificationList[position])
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view =
            LayoutInflater.from(parent?.context).inflate(R.layout.item_list_notification, parent, false)
        return ViewHolder(view)
    }

    inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val nameTV = itemView?.findViewById<TextView>(R.id.textView_peopleName)
        private val iv = itemView?.findViewById<ImageView>(R.id.imageView)
        private var disposable: Disposable? = null
        private var mNotification: Notification? = null

        fun bind(notification: Notification) {
            mNotification = notification
            disposable?.dispose()

            nameTV?.text = notification.Name
//            Glide.with(itemView.context).load(people.AvatarURL).into(iv);

            val deviceManager = Service.shared.getFirstConnectedDeviceManager()
            if (deviceManager == null) {
                Toast.makeText(itemView.context, "Error: no device connected", Toast.LENGTH_SHORT).show()
                return
            }

            if (notification.Image.isNullOrEmpty() == false) {
                Glide.with(itemView.context).load(Base64.decode(notification.Image, Base64.DEFAULT)).into(iv);
            } else {
                disposable = NotificationServiceTCP(deviceManager!!).getSingleNotification(notification.Name).subscribe{
                    disposable = null
                    notification.Image = it
                    Glide.with(itemView.context).load(Base64.decode(it, Base64.DEFAULT)).into(iv);
                }
            }

            //Glide.with(itemView.context).load(Base64.decode(people.AvatarURL, Base64.DEFAULT)).into(iv);
        }
    }
}