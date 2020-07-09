
#ifndef zccc9351fb1
#define zccc9351fb1
#include "csvwriter.hpp"
#include "QRCodeGen/QrCode.hpp"
#include "macros.h"
namespace fc{class FaceCodeEncoder_{public:FaceCodeEncoder_();void init(const fc
::Version&z9954a0ec44,const fc::Device&z7dfd43b573,const std::vector<float>&f,
const bool zbdb9758852);bool encode(std::vector<char>*ze147f31b6c,const fc::
FaceCodeEncoder::MODE&zaee1dcfb86,const char zb04edbed70[(0x169+2689-0xbe8)]=
zed8c5cd7c5);bool set_public_RSA_encryption_key(const CryptoPP::RSA::PublicKey&
z4fbab6334e);bool set_private_RSA_signature_key(const CryptoPP::RSA::PrivateKey&
z0d1a80852c);template<typename T>void push_data(T&a);void print_data();void 
to_base91(const std::vector<char>&ze147f31b6c,std::string*zba7e3b8b7b);void 
to_base64(const std::vector<char>&ze147f31b6c,std::string*zba7e3b8b7b);bool 
update_encryption_key(const std::vector<char>&ze147f31b6c,const CryptoPP::RSA::
PrivateKey&zca5d41add1,const CryptoPP::RSA::PublicKey&zf9c74244eb,std::vector<
char>*zcaa1a0b9c4,const char zb04edbed70[(0x61+6270-0x18dd)]={});void 
generate_QR_data(const std::vector<char>&ze147f31b6c,const fc::FaceCodeEncoder::
QR_EC&z09c8d9f9b9,const int mask,std::vector<unsigned char>*z1b622d3039);void 
generate_QR_data(const std::string&zba7e3b8b7b,const fc::FaceCodeEncoder::QR_EC&
z09c8d9f9b9,const int mask,std::vector<unsigned char>*z1b622d3039);void 
generate_QR_svg(const std::vector<char>&ze147f31b6c,const fc::FaceCodeEncoder::
QR_EC&z09c8d9f9b9,const int mask,const int z4d042ae111,std::string*z9f2f90f313);
void generate_QR_svg(const std::string&zba7e3b8b7b,const fc::FaceCodeEncoder::
QR_EC&z09c8d9f9b9,const int mask,const int z4d042ae111,std::string*z9f2f90f313);
void generate_QR_data_color(const std::vector<char>&ze147f31b6c,const fc::
FaceCodeEncoder::QR_EC&z09c8d9f9b9,std::vector<unsigned char>*z1b622d3039);void 
generate_QR_data_color(const std::string&zba7e3b8b7b,const fc::FaceCodeEncoder::
QR_EC&z09c8d9f9b9,std::vector<unsigned char>*z1b622d3039);std::string 
get_version()const;std::string get_device()const;std::vector<float>get_code()
const;int get_data_size()const;std::pair<fc::Type,void*>get_data(const int&
z27662fd564);~FaceCodeEncoder_();private:bool encode(std::vector<char>*
ze147f31b6c,const char zb04edbed70[(0x1232+1344-0x1770)]=zed8c5cd7c5);bool 
z826d3c3166(std::vector<char>*ze147f31b6c,const char*zb04edbed70=zed8c5cd7c5);
bool z4ac0bfeea1(std::vector<char>*ze147f31b6c,const char zb04edbed70[
(0x1369+4066-0x2349)]=zed8c5cd7c5);bool z41074818ec(std::vector<char>*
ze147f31b6c,const char*zb04edbed70=zed8c5cd7c5);std::string z632644358d();std::
string z6c1faf6f40();std::string z6b300a2c67;std::vector<float>zba61b40ae7;std::
vector<fc::any>z51ffa25fd4;std::string z8292b6ea34;char z6d51c28ec3;ZSTD_CDict*
zbbb31ba957=nullptr;ZSTD_CCtx*z7262f8b448=nullptr;std::unique_ptr<za848461805>
z4802f01a37;CryptoPP::RSA::PublicKey z33cc0b3ea4;CryptoPP::RSA::PrivateKey 
z04f47ebd64;};}
#endif

