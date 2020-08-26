
#include "infra-utils/uls_infra_log.h"
#include "ServerMgr.h"

#include "curl/include/curl.h"

using namespace std;


#define OP_ENCODE 0
#define OP_DECODE 1    //192.168.1.181
//#define ULS_URL_ADDR "http://118.89.139.53:7001/access_tokens"
//#define ULS_URL_ADDR "https://authority.ulsee.club/access_tokens"
#define ULS_URL_ADDR "http://authority.ulsee.club/access_tokens"

#include <android/log.h>

#define printError(...)  __android_log_print(ANDROID_LOG_ERROR, "AUZMgr", __VA_ARGS__);

namespace uls {


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

    std::pair<bool, bool>
    ServerMgr::connectServer(std::string &key, std::string &keySecret, std::string &data,
                             ULS_HTTP_HEADER &header, ULS_HTTP_BODY &body) {

        std::string strResponse;


        std::string str_base64_data = uls::cryptlite::base64::encode_from_string(data);
        /*if (b64_op((const unsigned char *) data.data(), data.size(),
                   base64_data, sizeof(base64_data) - 1, OP_ENCODE) > 0) {
          str_base64_data = base64_data;

        } else {
          return make_pair(false, false);
        }*/

        int httpRet = httpConnect(key, keySecret, str_base64_data, strResponse);
        if (httpRet < 0) {
            printError("strResponse = %s", strResponse.c_str());

            printError("httpRet < 0  ret:%d", httpRet);
            //httpRet == -1,表示key验证失败
            if (httpRet == -1) return make_pair(true, false);
            return make_pair(false, false);

        }

        auto lastPos = strResponse.find_last_of('"');
        auto seLastPos = strResponse.find_last_of('"', lastPos - 1);
        if (lastPos == std::string::npos || seLastPos == std::string::npos) {
            printError("strResponse = %s", strResponse.c_str());
            printError("http body parse error!");
            return make_pair(false, false);
        }

        //提取token
        std::string content = strResponse.substr(seLastPos + 1, lastPos - seLastPos - 1);

        char headerBuf[640] = "";
        char bodyBuf[640] = "";
        char secretBuf[640] = "";
        sscanf(content.c_str(), "%[0-9,a-z,A-Z].%[0-9,a-z,A-Z].%s", headerBuf, bodyBuf, secretBuf);

        //检验http响应是否被修改
        string headAndBody = std::string(headerBuf) + "." + std::string(bodyBuf);
        bool checkRet = checkHttpRespValid(std::move(headAndBody), std::string(secretBuf));
        if (!checkRet) {
            return make_pair(true, false);
        }

        //解密 header, body
        std::string clearHeader;
        std::string clearBody;

        if (decryptHttp(headerBuf, bodyBuf, clearHeader, clearBody) < 0) {
            return make_pair(true, false);
        }
        printError("clearHeader = %s", clearHeader.c_str());
        printError("clearBody = %s", clearBody.c_str());

        //解析头

        getRespHeader(clearHeader, header);


        getRespBody(clearBody, body);

        return make_pair(true, true);


    }

    int ServerMgr::httpConnect(std::string &key, std::string &keySecret, std::string &data,
                               std::string &strResponse) {

        // std::string strResponse;
        CURL * curl = curl_easy_init();
        long respCode = 0;
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, ULS_URL_ADDR);

            curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 2000L);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &strResponse);

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
                                      "Content-Disposition: form-data; name=\"uuid\"\n"
                                      "\n"
                              << data
                              << "\n------WebKitFormBoundaryA9AHtdQEgtrXurii--";


            std::string body = ossHttpPostParams.str();
            LOGD("http request body = %s\n", body.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
            CURLcode res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respCode);
            } else {
                LOGD("curl_easy_perform() failed: %s\n",
                     curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);

        }

        curl_global_cleanup();
        LOGD("respCode %ld", respCode);
        if (respCode == 200) {

            return 0;  //key验证成功
        } else if (respCode == 400) {
            return -1;  //key 验证失败
        } else {
            printError("respCode %ld", respCode);
            return -2;  //网络问题或者其他原因
        }

    }

    int ServerMgr::checkHttpRespValid(std::string &&headAndBody, std::string secret) {
        const char *key = "ULSJwtSigningKeyHeiHei";

        unsigned char mac[1024] = "";
        unsigned int mac_length = 0;
        uint8_t digest[32];
        uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(headAndBody.c_str(),
                                                           headAndBody.length(), key, strlen(key),
                                                           digest);

        std::string result = uls::cryptlite::base64::encode_from_array(digest, 32);

        //校验两个字符串是否相等（‘+’，‘/’不进行比较）
        bool ss = equal(secret.cbegin(), secret.cend(), result.cbegin(),
                        [](char ele1, char ele2) {
                            if (ele2 == '+' || ele2 == '/') { return true; }
                            return ele1 == ele2;
                        });

        return ss;
    }

