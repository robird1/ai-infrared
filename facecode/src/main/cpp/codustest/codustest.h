//
// Created by codus on 2020/7/8.
//

#ifndef MY_APPLICATION_CODUSTEST_H
#define MY_APPLICATION_CODUSTEST_H

#include <vector>
namespace ct
{

    class CodusEncoder {
    public:

        CodusEncoder();
        void to_base64(const std::vector<char> &facecode_binary, std::string *facecode_text);
    };
}

#endif //MY_APPLICATION_CODUSTEST_H
