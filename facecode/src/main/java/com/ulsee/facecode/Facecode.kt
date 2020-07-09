package com.ulsee.facecode

class Facecode {

    external fun generateFacecode(features: FloatArray): String

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("facecode")
        }
        val shared = Facecode()
    }
}