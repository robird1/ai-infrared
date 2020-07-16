#ifndef AUTHORIZATION_AUZMGR_H_
#define AUTHORIZATION_AUZMGR_H_

#include <map>
namespace uls {

  class AUZMgr {

    public:
        static AUZMgr *getInstance() {
            return m_instance;
        }

        void init(const char *auth_group, int platform, const char * mac,const char * appId, std::string directory);

        int getAUZResult(std::string &key, std::string keySecret, bool &ret);

    private:
        AUZMgr() {}

        int getMac(char buf[8]);

        /**
         * 离线授权task
         * @return
         */
        int offlineAUZ(std::string licenseFile);

        int onlineAUZ(std::string licenseFile, int expiration);

    private:
        static AUZMgr *m_instance;
        std::map<std::string, bool> keyMap;  //key+keySecret作为key,bool作为验证是否成功
        std::string auth_group;
        int platform;
        std::string mac;
        std::string directory;
        std::string appId;

    };
}

#endif //UNTITLED2_AUZMGR_H
