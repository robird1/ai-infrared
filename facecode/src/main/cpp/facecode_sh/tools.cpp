
#include <fstream>
#include <assert.h>
#include "tools.h"
std::vector<std::string>fc::zb6abe21710::ze153efa0be(std::istream&in,char 
z2047468a35){std::stringstream z6c73fb9766;bool z7652e13bf7=false;std::vector<
std::string>zdeaa629292;while(in){unsigned char zbdb9758852=in.get();if(!
z7652e13bf7&&zbdb9758852==((char)(0x383+8210-0x2373)))z7652e13bf7=true;else if(
z7652e13bf7&&zbdb9758852==((char)(0x1405+4793-0x269c))){if(in.peek()==
((char)(0x150c+2690-0x1f6c)))z6c73fb9766<<(char)in.get();else z7652e13bf7=false;
}else if(!z7652e13bf7&&zbdb9758852==z2047468a35){zdeaa629292.push_back(
z6c73fb9766.str());z6c73fb9766.str("");}else if(zbdb9758852!=(unsigned char)std
::ifstream::traits_type::eof())z6c73fb9766<<zbdb9758852;}zdeaa629292.push_back(
z6c73fb9766.str());return zdeaa629292;}std::vector<std::string>fc::zb6abe21710::
ze153efa0be(const std::string&line,char z2047468a35){std::stringstream 
z6c73fb9766(line);return ze153efa0be(z6c73fb9766,z2047468a35);}std::string fc::
zb6abe21710::generate_bin_QR_svg(const std::vector<char>&data,const int 
z4d042ae111){zbaf424f1e4::z3c5f83bd40::z590c984210 zbf3f6cd858;zbf3f6cd858=
zbaf424f1e4::z3c5f83bd40::z590c984210::z1ef8edc2c5;std::vector<uint8_t>
z2e2d75bb9f(data.begin(),data.end());std::vector<zbaf424f1e4::z1b5e623412>
ze0e6a80a83;ze0e6a80a83.push_back(zbaf424f1e4::z1b5e623412::zc22c182213(
(0x238d+166-0x2432)));ze0e6a80a83.push_back(zbaf424f1e4::z1b5e623412::
z40b9d4bf17(z2e2d75bb9f));zbaf424f1e4::z3c5f83bd40 z2a2c2aed28=zbaf424f1e4::
z3c5f83bd40::z7517157a9f(ze0e6a80a83,zbf3f6cd858,(0xb3b+830-0xe78),
(0x87d+2112-0x1095),(0xae6+2337-0x1407),true);return z2a2c2aed28.ze9ce39a19a(
z4d042ae111);}ZSTD_CDict*fc::zb6abe21710::z583359b2da(const std::string&
zefb80a72fe,int zdacecc7261){std::ifstream z4f35960f0d(zefb80a72fe,std::ifstream
::in|std::ifstream::binary);if(!z4f35960f0d.is_open()){throw(std::runtime_error(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x45\x72\x72\x6f\x72\x20\x55\x6e\x6b\x6e\x6f\x77\x6e\x20\x43\x6f\x6d\x70\x72\x65\x73\x73\x69\x6f\x6e"
));}std::vector<char>zde1d4b815d((std::istreambuf_iterator<char>(z4f35960f0d)),
std::istreambuf_iterator<char>());z4f35960f0d.close();ZSTD_CDict*const 
zfa6761fefb=ZSTD_createCDict(&zde1d4b815d[(0x136d+2846-0x1e8b)],zde1d4b815d.size
(),zdacecc7261);return zfa6761fefb;}ZSTD_DDict*fc::zb6abe21710::z958fd1b03e(
const std::string&zefb80a72fe){std::ifstream z4f35960f0d(zefb80a72fe,std::
ifstream::in|std::ifstream::binary);if(!z4f35960f0d.is_open()){throw(std::
runtime_error(
"\x46\x61\x63\x65\x44\x65\x43\x6f\x64\x65\x3a\x3a\x45\x72\x72\x6f\x72\x20\x55\x6e\x6b\x6e\x6f\x77\x6e\x20\x44\x65\x63\x6f\x6d\x70\x72\x65\x73\x73\x69\x6f\x6e"
));}std::vector<char>zde1d4b815d((std::istreambuf_iterator<char>(z4f35960f0d)),
std::istreambuf_iterator<char>());z4f35960f0d.close();ZSTD_DDict*const 
z8b51113b6e=ZSTD_createDDict(&zde1d4b815d[(0x509+7506-0x225b)],zde1d4b815d.size(
));return z8b51113b6e;}ZSTD_CDict*fc::zb6abe21710::z583359b2da(int zdacecc7261){
std::vector<unsigned char>zde1d4b815d(z5fd9bce4a4,z5fd9bce4a4+z0a2a9486d8);
ZSTD_CDict*const zfa6761fefb=ZSTD_createCDict(&zde1d4b815d[(0x1358+420-0x14fc)],
zde1d4b815d.size(),zdacecc7261);return zfa6761fefb;}ZSTD_DDict*fc::zb6abe21710::
z958fd1b03e(){std::vector<unsigned char>zde1d4b815d(z5fd9bce4a4,z5fd9bce4a4+
z0a2a9486d8);ZSTD_DDict*const z8b51113b6e=ZSTD_createDDict(&zde1d4b815d[
(0x433+8634-0x25ed)],zde1d4b815d.size());return z8b51113b6e;}bool fc::
zb6abe21710::zfbdd9b1d7e(const std::string&str,int zf8fa66423c,std::string*
z82c8d34a99){size_t zd6802bc739=ZSTD_compressBound(str.size());if(zd6802bc739<
(0x5c0+362-0x729))return false;*z82c8d34a99={};z82c8d34a99->resize(zd6802bc739);
size_t const z275e0712cd=ZSTD_compress((void*)z82c8d34a99->data(),zd6802bc739,
str.data(),str.size(),zf8fa66423c);if(ZSTD_isError(z275e0712cd))return false;
z82c8d34a99->resize(z275e0712cd);z82c8d34a99->shrink_to_fit();return true;}bool 
fc::zb6abe21710::zf08ccce3b6(const std::string&str,std::string*z1bbec3ce00){auto
 const zbbe0cd8734=ZSTD_getDecompressedSize(str.data(),str.size());if(
zbbe0cd8734<(0xc7c+1084-0x10b7))return false;*z1bbec3ce00={};z1bbec3ce00->resize
(zbbe0cd8734);size_t const z79e9472fb1=ZSTD_decompress((void*)z1bbec3ce00->data(
),zbbe0cd8734,str.data(),str.size());if(ZSTD_isError(z79e9472fb1))return false;
z1bbec3ce00->resize(z79e9472fb1);z1bbec3ce00->shrink_to_fit();return true;}bool 
fc::zb6abe21710::zfbdd9b1d7e(const std::string&data,ZSTD_CDict*zfa6761fefb,
ZSTD_CCtx*z9cf98d8515,std::string*z82c8d34a99){size_t zd6802bc739=
ZSTD_compressBound(data.size());if(zd6802bc739<(0xc07+3284-0x18da))return false;
*z82c8d34a99={};z82c8d34a99->resize(zd6802bc739);size_t const z275e0712cd=
ZSTD_compress_usingCDict(z9cf98d8515,(void*)z82c8d34a99->data(),zd6802bc739,data
.data(),data.size(),zfa6761fefb);if(ZSTD_isError(z275e0712cd))return false;
z82c8d34a99->resize(z275e0712cd);z82c8d34a99->shrink_to_fit();return true;}bool 
fc::zb6abe21710::zf08ccce3b6(const std::string&data,ZSTD_DDict*z8b51113b6e,
ZSTD_DCtx*z1050c12607,std::string*z1bbec3ce00){size_t zbbe0cd8734=
ZSTD_getDecompressedSize(data.data(),data.size());if(zbbe0cd8734<
(0x15a3+63-0x15e1))return false;*z1bbec3ce00={};z1bbec3ce00->resize(zbbe0cd8734)
;size_t const z79e9472fb1=ZSTD_decompress_usingDDict(z1050c12607,(void*)
z1bbec3ce00->data(),zbbe0cd8734,data.data(),data.size(),z8b51113b6e);if(
ZSTD_isError(z79e9472fb1))return false;z1bbec3ce00->resize(z79e9472fb1);
z1bbec3ce00->shrink_to_fit();return true;}float fc::zb6abe21710::z05fc9f353a(
float f,int z568a220ffa){union{int zdae0797406;float f;}zd102b906c5;z568a220ffa=
(0xe37+4539-0x1fd2)-z568a220ffa;zd102b906c5.f=f;zd102b906c5.zdae0797406=
zd102b906c5.zdae0797406+((0x1067+3247-0x1d15)<<(z568a220ffa-(0xf9c+43-0xfc6)));
zd102b906c5.zdae0797406=zd102b906c5.zdae0797406&(-(0x17d1+2286-0x20be)<<
z568a220ffa);return zd102b906c5.f;}std::string fc::zb6abe21710::z6ba11c04b2(
const std::vector<float>&zf82aea536b,const int p){if(zf82aea536b.size()>
(0xb7+8515-0x21fa)){int z098e48b8e6=zf82aea536b.size()*sizeof(zf82aea536b[
(0x1850+2113-0x2091)]);unsigned char z4e3bc1720c[z098e48b8e6];float z6789126fc4[
zf82aea536b.size()];if(p<(0x23b5+308-0x24c9)){for(unsigned int zdae0797406=
(0x923+3657-0x176c);zdae0797406<zf82aea536b.size();zdae0797406++)z6789126fc4[
zdae0797406]=fc::zb6abe21710::z05fc9f353a(zf82aea536b[zdae0797406],p);memcpy(&
z4e3bc1720c,&z6789126fc4,z098e48b8e6);}else{for(unsigned int zdae0797406=
(0x6b9+2531-0x109c);zdae0797406<zf82aea536b.size();zdae0797406++){char f[sizeof(
float)];float z6789126fc4=zf82aea536b[zdae0797406];memcpy(f,&z6789126fc4,sizeof(
float));z4e3bc1720c[zdae0797406*(0x438+4322-0x1516)]=f[(0x175a+441-0x1913)];
z4e3bc1720c[zdae0797406*(0x372+7466-0x2098)+(0x165a+674-0x18fb)]=f[
(0x12aa+3227-0x1f44)];z4e3bc1720c[zdae0797406*(0x779+6262-0x1feb)+
(0x740+7614-0x24fc)]=f[(0x1fb+718-0x4c7)];z4e3bc1720c[zdae0797406*
(0x1b4+766-0x4ae)+(0x49+3662-0xe94)]=f[(0x167+6829-0x1c11)];}}return std::string
(reinterpret_cast<const char*>(z4e3bc1720c),sizeof(z4e3bc1720c));}return std::
string("");}std::vector<float>fc::zb6abe21710::zdd5f92a70e(const std::string&s){
std::vector<float>z127251fe38;int size=s.length()/(0x7af+5141-0x1bc0);for(int 
zdae0797406=(0x5d8+6813-0x2075);zdae0797406<size;zdae0797406++){float f=0.0f;
const char*z4935204c40=s.c_str();memcpy(&f,&z4935204c40[zdae0797406*
(0x47b+3642-0x12b1)],sizeof(float));z127251fe38.push_back(f);}return z127251fe38
;}template<typename T>std::string fc::zb6abe21710::zd97f9decbf(const T&f){
unsigned char z4e3bc1720c[sizeof(f)];memcpy(&z4e3bc1720c,&f,sizeof(f));return 
std::string(reinterpret_cast<const char*>(z4e3bc1720c),sizeof(z4e3bc1720c));}
void fc::zb6abe21710::zdd53d384d8(const std::vector<float>&f,const float min,
const float max,std::vector<float>*z9954a0ec44){z9954a0ec44->reserve(f.size());
for(unsigned int zdae0797406=(0x80+7402-0x1d6a);zdae0797406<f.size();zdae0797406
++)z9954a0ec44->push_back(((f[zdae0797406]-min)/(max-min)));}void fc::
zb6abe21710::z10a058d443(const std::vector<float>&f,const float min,const float 
max,std::vector<float>*z9954a0ec44){z9954a0ec44->reserve(f.size());for(unsigned 
int zdae0797406=(0x1509+459-0x16d4);zdae0797406<f.size();zdae0797406++)
z9954a0ec44->push_back((min+(f[zdae0797406])*(max-min)));}void fc::zb6abe21710::
z10a058d443(const float min,const float max,std::vector<float>*z9954a0ec44){for(
unsigned int zdae0797406=(0x4c3+822-0x7f9);zdae0797406<z9954a0ec44->size();
zdae0797406++)z9954a0ec44->at(zdae0797406)=(min+(z9954a0ec44->at(zdae0797406))*(
max-min));}std::string fc::zb6abe21710::z4bfeee0ea7(const std::string&
zd56cbc2f33){static const char*const z851de2e7a0=
"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x41\x42\x43\x44\x45\x46";size_t 
z006e4786fa=zd56cbc2f33.length();std::string z127251fe38;z127251fe38.reserve(
(0x348+2558-0xd44)*z006e4786fa);for(size_t zdae0797406=(0x927+6796-0x23b3);
zdae0797406<z006e4786fa;++zdae0797406){const unsigned char zbdb9758852=
zd56cbc2f33[zdae0797406];z127251fe38.push_back(z851de2e7a0[zbdb9758852>>
(0x4e1+5726-0x1b3b)]);z127251fe38.push_back(z851de2e7a0[zbdb9758852&
(0x11bd+1970-0x1960)]);}return z127251fe38;}std::string fc::zb6abe21710::
zfc2c31d7b6(const std::string&zd56cbc2f33){static const char*const z851de2e7a0=
"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x41\x42\x43\x44\x45\x46";size_t 
z006e4786fa=zd56cbc2f33.length();if(z006e4786fa&(0x207+9362-0x2698))throw std::
invalid_argument(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x6f\x64\x64\x20\x6c\x65\x6e\x67\x74\x68"
);std::string z127251fe38;z127251fe38.reserve(z006e4786fa/(0x12d4+1319-0x17f9));
for(size_t zdae0797406=(0xee4+5988-0x2648);zdae0797406<z006e4786fa;zdae0797406+=
(0x13d4+40-0x13fa)){char a=zd56cbc2f33[zdae0797406];const char*p=std::
lower_bound(z851de2e7a0,z851de2e7a0+(0x2006+1117-0x2453),a);if(*p!=a)throw std::
invalid_argument(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x6e\x6f\x74\x20\x68\x65\x78\x20\x64\x69\x67\x69\x74"
);char b=zd56cbc2f33[zdae0797406+(0x13c3+3213-0x204f)];const char*zdfcb371881=
std::lower_bound(z851de2e7a0,z851de2e7a0+(0xd3+1423-0x652),b);if(*zdfcb371881!=b
)throw std::invalid_argument(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x6e\x6f\x74\x20\x68\x65\x78\x20\x64\x69\x67\x69\x74"
);z127251fe38.push_back(((p-z851de2e7a0)<<(0x377+3822-0x1261))|(zdfcb371881-
z851de2e7a0));}return z127251fe38;}std::string fc::zb6abe21710::zbbf6e99270(){
char z1e555ded70[(0x6f0+6803-0x2103)];std::string result="";FILE*pipe=popen(
"\x2f\x62\x69\x6e\x2f\x63\x61\x74\x20\x2f\x70\x72\x6f\x63\x2f\x63\x70\x75\x69\x6e\x66\x6f\x20\x7c\x20\x67\x72\x65\x70\x20\x27\x48\x61\x72\x64\x77\x61\x72\x65\x27"
,"\x72");if(!pipe)throw std::runtime_error(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x43\x61\x6e\x27\x74\x20\x64\x65\x74\x65\x63\x74\x20\x68\x61\x72\x64\x77\x61\x72\x65"
);try{while(fgets(z1e555ded70,sizeof z1e555ded70,pipe)!=NULL){result+=
z1e555ded70;}}catch(...){pclose(pipe);throw std::runtime_error(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x43\x61\x6e\x27\x74\x20\x64\x65\x74\x65\x63\x74\x20\x68\x61\x72\x64\x77\x61\x72\x65"
);}pclose(pipe);if(result.size()==(0xcd7+2432-0x1657))return
"\x75\x6e\x6b\x6e\x6f\x77\x6e";result=result.substr(result.find("\x3a")+
(0x1329+4089-0x2320));result.erase(std::remove(result.begin(),result.end(),'\n')
,result.end());return result;}template std::string fc::zb6abe21710::zd97f9decbf<
int>(const int&T);template std::string fc::zb6abe21710::zd97f9decbf<float>(const
 float&T);template std::string fc::zb6abe21710::zd97f9decbf<double>(const double
&T);template std::string fc::zb6abe21710::zd97f9decbf<long>(const long&T);
template std::string fc::zb6abe21710::zd97f9decbf<char>(const char&T);template 
std::string fc::zb6abe21710::zd97f9decbf<unsigned int>(const unsigned int&T);
template std::string fc::zb6abe21710::zd97f9decbf<unsigned short int>(const 
unsigned short int&T);
