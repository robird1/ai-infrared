//
// Created by uriah on 18-2-24.
//

#include "encryptUtil.h"
#include <cryptlite/sha256.h>
#include <cryptlite/hmac.h>

std::string uls_calc_encrypt_key(std::string key, std::string keySecret){

  const char *keyword = "ULSJwtSigningKeyHeiHei";
  std::string plain = key + keySecret;
  uint8_t digest[32];
  uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(plain.c_str(), plain.length(), keyword, strlen(keyword), digest);

  std::string base64 = uls::cryptlite::base64::encode_from_array(digest, 32);
  return base64;
}

std::string uls_calc_encrypt_key(std::string plain){

  const char *keyword = "ULSJwtSigningKeyHeiHei";
  uint8_t digest[32];
  uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(plain.c_str(), plain.length(), keyword, strlen(keyword), digest);

  std::string base64 = uls::cryptlite::base64::encode_from_array(digest, 32);
  return base64;
}