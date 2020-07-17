package com.ulsee.thermalapp.utils

import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Context.NOTIFICATION_SERVICE
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.media.RingtoneManager
import android.net.Uri
import android.os.Build
import android.util.Base64
import android.util.Log
import androidx.core.app.NotificationCompat
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.Notification2
import java.lang.Exception


class NotificationCenter {
    private var mInstance: NotificationCenter? = null
    companion object {
        val shared = NotificationCenter()
    }

//    fun shared(): NotificationCenter? {
//        if (mInstance == null) mInstance = NotificationCenter()
//        return mInstance
//    }

    private var id = 100

    fun show(
        ctx: Context,
        intent: Intent,
        title: String?,
        messageBody: String?,
        bigImage: Bitmap?
    ) {
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
        val pendingIntent = PendingIntent.getActivity(
            ctx, 0 /* Request code */, intent,
            PendingIntent.FLAG_ONE_SHOT
        )
        val channelId = "channel_id_singleton"
        val defaultSoundUri: Uri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION)
        val notificationBuilder: NotificationCompat.Builder = NotificationCompat.Builder(ctx, channelId)
            .setSmallIcon(R.mipmap.ic_launcher)
            .setColor(ctx.getResources().getColor(R.color.colorPrimary))
            .setContentTitle(title)
            .setContentText(messageBody)
            .setAutoCancel(true)
            .setSound(defaultSoundUri)
            .setContentIntent(pendingIntent)

        if (bigImage != null) {
            val bigPicture = NotificationCompat.BigPictureStyle()
            bigPicture.bigPicture(bigImage)
            notificationBuilder.setStyle(bigPicture)
        }

        val notificationManager =
            ctx.getSystemService(NOTIFICATION_SERVICE) as NotificationManager

        // Since android Oreo notification channel is needed.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                channelId,
                "Channel human readable title",
                NotificationManager.IMPORTANCE_DEFAULT
            )
            notificationManager.createNotificationChannel(channel)
        }
        notificationManager.notify(id /* ID of notification */, notificationBuilder.build())
        Log.d("NotificationCenter", "[After] notificationManager.notify")
        id++
    }

    fun show2(
        ctx: Context,
        intent: Intent,
        title: String?,
        notification: Notification2?
    ) {
        Log.d("NotificationCenter", "[Enter] show2")
        intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
        val pendingIntent = PendingIntent.getActivity(
            ctx, 0 /* Request code */, intent,
            PendingIntent.FLAG_ONE_SHOT
        )
        val channelId = "channel_id_singleton"
        val defaultSoundUri: Uri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION)
        val notificationBuilder: NotificationCompat.Builder = NotificationCompat.Builder(ctx, channelId)
            .setSmallIcon(R.mipmap.ic_launcher)
            .setColor(ctx.getResources().getColor(R.color.colorPrimary))
            .setContentTitle(title)
            .setContentText(getContentText(notification!!))
            .setAutoCancel(true)
            .setSound(defaultSoundUri)
            .setContentIntent(pendingIntent)
//            .setWhen(notificatiion?.createdAt?.time)

        val bitImage = getImageIcon(notification)
        if(bitImage != null) {
                val bigPicture = NotificationCompat.BigPictureStyle()
                bigPicture.bigPicture(bitImage)
                notificationBuilder.setLargeIcon(bitImage)
                notificationBuilder.setStyle(bigPicture)
        }

//        val triggeredTime = notificatiion?.createdAt?.time
//        if (triggeredTime != null) {
//            notificationBuilder.setWhen(triggeredTime)
//        }

        val notificationManager =
            ctx.getSystemService(NOTIFICATION_SERVICE) as NotificationManager

        // Since android Oreo notification channel is needed.
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                channelId,
                "Channel human readable title",
                NotificationManager.IMPORTANCE_DEFAULT
            )
            notificationManager.createNotificationChannel(channel)
        }
        notificationManager.notify(id /* ID of notification */, notificationBuilder.build())
        id++
    }

    private fun getImageIcon(notification: Notification2?) : Bitmap? {
        var bitImage : Bitmap? = null
        try {
            val decodedString: ByteArray = Base64.decode(notification?.Data, Base64.DEFAULT)
            bitImage = BitmapFactory.decodeByteArray(
                decodedString,
                0,
                decodedString.size
            )
        } catch(e: Exception) {

        }
        return bitImage
    }

    private fun getContentText(notification: Notification2) : String {
        return notification.Time + " Abnormal temperature detected. ("+ notification.displayName+ notification.TempValue + notification.tempratureUnitString+ ")"
    }

}