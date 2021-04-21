package com.ulsee.thermalapp.ui.notification

import android.app.Activity
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
import com.ulsee.thermalapp.data.response.NotificationImage
import com.ulsee.thermalapp.data.services.DeviceManager

//class NotificationListAdapter(private val args: ListFragmentArgs, private val activity: Activity): RecyclerView.Adapter<NotificationListAdapter.ViewHolder>() {
class NotificationListAdapter(private val activity: Activity): RecyclerView.Adapter<NotificationListAdapter.ViewHolder>() {

    private val deviceManager: DeviceManager? by lazy {
//        Service.shared.getManagerOfDeviceID(args.deviceID)
        Service.shared.getFirstConnectedDeviceManager()
    }

    private var notificationList: List<Notification> = ArrayList()
    fun setList(list: List<Notification>) {
        notificationList = list
        notifyDataSetChanged()
    }
    fun getList():List<Notification> {
        return notificationList
    }

    override fun getItemCount(): Int = this.notificationList.size

    override fun onBindViewHolder(holder: ViewHolder, position: Int) {
        holder.bind(notificationList[position])
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ViewHolder {
        val view =
            LayoutInflater.from(parent.context).inflate(R.layout.item_list_notification, parent, false)
        return ViewHolder(view)
    }

    inner class ViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val nameTV = itemView.findViewById<TextView>(R.id.textView_peopleName)
        private val iv = itemView.findViewById<ImageView>(R.id.imageView)

        fun bind(notification: Notification) {
            nameTV?.text = getNotificationText(notification)
            clearImageCache()

            if (deviceManager == null) {
                Toast.makeText(itemView.context, R.string.toast_no_connected_device, Toast.LENGTH_SHORT).show()
                return
            }

            try {
                if (notification.Data.isNotEmpty()) {
                    Glide.with(itemView.context).load(Base64.decode(notification.Data, Base64.DEFAULT)).into(iv)

                } else {
                    val listener = object: DeviceManager.OnGotNotificationImageListener{
                        override fun onNotificationImage(image: NotificationImage): Boolean {
                            if (image.ID == notification.ID) {
                                activity.runOnUiThread {
                                    deviceManager!!.removeOnGotNotificationImageListener(this)
                                    notification.Data = image.Data
                                    Glide.with(itemView.context).load(Base64.decode(image.Data, Base64.DEFAULT)).into(iv)
                                }
                                return true
                            }
                            return false
                        }
                    }

                    deviceManager!!.addOnGotNotificationImageListener(listener)
                    deviceManager!!.getSingleNotification(notification.ID)
                }

            } catch (e: Exception) {
                showToast(e.localizedMessage!!)
            }

        }

        private fun clearImageCache() {
            iv.setImageResource(0)
        }

        private fun getNotificationText(notification: Notification) =
            "${notification.Time} ${itemView.context.getString(R.string.notification_center_text)} ("+ notification.displayName+ notification.TempValue + notification.tempratureUnitString+ ")"

        private fun showToast(msg: String) {
            Toast.makeText(itemView.context, msg, Toast.LENGTH_LONG).show()
        }

        private fun showToast(resId: Int) {
            Toast.makeText(itemView.context, itemView.context.getString(resId), Toast.LENGTH_LONG).show()
        }

    }
}
