package com.uls.multifacetrackerlib.utils;

import android.content.Context;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.text.TextUtils;

import java.io.BufferedReader;
import java.io.FileReader;
import java.net.NetworkInterface;
import java.util.Collections;
import java.util.List;

/**
 * Created by Administrator on 2017/9/29.
 */

/**
 * 获取设备相关的信息
 *
 * @author hongjunmin
 */
public class DeviceUtil {

    /**
     * 在Android 6.0系统上, 通过Context.WIFI_SERVICE获取的Mac地址是个固定的假值“02:00:00:00:00:00”
     */
    private static final String FALSE_MAC_ADDRESS_ON_ANDROID_MARSHMALLOW = "02:00:00:00:00:00";
    private static final String INVALID_MAC_ADDRESS = "00:00:00:00:00:00";

    /**
     * 获得本机MAC地址 但在6.0以上会返回固定的假值"02:00:00:00:00:00"
     */
    private static String getWifiMacOld(Context context) {
        String wifiMac = "";
        try {
            WifiManager wifi = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
            WifiInfo info = wifi.getConnectionInfo();
            wifiMac = info.getMacAddress();
        } catch (Exception ex) {
            if (ex != null) {
                ex.printStackTrace();
            }
        }
        return wifiMac;
    }

    /**
     * 获取mac地址
     * 兼容没有WiFi功能的设备以及android6.0
     *
     * @return
     */
    private static String getWifiMac(Context context) {
        try {
            String addr = getWifiMacOld(context);
            if (!TextUtils.isEmpty(addr) && !addr.equals(INVALID_MAC_ADDRESS) && !addr.equals(FALSE_MAC_ADDRESS_ON_ANDROID_MARSHMALLOW)) {
                return addr;
            }
            List<NetworkInterface> all = Collections.list(NetworkInterface.getNetworkInterfaces());
            for (NetworkInterface nif : all) {
                if (!nif.getName().equalsIgnoreCase("wlan0")) continue;
                byte[] macBytes = nif.getHardwareAddress();
                if (macBytes == null) {
                    return "";
                }
                StringBuilder res1 = new StringBuilder();
                for (byte b : macBytes) {
                    res1.append(Integer.toHexString(b & 0xFF) + ":");
                }
                if (res1.length() > 0) {
                    res1.deleteCharAt(res1.length() - 1);
                }
                return res1.toString();
            }
        } catch (Exception ex) {
            if (ex != null) {
                ex.printStackTrace();
            }
        }
        return "";
    }

    public static String getMacAddress(Context context) {
        String macAddress = "";
        try {
            macAddress = getWifiMac(context);
            if (TextUtils.isEmpty(macAddress)) {
                macAddress = getSTBMacAddress(context);
            }
        } catch (Exception e) {
            if (e != null) {
                e.printStackTrace();
            }
        }
        return macAddress;
    }

    /**
     * 适用于没有wifi, 但是有有线网卡的设备
     * @return
     */
    private static String getSTBMacAddress(Context context) {
        try {
            return loadFileAsString("/sys/class/net/eth0/address").toUpperCase().substring(0, 17);
        } catch (Exception ex) {
            if (ex != null) {
                ex.printStackTrace();
            }
        }
        return "";
    }

    public static String loadFileAsString(String filePath) throws Exception {
        StringBuffer fileData = new StringBuffer(1000);
        BufferedReader reader = new BufferedReader(new FileReader(filePath));
        char[] buf = new char[1024];
        int numRead = 0;
        while ((numRead = reader.read(buf)) != -1) {
            String readData = String.valueOf(buf, 0, numRead);
            fileData.append(readData);
        }
        reader.close();
        return fileData.toString();
    }


}
