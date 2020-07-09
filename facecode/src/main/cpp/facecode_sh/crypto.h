
#ifndef z095fd644b2
#define z095fd644b2
#include <string>
#include "cryptopp/v8.2/include/sha.h"
#include "cryptopp/v8.2/include/sha3.h"
#include "cryptopp/v8.2/include/rsa.h"
#include "cryptopp/v8.2/include/aes.h"
#include "cryptopp/v8.2/include/filters.h"
#include "cryptopp/v8.2/include/modes.h"
#include "cryptopp/v8.2/include/hex.h"
#include "cryptopp/v8.2/include/osrng.h"
#include "cryptopp/v8.2/include/pssr.h"
#include "cryptopp/v8.2/include/whrlpool.h"
#include "cryptopp/v8.2/include/files.h"
#include "cryptopp/v8.2/include/ida.h"
#include <assert.h>
#include <iostream>
#include "pem/pem.h"
namespace fc{namespace za2341d21ce{struct z92f64bfcf0{std::string zb03064e0ff;
std::string zd70abecf74;};std::string zbd0a8b580a(const std::string&zca6df2d267,
const std::string&z4c85584d74);void RSA_generate_KeyPair_(unsigned int 
z5cb8b6d0f7,CryptoPP::RSA::PublicKey*z495689be55,CryptoPP::RSA::PrivateKey*
zca5d41add1);std::string za12655acbc(const std::string&str);std::string 
z4b285ff686(const CryptoPP::RSA::PublicKey&z00e2c09096,const std::string&message
);bool z665dc26c9f(const CryptoPP::RSA::PrivateKey&z1cbdf6bbda,const std::string
&zd70abecf74,std::string*message);z92f64bfcf0 zea1bd5af49(const std::string&
z4c85584d74,const std::string&message);std::string z32d07e8312(const std::string
&z4c85584d74,const std::string zb03064e0ff,const std::string&zd70abecf74);std::
string z32d07e8312(const std::string&z4c85584d74,const z92f64bfcf0&zb271657760);
std::string z8c6b4183f9(const CryptoPP::RSA::PrivateKey&z9a6a46ee46,const std::
string&z8893c89d2b);bool ze3763691e2(const CryptoPP::RSA::PublicKey&z5a4b52eaef,
const std::string&z8893c89d2b,const std::string&z014d1f4106);bool zccd5fa9fae(
const CryptoPP::RSA::PublicKey&z495689be55);bool zccd5fa9fae(const CryptoPP::RSA
::PrivateKey&zca5d41add1);bool zb081cc2488(const CryptoPP::RSA::PublicKey&
z495689be55,const CryptoPP::RSA::PublicKey&ze38905fc66);bool zb081cc2488(const 
CryptoPP::RSA::PrivateKey&zca5d41add1,const CryptoPP::RSA::PrivateKey&
zeedc8f2883);void z57b6949d5b(const std::string&z3740df047c,const CryptoPP::RSA
::PrivateKey&z4c85584d74);void z4f0087eb11(const std::string&z3740df047c,const 
CryptoPP::RSA::PublicKey&z4c85584d74);void za3286fe3a5(const std::string&
z3740df047c,const CryptoPP::BufferedTransformation&zd06bda30a2);void z4bf875fc88
(const std::string&z3740df047c,CryptoPP::RSA::PrivateKey*z4c85584d74);void 
zccb0e2e79f(const std::string&z3740df047c,CryptoPP::RSA::PublicKey*z4c85584d74);
void z4bf875fc88(const std::vector<char>&data,CryptoPP::RSA::PrivateKey*
z4c85584d74);void zccb0e2e79f(const std::vector<char>&data,CryptoPP::RSA::
PublicKey*z4c85584d74);void zd13d360aff(const std::string&z3740df047c,CryptoPP::
BufferedTransformation*zd06bda30a2);void z7767b68524(const std::string&
z3740df047c,const CryptoPP::RSA::PrivateKey&z4c85584d74);void zba8e8150ca(const 
std::string&z3740df047c,CryptoPP::RSA::PrivateKey*z4c85584d74);void zba8e8150ca(
const std::vector<char>&data,CryptoPP::RSA::PrivateKey*z4c85584d74);void 
zec9d2d26dc(const std::string&z3740df047c,const CryptoPP::RSA::PublicKey&
z4c85584d74);void z2830f89e95(const std::string&z3740df047c,CryptoPP::RSA::
PublicKey*z4c85584d74);void z2830f89e95(const std::vector<char>&data,CryptoPP::
RSA::PublicKey*z4c85584d74);void zac58a82c01(const std::string&z3740df047c,const
 CryptoPP::RSA::PrivateKey&z4c85584d74,const std::string&z044dea4665={});void 
zfc21ba32c5(const std::string&z3740df047c,CryptoPP::RSA::PrivateKey*z4c85584d74,
const std::string&z044dea4665={});void z61678a7373(const std::string&z3740df047c
,const CryptoPP::RSA::PublicKey&z4c85584d74);void zbb31811941(const std::string&
z3740df047c,CryptoPP::RSA::PublicKey*z4c85584d74);void z1514a6d90a(const 
CryptoPP::RSA::PrivateKey&zca5d41add1,std::string*z59876a71bc);void z0d79a0bf63(
const std::string&z59876a71bc,CryptoPP::RSA::PrivateKey*zca5d41add1);void 
zf2dd96ae58(const CryptoPP::RSA::PublicKey&z495689be55,std::string*z54a517a822);
void z312278d7a1(const std::string&z54a517a822,CryptoPP::RSA::PublicKey*
z495689be55);bool z83cca138dc(int z7d11053ea7,int zb6b50d757b,const char*
z3740df047c,const char*seed);bool z87138f6ec7(int z7d11053ea7,const char*
zdf981b3344,char*const*ze62395cdeb);bool z90976c7263(int z7d11053ea7,int 
zb6b50d757b,const char*z3740df047c);bool z9166fd79df(int z7d11053ea7,const char*
zdf981b3344,char*const*ze62395cdeb);}}
#endif

