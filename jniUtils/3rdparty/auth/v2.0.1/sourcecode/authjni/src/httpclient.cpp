//
// Created by uriah on 18-2-8.
//
#ifdef USE_CURL
#include <curl/curl.h>
#endif

#include <sstream>
#include <cryptlite/sha256.h>
#include <cryptlite/hmac.h>
#include <infra-utils/uls_infra_utils.h>
#include <iostream>
#include "Log.h"
#include "httpclient.h"
#include "uls_json/json.hpp"
#include "utils/md5.h"
#define ULS_URL_ADDR "http://authority.ulsee.club/access_tokens"
//#define ULS_URL_ADDR  "http://192.168.1.143:12020/access_tokens"

#define TAG "httpClient"
namespace uls {

#ifdef USE_CURL
static size_t OnWriteData(void *buffer, size_t size, size_t nmemb,
                          void *lpVoid) {
  std::string *str = dynamic_cast<std::string *>((std::string *) lpVoid);
  if (nullptr == str || nullptr == buffer) {
    return -1;
  }

  char *pData = (char *) buffer;
  str->append(pData, size * nmemb);

  return nmemb;
}

uls_http_status defaultConnectServer(std::string key, std::string keySecret, std::string random, std::string data, std::string mac,std::string &response){

  CURL *curl = curl_easy_init();
  long respCode = 0;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, ULS_URL_ADDR);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2000L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "cache-control: no-cache");
    headers = curl_slist_append(
            headers, "application/x-www-form-urlencoded;charset=utf-8");
    headers =
            curl_slist_append(headers,
                              "Content-Type:multipart/form-data; "
                                      "boundary=----WebKitFormBoundaryA9AHtdQEgtrXurii");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    std::ostringstream ossHttpPostParams;
    ossHttpPostParams << "------WebKitFormBoundaryA9AHtdQEgtrXurii\n"
            "Content-Disposition: form-data; name=\"key\"\n"
            "\n"
                      << key
                      << "\n------WebKitFormBoundaryA9AHtdQEgtrXurii\n"
                              "Content-Disposition: form-data; name=\"key_secret\"\n"
                              "\n"
                      << keySecret
                      << "\n------WebKitFormBoundaryA9AHtdQEgtrXurii\n"
                              "Content-Disposition: form-data; name=\"random\"\n"
                              "\n"
                      << random
                      << "\n------WebKitFormBoundaryA9AHtdQEgtrXurii\n"
                              "Content-Disposition: form-data; name=\"mac\"\n"
                              "\n"
                      << mac
                      << "\n------WebKitFormBoundaryA9AHtdQEgtrXurii\n"
                              "Content-Disposition: form-data; name=\"uuid\"\n"
                              "\n"
                      << data
                      << "\n------WebKitFormBoundaryA9AHtdQEgtrXurii--";


    std::string body = ossHttpPostParams.str();


    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    CURLcode res = curl_easy_perform(curl);

    if (res == CURLE_OK) {
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respCode);
    }else{
      LOGE(TAG,"curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    //LOGE(TAG,"body = %s",response.c_str());
    curl_easy_cleanup(curl);

  }

  curl_global_cleanup();
  if (respCode == 200) {
    return ULS_HTTP_OK;  //key验证成功
  } else if (respCode == 400) {
    return ULS_HTTP_BAD_REQUEST;  //key 验证失败
  } else {
    return ULS_HTTP_OTHER_ERROR;  //网络问题或者其他原因
  }
}

#endif

#ifdef USE_CURL
ConnectServer HttpClient::func_ = defaultConnectServer;
#else
ConnectServer HttpClient::func_ = nullptr;
#endif


//上层可以自定义与服务器交互
void HttpClient::setCallback(ConnectServer func) {
  func_ = func;
}

uls_http_status HttpClient::connectServer(std::string &key, std::string &keySecret,
                                                    std::string &data, std::string &macAddress,
                                                    std::string &response) {

  key_ = key;
  key_secret_ = keySecret;

  //生成随机数
  srand(time(nullptr));
  int random = rand() % 10000;
  char crand[32] = "";
  sprintf(crand, "%d", random);
  random_ = crand;

  std::string str_md5_data  = MD5(data).toStr();

  if(func_ != nullptr) {
    //std::cout<<"uriah ="<<str_md5_data<<std::endl;
    return func_(key, keySecret, random_, str_md5_data, macAddress, response);
  }

  LOGE(TAG,"please set callback!");
  return ULS_HTTP_OTHER_ERROR;

}

