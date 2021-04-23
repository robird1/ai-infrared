package com.ulsee.thermalapp.data.request

class UpdateCalibration(x: Int, y: Int, w: Int, h: Int) {
    val Action = 1
    var LT = intArrayOf(x, y)
    var WH = intArrayOf(w, h)
}