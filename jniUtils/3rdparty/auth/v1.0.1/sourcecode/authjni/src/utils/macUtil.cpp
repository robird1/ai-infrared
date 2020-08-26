//
// Created by uriah on 17-12-4.
//
#include "infra-utils/uls_infra_utils.h"
#include "infra-utils/uls_infra_log.h"
#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <string>
std::string  uls_getLocalMac()
{
    int fd;
    int interfaceNum = 0;
    struct ifreq buf[16];
    char mac[32];
    struct ifconf ifc;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        close(fd);
        return "";
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;
    if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
    {
        interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
        while (interfaceNum-- > 0)
        {
            //get the mac of this interface
            if (!ioctl(fd, SIOCGIFHWADDR, (char *)(&buf[interfaceNum])))
            {
                memset(mac, 0, sizeof(mac));

                if (buf[interfaceNum].ifr_hwaddr.sa_family != ARPHRD_ETHER) {
                    continue;
                }

                snprintf (mac, sizeof(mac), "%X:%X:%X:%X:%X:%X", (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[0], (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[1], (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[2], (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[3], (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[4], (unsigned char) buf[interfaceNum].ifr_hwaddr.sa_data[5]);
                if(mac[0] != '0') {
                    close(fd);
                    return mac;
                }
            }
            else
            {
                close(fd);
                return "";
            }

        }
    }


    close(fd);

    return "";
}

std::string uls_getOwnerExeName(void)
{
    char path[1024] = "";
    const int cnt = readlink("/proc/self/exe", path, sizeof(path));
	LOGI("readlink gets path[%s]\n", path);

	std::string exeName;

    if(cnt<=0 or sizeof(path)<=cnt) {
       LOGD("error, readlink returns %d\n", cnt);
    } else {
       for(int i = cnt; i > 0; i--) {
         if(path[i] == '/') {
			exeName = std::string(&path[i+1]);
			break;
	     }
	  }
	}

    LOGD("uls_getOwnerExeName returns %s\n", exeName.c_str());

    return exeName;
}
