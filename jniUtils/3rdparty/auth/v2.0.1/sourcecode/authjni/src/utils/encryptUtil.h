//
// Created by uriah on 18-2-24.
//

#ifndef ULSAUZMGR_ENCRYPTUTIL_H_H
#define ULSAUZMGR_ENCRYPTUTIL_H_H

#include <string>

std::string uls_calc_encrypt_key(std::string key, std::string keySecret);

std::string uls_calc_encrypt_key(std::string plain);
#endif //ULSAUZMGR_ENCRYPTUTIL_H_H
