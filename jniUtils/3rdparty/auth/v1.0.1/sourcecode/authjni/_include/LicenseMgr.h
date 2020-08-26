//
// Created by uriah on 17-11-17.
//

#ifndef UNTITLED2_LICENSEMGR_H
#define UNTITLED2_LICENSEMGR_H

#include <iostream>
#include "cryptlite/hmac.h"
#include <cryptlite/sha256.h>

namespace uls {
    long getTimetoLong();
    class LicenseMgr {
    public:
        /**
         * 判断授权文件是否存在
         * @param file 文件名
         * @param licenContent 授权文件内容
         * @return 是否存在,若存在，则把结果写入licenContent.
         */
        bool isLicenFileExist(std::string &file, char buf[16], long &offlineDate);

        /**
         * 检测机器码是否相同（不一定使用mac地址）
         * @param licenContent 授权文件内容
         * @param uniqueInfo   机器码
         * @return 是否相同，等信息
         */
        int checkMachineCode(char mac1[8], char mac2[8]);

        /**
         * 检测
         * @param file
         * @param licneContent
         * @return
         */
        int writeLicenFile(std::string file, char buf[8], int offlineCount);


    };
}

#endif //UNTITLED2_LICENSEMGR_H
