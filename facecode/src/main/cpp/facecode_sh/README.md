# FaceCode

Dependencies are **Crypto++ 8.2** (https://www.cryptopp.com/) and **Zstd** (https://github.com/facebook/zstd).

Follow the instructions in https://github.com/noloader/cryptopp-cmake to compile **Crypto++**

**Zstd** needs to be compiled using **-fPIC** flag in order to get the static library work.

Examples on how to use the library are in the folder **/examples**:
- **facecode_single.cpp**   Simple FaceCode generation
- **facecode_multi.cpp**    Generating multiple FaceCodes
- **facecode_keys.cpp**     Facecode Keys management
- **facecode_key_smartlock_qr.cpp** Example on how to introduce the Key in the embedded device using QR Code. Keys must be unique for each device.