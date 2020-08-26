Please use the following command format to build the library.

ndk-build NDK_APPLICATION_MK=<target_product_Application.mk_file> -B

For example,

ndk-build NDK_APPLICATION_MK=Application_MultiFaceTracker.mk -B

Please note that the NDK version used is 15.1.4119039.
Application.mk was deleted from the project to avoid using wrong platform id.
You should always use the above command format to build your desired library.