//提取json中的字段
    std::string getSubStr(std::string &fullStr, std::string &&sub) {
        auto user_id = fullStr.find(sub);
        auto last_id = fullStr.find(",", user_id);
        std::string subStr = fullStr.substr(user_id, last_id - user_id);

        return subStr;
    }

    int ServerMgr::getRespHeader(std::string header, ULS_HTTP_HEADER &http_header) {


        //去掉字符串可能存在的空格，换行符合
        int temp = 0;
        char buf[1024] = "\0";
        for (auto iter = header.begin(); iter != header.end(); ++iter) {
            if (*iter != ' ' && *iter != '\n') {
                buf[temp++] = *iter;
            }
        }
        std::string noEmptyStr(buf);


        std::string typStr = getSubStr(noEmptyStr, "\"typ\":");
        char typ[512] = "\0";
        sscanf(typStr.c_str(), "\"typ\":\"%[^\"]", typ);

        std::string algStr = getSubStr(noEmptyStr, "\"alg\":");
        char alg[512] = "\0";
        sscanf(algStr.c_str(), "\"alg\":\"%[^\"]", alg);

        std::string versionStr = getSubStr(noEmptyStr, "\"version\":");
        char version[512] = "\0";
        sscanf(versionStr.c_str(), "\"version\":\"%[^\"]", version);

        http_header.type = typ;
        http_header.alg = alg;
        http_header.version = version;

        return 1;
    }


    int ServerMgr::getRespBody(std::string body, ULS_HTTP_BODY &http_body) {

        //去掉字符串可能存在的空格，换行符合
        int temp = 0;
        char buf[1024] = "\0";
        for (auto iter = body.begin(); iter != body.end(); ++iter) {
            if (*iter != ' ' && *iter != '\n') {
                buf[temp++] = *iter;
            }
        }
        std::string noEmptyStr(buf);

        std::string user_idStr = getSubStr(noEmptyStr, "\"user_id\":");
        int user_id = 0;
        sscanf(user_idStr.c_str(), "\"user_id\":%d", &user_id);

        std::string expirationStr = getSubStr(noEmptyStr, "\"expiration\":");
        int expiration = 0;
        sscanf(expirationStr.c_str(), "\"expiration\":%d", &expiration);

        std::string auth_groupStr = getSubStr(noEmptyStr, "\"auth_group\":");
        char auth_group[512] = "\0";
        sscanf(auth_groupStr.c_str(), "\"auth_group\":\"%[^\"]", auth_group);

        std::string platformStr = getSubStr(noEmptyStr, "\"platform\":");
        int platform = 0;
        sscanf(platformStr.c_str(), "\"platform\":%d", &platform);

        std::string bind_contentStr = getSubStr(noEmptyStr, "\"bind_content\":");
        char bind_content[512] = "\0";
        sscanf(bind_contentStr.c_str(), "\"bind_content\":\"%[^\"]", bind_content);

        std::string countStr = getSubStr(noEmptyStr, "\"count\":");
        int count = 0;
        sscanf(countStr.c_str(), "\"count\":%d", &count);


        http_body.user_id = user_id;
        http_body.expiration = expiration;
        http_body.count = count;
        http_body.auth_group = auth_group;
        http_body.platform = platform;
        http_body.bind_content = bind_content;

        return 1;
    }

    int ServerMgr::decryptHttp(char *header, char *body, std::string &clearHeader,
                               std::string &clearBody) {
        while (strlen(header) % 4 != 0) {
            header[strlen(header)] = '=';
        }
        while (strlen(body) % 4 != 0) {
            body[strlen(body)] = '=';
        }


        uls::cryptlite::base64::decode(std::string(header), clearHeader);
        uls::cryptlite::base64::decode(std::string(body), clearBody);

        return 0;
    }
}