bool HttpClient::getResToken(std::string &response, std::string &token, std::string & transport_key) {

  auto json = uls::json::parse(response);
  //std::cout<<json.dump();
  if(json["data"]["token"].type() == uls::json::value_t::string
     && json["data"]["transport_key"].type() == uls::json::value_t::string) {
    token = json["data"]["token"];
    transport_key = json["data"]["transport_key"];
    return true;
  }

  LOGE(TAG,"httpClient getResToken error! please check the body of http");

  return false;
}


bool HttpClient::valid(std::string &token, std::string & transport_key) {

  auto lastPos = token.find_last_of('.');

  if(lastPos == std::string::npos) {
    LOGE(TAG,"httpClient invalid message,please connect the server");
    return false;
  }
  //std::cout<<token<<std::endl;
  std::string secret = token.substr(lastPos+1);
  // std::cout<<secret<<std::endl;
  std::string head_body = token.substr(0, lastPos);
  // std::cout<<head_body<<std::endl;

  //先进行sh256,生成32位hash值，最后base64编码
  const char *key = "ULSJwtSigningKeyHeiHei";
  uint8_t digest[32];
  uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(head_body.c_str(), head_body.length(), key, strlen(key), digest);
  std::string cal_secret = uls::cryptlite::base64::encode_from_array(digest, 32);

  //校验两个字符串是否相等（‘+’，‘/’不进行比较,base64有两种形式）
  bool ss = equal(secret.cbegin(), secret.cend(), cal_secret.cbegin(),
                  [](char ele1, char ele2) {
                      if (ele2 == '+' || ele2 == '/') { return true; }
                      return ele1 == ele2;
                  });


  //校验服务器是否伪造
  //new add
  char version[10] = "";
  sprintf(version, "%d", version_);

  std::string m_version = MD5(version).toStr();

  std::string m_key = MD5(key_).toStr();

  std::string m_keySecret = MD5(key_secret_).toStr();

  std::string m_random = MD5(random_).toStr();

  std::string mergeStr = m_version + m_key + m_random + m_keySecret;

  std::string m_ret = MD5(mergeStr).toStr();

  bool equal = transport_key.compare(m_ret) == 0;

  return ss && equal;


}

int HttpClient::extractToken(std::string &response, uls::Token &token) {

  auto lastPos = response.find_last_of('.');
  auto seLastPos = response.find_last_of('.', lastPos - 1);
  if (lastPos == std::string::npos || seLastPos == std::string::npos) {
    LOGE(TAG,"httpClient extractToken wrong token");
    return ULS_API_RET_PARAM_ERR;
  }

  std::string body = response.substr(seLastPos + 1, lastPos - seLastPos - 1);

  //base64解码
  while (body.length() % 4 != 0) {
    body.append("=") ;
  }
  std::string decode_body;
  uls::cryptlite::base64::decode(body, decode_body);

  //decode_body　json格式
  auto json = uls::json::parse(decode_body);
  // std::string stfff = json.dump();
  //std::cout<<"stf =" << stfff<<std::endl;

  if(json["version"].type() != uls::json::value_t::null) {
    version_ = json["version"];
  }

  if(json["create_time"].type() != uls::json::value_t::null) {
     token.create_time_ = json["create_time"];
  }

  if(json["current_time"].type() != uls::json::value_t::null) {
    token.current_time_ = json["current_time"];
  }

  if(json["limit_days"].type() != uls::json::value_t::null) {
    token.limit_days_ = json["limit_days"];
  }

  if(json["days"].type() != uls::json::value_t::null) {
    token.days_ = json["days"];
  }

  if(json["limit_auth_group"].type() != uls::json::value_t::null) {
    token.limit_auth_group_ = json["limit_auth_group"];
  }

//  if(json["auth_group"].type() != uls::json::value_t::null) {
//    token.auth_group_ = json["auth_group"];
//  }

  if(json["auth_groups"].type() == uls::json::value_t::array) {

    uls::json tmp = json["auth_groups"];
    for(json::iterator iter = tmp.begin();iter != tmp.end(); iter++) {
      std::string str = *iter;
      token.auth_groups_.push_back(str);
    }

  }

  if(json["limit_platform"].type() != uls::json::value_t::null) {
    token.limit_platform_ = json["limit_platform"];
  }

  if(json["platform"].type() != uls::json::value_t::null) {
    token.platform_ = json["platform"];
  }

  if(json["limit_bind_content"].type() != uls::json::value_t::null) {
    token.limit_bind_content_ = json["limit_bind_content"];
  }

  if(json["bind_content"].type() != uls::json::value_t::null) {
    token.bind_content_ = json["bind_content"];
  }

  return ULS_API_RET_SUCCESS;


}

}//end namespace uls