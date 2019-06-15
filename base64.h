//
// Created by dantroll on 09/06/19.
//

#ifndef SERVER_MYINVLIB_BASE64_H
#define SERVER_MYINVLIB_BASE64_H


#include <string>

std::string base64_encode(unsigned char const *, unsigned int len);

std::string base64_decode(std::string const &s);


#endif //SERVER_MYINVLIB_BASE64_H
