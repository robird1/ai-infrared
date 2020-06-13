package com.ulsee.thermalapp.data.request

class SetVideo(status: VideoStatus) {

    enum class VideoStatus(val flag: Int) {
        openRGB(0),
        closeRGB(1),
        openThermal(2),
        closeThermal(3)
    }
    val Action = 4
    val Flag = status.flag
}