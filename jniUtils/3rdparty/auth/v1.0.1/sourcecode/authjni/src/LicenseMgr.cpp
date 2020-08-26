#include <fstream>
#include <cstring>
#include <chrono>

#include "LicenseMgr.h"


#include <android/log.h>
#define printError(...)  __android_log_print(ANDROID_LOG_ERROR, "AUZMgr", __VA_ARGS__);

using namespace std;
namespace uls {

    //  返回当前的时间（最小单位是分钟，如果以秒为单位，则long溢出）
    long getTimetoLong() {
        std::time_t result = std::time(nullptr);
        auto localtime = std::gmtime(&result);
       // std::cout<<"localtime = "<<localtime<<std::endl;
        long sum = (localtime->tm_year) * 365 * 24 * 60
                   + (localtime->tm_yday) * 24 * 60
                   + (localtime->tm_hour) * 60
                   + localtime->tm_min;
                  // + localtime->tm_sec ;
        return sum;
    }
    bool LicenseMgr::isLicenFileExist(std::string &file, char mac[32], long &deadline) {

      //  std::cout<<"file is ="<<file<<std::endl;
        std::ifstream fin(file.c_str(), std::ios::in | std::ios::binary);
        if (!fin) {
            return false;
        }

        fin.read(mac, 16);
        fin.read((char *) &deadline, sizeof(long));
       //q std::cout << "deadline" << deadline <<"mac = "<<mac<< std::endl;

         uint8_t hash[48] = "";
        fin.read((char *) hash, 32);

        char key[] = "ULSJwtSigningKeyHeiHei";


      uint8_t digest[32];
      uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(mac, strlen(mac), key, strlen(key), digest);

        fin.close();

        //sha256数字签名只有３２位
        bool equal = false;
        for(int i=0; i< 32;i++) {
            if(digest[i] != hash[i]) {
                equal = false;
                break;
            }
            equal = true;
        }
        return equal;

    }

    int LicenseMgr::checkMachineCode(char mac1[16], char mac2[16]) {


        int sum = 0;
        for (int i = 0; i < 16; i++) {
            sum += (mac1[i] == mac2[i] ? 1 : 0);
        }
      //  std::cout << "sum" << sum << std::endl;
        return sum == 16;
    }

    int LicenseMgr::writeLicenFile(std::string file, char mac[32], int offlineCount) {

        ofstream fin(file.c_str(), ios::out | ios::binary | ios::trunc);
        if (!fin) {
            std::cout << "writeLicenFile error" << std::endl;
            printError("writeLicenFile error");
            return -1;
        }
        printError("writeLicenFile succeed");

        mac[16] = '\0';
        fin.write(mac, 16);

        long time = getTimetoLong() + offlineCount / 60;  //记录截至时间，以分钟为单位


        fin.write((char *)&time, sizeof(long));

        char key[] = "ULSJwtSigningKeyHeiHei";

        uint8_t digest[32];
        uls::cryptlite::hmac<uls::cryptlite::sha256>::calc(mac, strlen(mac), key, strlen(key), digest);


        fin.write((char *) digest, 32);
        fin.close();

        return 2;
    }
}
