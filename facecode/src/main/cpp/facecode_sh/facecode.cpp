
#include "facecode.h"
#include "facecodeencoder_.h"
#include "facecodedecoder_.h"
class fc::crypto::RSAPublicKey::A{public:CryptoPP::RSA::PublicKey data;};fc::
crypto::RSAPublicKey::RSAPublicKey(){a=new A();}fc::crypto::RSAPublicKey::~
RSAPublicKey(){delete a;}void*fc::crypto::RSAPublicKey::getPtr()const{return&a->
data;}void fc::crypto::RSAPublicKey::save_X509(const std::string&z3740df047c){fc
::za2341d21ce::zec9d2d26dc(z3740df047c,a->data);}void fc::crypto::RSAPublicKey::
load_X509(const std::string&z3740df047c){fc::za2341d21ce::z2830f89e95(
z3740df047c,&a->data);}void fc::crypto::RSAPublicKey::load_X509(const std::
vector<char>&data){fc::za2341d21ce::z2830f89e95(data,&a->data);}void fc::crypto
::RSAPublicKey::save_PEM(const std::string&z3740df047c){fc::za2341d21ce::
z61678a7373(z3740df047c,a->data);}void fc::crypto::RSAPublicKey::load_PEM(const 
std::string&z3740df047c){fc::za2341d21ce::zbb31811941(z3740df047c,&a->data);}
void fc::crypto::RSAPublicKey::save_DER(const std::string&z3740df047c){fc::
za2341d21ce::z4f0087eb11(z3740df047c,a->data);}void fc::crypto::RSAPublicKey::
load_BER(const std::string&z3740df047c){fc::za2341d21ce::zccb0e2e79f(z3740df047c
,&a->data);}void fc::crypto::RSAPublicKey::load_BER(const std::vector<char>&data
){fc::za2341d21ce::zccb0e2e79f(data,&a->data);}bool fc::crypto::RSAPublicKey::
validate(){return fc::za2341d21ce::zccd5fa9fae(a->data);}bool fc::crypto::
RSAPublicKey::compare(const fc::crypto::RSAPublicKey&z495689be55){return fc::
za2341d21ce::zb081cc2488(a->data,*(CryptoPP::RSA::PublicKey*)z495689be55.getPtr(
));}std::string fc::crypto::RSAPublicKey::to_string(){std::string s;fc::
za2341d21ce::zf2dd96ae58(a->data,&s);fc::za2341d21ce::z92f64bfcf0 zb271657760;
zb271657760=fc::za2341d21ce::zea1bd5af49(fc::za2341d21ce::za12655acbc(
",@#$5%7(02/.,/.pfdDk9}{<zxc.m.a\"s\"ñdp;=-[P."),s);return zb271657760.
zb03064e0ff+zb271657760.zd70abecf74;}void fc::crypto::RSAPublicKey::from_string(
const std::string&s){std::string zc4cd92f0c6;zc4cd92f0c6=fc::za2341d21ce::
z32d07e8312(fc::za2341d21ce::za12655acbc(
",@#$5%7(02/.,/.pfdDk9}{<zxc.m.a\"s\"ñdp;=-[P."),std::string(s.begin(),s.begin(
)+zbda7cbaa43),std::string(s.begin()+zbda7cbaa43,s.end()));fc::za2341d21ce::
z312278d7a1(zc4cd92f0c6,&a->data);}class fc::crypto::RSAPrivateKey::A{public:
CryptoPP::RSA::PrivateKey data;};fc::crypto::RSAPrivateKey::RSAPrivateKey(){a=
new A();}fc::crypto::RSAPrivateKey::~RSAPrivateKey(){delete a;}void*fc::crypto::
RSAPrivateKey::getPtr()const{return&a->data;}void fc::crypto::RSAPrivateKey::
save_PKCS8(const std::string&z3740df047c){fc::za2341d21ce::z7767b68524(
z3740df047c,a->data);}void fc::crypto::RSAPrivateKey::load_PKCS8(const std::
string&z3740df047c){fc::za2341d21ce::zba8e8150ca(z3740df047c,&a->data);}void fc
::crypto::RSAPrivateKey::load_PKCS8(const std::vector<char>&data){fc::
za2341d21ce::zba8e8150ca(data,&a->data);}void fc::crypto::RSAPrivateKey::
save_PEM(const std::string&z3740df047c,const std::string&z044dea4665){fc::
za2341d21ce::zac58a82c01(z3740df047c,a->data,z044dea4665);}void fc::crypto::
RSAPrivateKey::load_PEM(const std::string&z3740df047c,const std::string&
z044dea4665){fc::za2341d21ce::zfc21ba32c5(z3740df047c,&a->data,z044dea4665);}
void fc::crypto::RSAPrivateKey::save_DER(const std::string&z3740df047c){fc::
za2341d21ce::z57b6949d5b(z3740df047c,a->data);}void fc::crypto::RSAPrivateKey::
load_BER(const std::string&z3740df047c){fc::za2341d21ce::z4bf875fc88(z3740df047c
,&a->data);}void fc::crypto::RSAPrivateKey::load_BER(const std::vector<char>&
data){fc::za2341d21ce::z4bf875fc88(data,&a->data);}bool fc::crypto::
RSAPrivateKey::validate(){return fc::za2341d21ce::zccd5fa9fae(a->data);}bool fc
::crypto::RSAPrivateKey::compare(const fc::crypto::RSAPrivateKey&zca5d41add1){
return fc::za2341d21ce::zb081cc2488(a->data,*(CryptoPP::RSA::PrivateKey*)
zca5d41add1.getPtr());}std::string fc::crypto::RSAPrivateKey::to_string(){std::
string s;fc::za2341d21ce::z1514a6d90a(a->data,&s);fc::za2341d21ce::z92f64bfcf0 
zb271657760;zb271657760=fc::za2341d21ce::zea1bd5af49(fc::za2341d21ce::
za12655acbc(",@#$5%7(02/.,/.pfdDk9}{<zxc.m.a\"s\"ñdp;=-[P."),s);return 
zb271657760.zb03064e0ff+zb271657760.zd70abecf74;}void fc::crypto::RSAPrivateKey
::from_string(const std::string&s){std::string zc4cd92f0c6;zc4cd92f0c6=fc::
za2341d21ce::z32d07e8312(fc::za2341d21ce::za12655acbc(
",@#$5%7(02/.,/.pfdDk9}{<zxc.m.a\"s\"ñdp;=-[P."),std::string(s.begin(),s.begin(
)+zbda7cbaa43),std::string(s.begin()+zbda7cbaa43,s.end()));fc::za2341d21ce::
z0d79a0bf63(zc4cd92f0c6,&a->data);}fc::crypto::RSAKeyPair::RSAKeyPair(){fc::
za2341d21ce::RSA_generate_KeyPair_(fc::RSA_KEY_SIZE_BITS,(CryptoPP::RSA::
PublicKey*)RSA_public_key.getPtr(),(CryptoPP::RSA::PrivateKey*)RSA_private_key.
getPtr());}fc::crypto::RSAKeyPair::~RSAKeyPair(){};std::string fc::utils::
hex_to_bin_str(const std::string&hex){return fc::zb6abe21710::zfc2c31d7b6(hex);}
std::string fc::utils::bin_str_to_hex(const std::string&zc3cab3c713){return fc::
zb6abe21710::z4bfeee0ea7(zc3cab3c713);}std::string fc::utils::hash(const std::
string&s){return fc::za2341d21ce::za12655acbc(s);}std::string fc::utils::
generate_bin_qr_svg(const std::vector<char>&z4d122f5bf5,const int z4d042ae111){
return fc::zb6abe21710::generate_bin_QR_svg(z4d122f5bf5,z4d042ae111);}class fc::
FaceCodeEncoder::F{public:std::unique_ptr<FaceCodeEncoder_>FC;F(){FC=std::
unique_ptr<FaceCodeEncoder_>(new FaceCodeEncoder_());};};fc::FaceCodeEncoder::
FaceCodeEncoder():F_{std::unique_ptr<F>(new F())}{}void fc::FaceCodeEncoder::
init(const fc::Version&z9954a0ec44,const fc::Device&z7dfd43b573,const std::
vector<float>&f,const bool zee86f86371){F_->FC->init(z9954a0ec44,z7dfd43b573,f,
zee86f86371);}template<typename T>void fc::FaceCodeEncoder::push_data(T&a){F_->
FC->push_data(a);}template void fc::FaceCodeEncoder::push_data<int>(int&T);
template void fc::FaceCodeEncoder::push_data<float>(float&T);template void fc::
FaceCodeEncoder::push_data<double>(double&T);template void fc::FaceCodeEncoder::
push_data(long&T);template void fc::FaceCodeEncoder::push_data(std::string&T);
template void fc::FaceCodeEncoder::push_data(char&T);void fc::FaceCodeEncoder::
print_data(){F_->FC->print_data();}bool fc::FaceCodeEncoder::encode(std::vector<
char>*ze147f31b6c,const fc::FaceCodeEncoder::MODE&zaee1dcfb86,const char 
zb04edbed70[(0x28+9094-0x23ac)]){if(zb04edbed70==nullptr)return F_->FC->encode(
ze147f31b6c,zaee1dcfb86);else return F_->FC->encode(ze147f31b6c,zaee1dcfb86,
zb04edbed70);}bool fc::FaceCodeEncoder::set_public_RSA_encryption_key(const fc::
crypto::RSAPublicKey&z4fbab6334e){return F_->FC->set_public_RSA_encryption_key(*
(CryptoPP::RSA::PublicKey*)z4fbab6334e.getPtr());}bool fc::FaceCodeEncoder::
set_private_RSA_signature_key(const fc::crypto::RSAPrivateKey&z0d1a80852c){
return F_->FC->set_private_RSA_signature_key(*(CryptoPP::RSA::PrivateKey*)
z0d1a80852c.getPtr());}void fc::FaceCodeEncoder::to_base91(const std::vector<
char>&ze147f31b6c,std::string*zba7e3b8b7b){return F_->FC->to_base91(ze147f31b6c,
zba7e3b8b7b);}void fc::FaceCodeEncoder::to_base64(const std::vector<char>&
ze147f31b6c,std::string*zba7e3b8b7b){return F_->FC->to_base64(ze147f31b6c,
zba7e3b8b7b);}bool fc::FaceCodeEncoder::update_encryption_key(const std::vector<
char>&ze147f31b6c,const fc::crypto::RSAPrivateKey&zca5d41add1,const fc::crypto::
RSAPublicKey&zf9c74244eb,std::vector<char>*zcaa1a0b9c4,const char zb04edbed70[
(0x1c4+6995-0x1d15)]){if(zb04edbed70==nullptr)return F_->FC->
update_encryption_key(ze147f31b6c,*(CryptoPP::RSA::PrivateKey*)zca5d41add1.
getPtr(),*(CryptoPP::RSA::PublicKey*)zf9c74244eb.getPtr(),zcaa1a0b9c4);else 
return F_->FC->update_encryption_key(ze147f31b6c,*(CryptoPP::RSA::PrivateKey*)
zca5d41add1.getPtr(),*(CryptoPP::RSA::PublicKey*)zf9c74244eb.getPtr(),
zcaa1a0b9c4,zb04edbed70);}void fc::FaceCodeEncoder::generate_QR_data(const std::
vector<char>&ze147f31b6c,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,const int 
mask,std::vector<unsigned char>*z1b622d3039){F_->FC->generate_QR_data(
ze147f31b6c,z09c8d9f9b9,mask,z1b622d3039);}void fc::FaceCodeEncoder::
generate_QR_data(const std::string&zba7e3b8b7b,const fc::FaceCodeEncoder::QR_EC&
z09c8d9f9b9,const int mask,std::vector<unsigned char>*z1b622d3039){F_->FC->
generate_QR_data(zba7e3b8b7b,z09c8d9f9b9,mask,z1b622d3039);}void fc::
FaceCodeEncoder::generate_QR_svg(const std::vector<char>&ze147f31b6c,const fc::
FaceCodeEncoder::QR_EC&z09c8d9f9b9,const int mask,const int z4d042ae111,std::
string*z9f2f90f313){F_->FC->generate_QR_svg(ze147f31b6c,z09c8d9f9b9,mask,
z4d042ae111,z9f2f90f313);}void fc::FaceCodeEncoder::generate_QR_svg(const std::
string&zba7e3b8b7b,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,const int mask,
const int z4d042ae111,std::string*z9f2f90f313){F_->FC->generate_QR_svg(
zba7e3b8b7b,z09c8d9f9b9,mask,z4d042ae111,z9f2f90f313);}void fc::FaceCodeEncoder
::generate_QR_data_color(const std::vector<char>&ze147f31b6c,const fc::
FaceCodeEncoder::QR_EC&z09c8d9f9b9,std::vector<unsigned char>*z1b622d3039){F_->
FC->generate_QR_data_color(ze147f31b6c,z09c8d9f9b9,z1b622d3039);}void fc::
FaceCodeEncoder::generate_QR_data_color(const std::string&zba7e3b8b7b,const fc::
FaceCodeEncoder::QR_EC&z09c8d9f9b9,std::vector<unsigned char>*z1b622d3039){F_->
FC->generate_QR_data_color(zba7e3b8b7b,z09c8d9f9b9,z1b622d3039);}std::string fc
::FaceCodeEncoder::get_version()const{return F_->FC->get_version();}std::string 
fc::FaceCodeEncoder::get_device()const{return F_->FC->get_device();}std::string 
fc::FaceCodeEncoder::get_header()const{return F_->FC->get_version();}std::pair<
fc::Type,void*>fc::FaceCodeEncoder::get_data(const int&z27662fd564){return F_->
FC->get_data(z27662fd564);}std::vector<float>fc::FaceCodeEncoder::get_code()
const{return F_->FC->get_code();}int fc::FaceCodeEncoder::get_data_size()const{
return F_->FC->get_data_size();}fc::FaceCodeEncoder::~FaceCodeEncoder(){}class 
fc::FaceCodeDecoder::F{public:std::unique_ptr<FaceCodeDecoder_>za1379a92a5;F(){
za1379a92a5=std::unique_ptr<FaceCodeDecoder_>(new FaceCodeDecoder_());};};fc::
FaceCodeDecoder::FaceCodeDecoder():F_{std::unique_ptr<F>(new F())}{}bool fc::
FaceCodeDecoder::set_private_RSA_decryption_key(const fc::crypto::RSAPrivateKey&
z3943db4c39){return F_->za1379a92a5->set_private_RSA_decryption_key(*(CryptoPP::
RSA::PrivateKey*)z3943db4c39.getPtr());}bool fc::FaceCodeDecoder::
set_public_RSA_signature_key(const fc::crypto::RSAPublicKey&z291e63aee2){return 
F_->za1379a92a5->set_public_RSA_signature_key(*(CryptoPP::RSA::PublicKey*)
z291e63aee2.getPtr());}bool fc::FaceCodeDecoder::decode(const std::string&
z7dfd43b573){return F_->za1379a92a5->decode(z7dfd43b573);}bool fc::
FaceCodeDecoder::decode(const std::vector<char>&z7dfd43b573){return F_->
za1379a92a5->decode(z7dfd43b573);}std::string fc::FaceCodeDecoder::get_version(
const std::vector<char>&z878675891b)const{return F_->za1379a92a5->get_version(
z878675891b);}std::string fc::FaceCodeDecoder::get_version(const std::string&
z878675891b)const{return F_->za1379a92a5->get_version(z878675891b);}std::string 
fc::FaceCodeDecoder::get_device(const std::vector<char>&z878675891b)const{return
 F_->za1379a92a5->get_device(z878675891b);}std::string fc::FaceCodeDecoder::
get_device(const std::string&z878675891b)const{return F_->za1379a92a5->
get_device(z878675891b);}std::string fc::FaceCodeDecoder::get_header(const std::
vector<char>&z878675891b)const{return F_->za1379a92a5->get_header(z878675891b);}
std::string fc::FaceCodeDecoder::get_header(const std::string&z878675891b)const{
return F_->za1379a92a5->get_header(z878675891b);}std::string fc::FaceCodeDecoder
::get_type(const std::vector<char>&z878675891b)const{return F_->za1379a92a5->
get_type(z878675891b);}std::string fc::FaceCodeDecoder::get_type(const std::
string&z878675891b)const{return F_->za1379a92a5->get_type(z878675891b);}std::
vector<float>fc::FaceCodeDecoder::get_code()const{return F_->za1379a92a5->
get_code();}std::pair<fc::Type,void*>fc::FaceCodeDecoder::get_data(const int&
z27662fd564){return F_->za1379a92a5->get_data(z27662fd564);}int fc::
FaceCodeDecoder::get_data_size()const{return F_->za1379a92a5->get_data_size();}
void fc::FaceCodeDecoder::print_data(){F_->za1379a92a5->print_data();}fc::
FaceCodeDecoder::~FaceCodeDecoder(){}
