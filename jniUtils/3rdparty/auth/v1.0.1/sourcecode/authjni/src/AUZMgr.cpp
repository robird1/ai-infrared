#include "infra-utils/uls_infra_log.h"
#include <cstring>
#include <mutex>
#include <ServerMgr.h>
#include <sstream>
#include "authorization/AUZMgr.h"
#include "LicenseMgr.h"
#include <android/log.h>
#define printError(...)  __android_log_print(ANDROID_LOG_ERROR, "AUZMgr", __VA_ARGS__);

namespace {
    std::mutex g_AUZ_ulsee_mutex;  //互斥锁，用来保证线程安全
}

namespace uls {
    AUZMgr *AUZMgr::m_instance = new AUZMgr;   //静态成员变量初始化

    void AUZMgr::init(const char* auth_group, int platform, const char* mac, const char* appId, std::string directory) {
        this->auth_group = auth_group;
        this->platform = platform;
        this->mac = mac;
        this->directory = directory;
        this->appId = appId;

        //若末尾不是'/',则在最后添加'/'
        if(directory.back() != '/' && directory.back() != '\\') {
          this->directory.append("/");
        }

    }

    int AUZMgr::getAUZResult(std::string &key, std::string keySecret, bool &ret) {
        std::lock_guard<std::mutex> lck(g_AUZ_ulsee_mutex);
        std::string mergeStr = key + keySecret;
        LOGD("mergeStr[%s]\n", mergeStr.c_str());

        auto pos = this->keyMap.find(mergeStr);
        if (pos != this->keyMap.end()) {
            //XXX:keyMap是新new的，会走到这里吗？
            //找到，则表示已经验证，则直接读取结果
            printError("Already validated on this device.\n");
            ret = pos->second;
            return 0;
        } else {
            printError("Didn't validated on this device.\n");
        }

        std::stringstream ss;
        ss<<"{\n"
                "   \"auth_group\" : \""<<auth_group<<"\",\n"
                "   \"bind_content\" : \""<<appId<<"\",\n"
                "   \"machine_id\" : \""<<mac<<"\",\n"
                "   \"platform\" : "<<platform<<"\n}";


        std::string data = ss.str();
        printError("uuid = %s",data.c_str());

        ServerMgr serverMgr;
        bool httpConnect;
        bool keyCheck;
        ULS_HTTP_HEADER header = {};
        ULS_HTTP_BODY body = {};
        std::tie(httpConnect, keyCheck) = serverMgr.connectServer(key, keySecret, data, header, body);

        bool status;
        std::string licenseFile = this->directory + "license.txt";
      //  std::cout<<"directory="<<this->directory<<"  license.txt ="<<licenseFile<<std::endl;
        if (!httpConnect) {
            //http通信失败，则走离线认证
            printError("http connect failed, use offline validation.\n");
            int ret = offlineAUZ(licenseFile);
            status = (ret >= 0);
        } else if (keyCheck) {
            //http通信成功，且网络授权成功
            printError("http connect successed.\n");
            if (body.platform == platform && body.auth_group == auth_group) {
                LOGD("check successed %d=%d,%s=%s ",body.platform, platform, body.auth_group.c_str(), auth_group.c_str());
                onlineAUZ(licenseFile, body.expiration);
                status = true;
            } else {
                printError("key is not this platform or product group" );
                status = false;
            }
        } else {
            //http 通信成功，且网络授权失败
            printError("http connect successed, but online validation failed.\n");
            status = false;
        }

        this->keyMap[mergeStr] = status;

        ret = status;
        printError("getAUZResult returns[%s].\n", ret?"true":"false");
        return 0;
    }

    int AUZMgr::getMac(char *buf) {

        memcpy(buf, mac.data(), mac.size());
        return 1;
    }

    int AUZMgr::offlineAUZ(std::string licenseFile) {

        LicenseMgr licenseMgr;
        char buf[32] = "";
        long deadtime = 0;
        bool isExist = licenseMgr.isLicenFileExist(licenseFile, buf, deadtime);

        if (!isExist) {
            //离线认证，授权文件一定要存在的。
            printError("Error, \"license.txt\" doesn't exist.\n");
            return -1;
        }

        char mac[32] = "";
        getMac(mac);
       // std::cout<<"offlineAUZ mac ="<<mac<<std::endl;
        long itime = getTimetoLong();

        int sameMac = licenseMgr.checkMachineCode(buf, mac);

        if(strlen(mac) == 0) {
            sameMac = 1;
        }
        if (sameMac > 0 && itime < deadtime) {
            //mac相同，且没有到截至时间
            printError("离线认证通过\n");
            return 0;
            //}
        } else {
            //mac验证失败
            printError("mac is different\n");
            return -2;
        }
    }

    int AUZMgr::onlineAUZ(std::string licenseFile, int expiration) {
        LicenseMgr licenseMgr;
        char mac[32] = "";
        getMac(mac);

       // std::cout<<"onlineAUZ mac"<<mac<<std::endl;
        licenseMgr.writeLicenFile(licenseFile, mac, expiration);
        return 1;
    }

}