package com.ulsee.thermalapp.utils

import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.PendingIntent
import android.content.Context
import android.content.Context.NOTIFICATION_SERVICE
import android.content.Intent
import android.graphics.Bitmap
import android.media.RingtoneManager
import android.net.Uri
import android.os.Build
import androidx.core.app.NotificationCompat
import com.ulsee.thermalapp.R


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
        id++
    }
}