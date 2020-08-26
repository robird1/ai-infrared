#include "macUtil.h"
#include "Log.h"
#include <string.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <fstream>

#include <unistd.h>
#include <string>
#include <iostream>
#include <cstdlib>

#define TAG "macUtil"

static std::string get_command_result(const std::string cmd) {
    std::string mac_address;

    const char * command_result = "command_result.txt";
    char command[500] = { 0 };
    snprintf(command, sizeof(command), "%s > %s", cmd.c_str(), command_result);

    if (0 == system(command))
    {
        std::ifstream ifs(command_result, std::ios::binary);
        if (ifs.is_open())
        {
            char line[200] = { 0 };
            while (!ifs.eof())
            {
                ifs.getline(line, sizeof(line));
                if (!ifs.good())
                {
                    break;
                }

                mac_address += line;
            }
        }
    }

    ::unlink(command_result);

    return mac_address;
}

static std::string  get_mac_address_ioctl()
{
    int fd;
    int interfaceNum = 0;
    struct ifreq buf[16];
    char mac[32];          //有线网卡mac地址
    struct ifconf ifc;

    printf("get_mac_address_ioctl 0\n");
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("get_mac_address_ioctl 0.1\n");
        close(fd);
        return "";
    }

    printf("get_mac_address_ioctl 1\n");

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;

    printf("get_mac_address_ioctl 2\n");

    if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
    {
        printf("get_mac_address_ioctl 2.1\n");
        interfaceNum = ifc.ifc_len / sizeof(struct ifreq);


        printf("get_mac_address_ioctl 2.2 interfaceNum=%d\n", interfaceNum);
        while (interfaceNum > 0)
        {
            printf("get_mac_address_ioctl 2.2.1 interfaceNum=%d\n", interfaceNum);
            memset(mac, 0, sizeof(mac));
            interfaceNum --;
            //get the mac of this interface
            if (!ioctl(fd, SIOCGIFHWADDR, (char *)(&buf[interfaceNum])))
            {
                printf("get_mac_address_ioctl 2.2.1.1\n");

                if (buf[interfaceNum].ifr_hwaddr.sa_family != ARPHRD_ETHER) {
                    continue;
                }

                std::string addr = buf[interfaceNum].ifr_name;

                //查找有线网卡
                auto search_etho = addr.find("eth");
                auto search_enp2s0 = addr.find("en");

                //查找无线网卡
                //auto search_wl = addr.find("wl");

                printf("get_mac_address_ioctl 2.2.1.2\n");
                if(search_enp2s0 != addr.npos || search_etho != addr.npos) {
                    snprintf (mac, sizeof(mac), "%02X:%02X:%02X:%02X:%02X:%02X", (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[0],
                              (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[1],
                              (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[2],
                              (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[3],
                              (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[4],
                              (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[5]);
                    break;
                }


            }

        }
    }


    close(fd);
    return std::string(mac);


}

static void parse_uuid_address(const char * file_name, const char * match_words, std::string & mac_address)
{
    printf("parse_uuid_address 0 file_name=%s\n", file_name);
    mac_address.c_str();

    std::ifstream ifs(file_name, std::ios::binary);
    if (!ifs.is_open())
    {
        return;
    }

//    printf("parse_uuid_address 1\n");
    char line[4096] = { 0 };
    while (!ifs.eof())
    {
//        printf("parse_uuid_address 2\n");
        ifs.getline(line, sizeof(line));
//        printf("parse_uuid_address 3 line=%s\n", line);
        if (!ifs.good())
        {
            break;
        }
//        printf("parse_uuid_address 4\n");

        const char * mac = strstr(line, match_words);
//        printf("parse_uuid_address 5 mac=%s\n", mac);
        if (NULL == mac)
        {
            continue;
        }
        printf("parse_uuid_address 6 mac=%s\n", mac);
        mac += strlen(match_words);

        while ('\0' != mac[0] && ' ' != mac[0] && '\n' != mac[0])
        {
            mac_address.push_back(mac[0]);
            ++mac;
        }
//        printf("parse_uuid_address 7\n");

        if (!mac_address.empty())
        {
            break;
        }
    }

    ifs.close();
}

static bool get_uuid_address_by_system(std::string & mac_address)
{
    printf("get_uuid_address_by_system 0\n");
    mac_address.c_str();

    const char * lshw_result = "lshw_result.txt";
    char command[51200] = { 0 };
    snprintf(command, sizeof(command), "dmesg > %s", lshw_result);

    printf("get_uuid_address_by_system 1 command=%s\n", command);
    if (0 == system(command))
    {
        printf("get_uuid_address_by_system 1.1\n");
        parse_uuid_address(lshw_result, "root=UUID=", mac_address);
    }

    printf("get_uuid_address_by_system 2\n");
    ::unlink(lshw_result);

    return(!mac_address.empty());
}

std::string  uls_getLocalMac() {
    std::string mac_address = get_command_result("cat /sys/class/net/wlan0/address");
    if(mac_address.empty()) {
        mac_address = get_command_result("settings get secure android_id");
    }
    LOGD(TAG, "mac_address =%s", mac_address.c_str());
    return mac_address;
}

std::string uls_getOwnerExeName(void)
{
    char path[1024] = "";
    const unsigned int cnt = readlink("/proc/self/exe", path, sizeof(path));
    LOGI(TAG, "readlink gets path[%s]\n", path);

    std::string exeName;
    std::string porcName(path);

    if(cnt<=0 or sizeof(path)<=cnt) {
        LOGD(TAG, "error, readlink returns %d\n", cnt);
    } else {
        exeName = std::string(porcName.substr(porcName.find_last_of("/\\")+1));
    }

    LOGD(TAG, "uls_getOwnerExeName returns %s\n", exeName.c_str());

    return exeName;
}

