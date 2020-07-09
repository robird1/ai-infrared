
#ifndef z058123f461
#define z058123f461
#include "macros.h"
namespace fc{class FaceCodeDecoder_{public:FaceCodeDecoder_();bool 
set_public_RSA_signature_key(const CryptoPP::RSA::PublicKey&z291e63aee2);bool 
set_private_RSA_decryption_key(const CryptoPP::RSA::PrivateKey&z3943db4c39);bool
 decode(const std::vector<char>&z7dfd43b573);bool decode(const std::string&
z7dfd43b573);std::string get_version(const std::vector<char>&z878675891b)const;
std::string get_version(const std::string&z878675891b)const;std::string 
get_device(const std::vector<char>&z878675891b)const;std::string get_device(
const std::string&z878675891b)const;std::string get_header(const std::vector<
char>&z878675891b)const;std::string get_header(const std::string&z878675891b)
const;std::string get_type(const std::vector<char>&z878675891b)const;std::string
 get_type(const std::string&z878675891b)const;std::vector<float>get_code()const;
int get_data_size()const;std::pair<fc::Type,void*>get_data(const int&z27662fd564
);void print_data();~FaceCodeDecoder_();private:void init();bool zc50f723fd5(
const std::string&z7dfd43b573);bool zbcc9453911(const std::string&z7dfd43b573);
bool z5a36db6ce4(const std::string&z7dfd43b573);bool z8950aa19b8(const std::
string&z7dfd43b573);bool z0c056b70fa(const std::string&z7dfd43b573);bool 
zf7daf2ffca(const std::string&z7dfd43b573);bool ze01618f12f(const std::string&
z7dfd43b573);bool zc5e9e7490f(const std::string&z7dfd43b573);bool zccb5769d0d(
const std::vector<char>&z7dfd43b573);bool zeb9d7ce3df(const std::vector<char>&
z7dfd43b573);bool z4283f72bc3(const std::vector<char>&z7dfd43b573);bool 
za6fde61b0e(const std::vector<char>&z7dfd43b573);bool z68d0ffc068(const std::
string&s,const std::string&zc52f7d2a20,std::string*z7dfd43b573);std::string 
ze50048830a;std::vector<float>zba61b40ae7;std::vector<fc::any>z51ffa25fd4;std::
string z8292b6ea34;char z6d51c28ec3;CryptoPP::RSA::PublicKey zf83a932b82;
CryptoPP::RSA::PrivateKey zc06a26b9e7;ZSTD_DDict*z47946b1a48;ZSTD_DCtx*
z68914b4d84;std::unique_ptr<z2344c2fefe>zb52cd36923;};}
#endif

