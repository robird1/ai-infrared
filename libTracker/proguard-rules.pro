# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in /Applications/Android/Android Studio.app/sdk/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

-keepclasseswithmembernames class * {
    native <methods>;
}

-keepclassmembers,allowoptimization enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

-keep enum com.uls.multifacetrackerlib.UlsTrackerMode {
    *;
}

-keepclassmembers class com.uls.multifacetrackerlib.UlsMultiTracker {
    private boolean[] mAlive;
    private float[][] mShape;
    private float[][] mShape3D;
    private float[][] mConfidence;
    private float[][] mPose;
    private float[][] mEulerAngles;
    private float[][] mPupils;
    private float[][] mGaze;
    private float[] mPoseQuality;
    private int[] mShapePointCount;
    private long nativeTrackerPtr;
}

-keep public class com.uls.multifacetrackerlib.UlsMultiTracker {
    public *;
    public static *;
 }

-keep class com.seeta.VisionDetRet {
    private float mConfidence;
    private int mLeft;
    private int mTop;
    private int mRight;
    private int mBottom;
 }

-keep public enum com.uls.multifacetrackerlib.UlsMultiTracker$** {
    **[] $VALUES;
    public *;
}

-keepattributes **

-keep class com.uls.multifacetrackerlib.bean.** { *; }
-keep class com.uls.multifacetrackerlib.utils.** { *; }
-keep public class com.uls.multifacetrackerlib.ULSeeTrackerManager {
    public *;
    public static *;
 }
 -keep public class com.uls.multifacetrackerlib.ULSeeError {
     public *;
     public static *;
  }
 -keep public class com.uls.multifacetrackerlib.bean.FaceEmotion{
      public *;
      public static *;
 }