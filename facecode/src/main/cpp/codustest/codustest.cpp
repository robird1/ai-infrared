//
// Created by codus on 2020/7/8.
//

#include "codustest.h"
#include <string>
ct::CodusEncoder::CodusEncoder() {

}
void ct::CodusEncoder::to_base64(const std::vector<char> &facecode_binary, std::string *facecode_text) {
    *facecode_text = std::to_string(1);
}