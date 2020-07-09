
#include <algorithm>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <utility>
#include "QrCode.hpp"
using std::int8_t;using std::uint8_t;using std::size_t;using std::vector;
namespace zbaf424f1e4{z1b5e623412::z256f6dd853::z256f6dd853(int zaee1dcfb86,int 
zfb3b01fed7,int zcdf0d3e14c,int zbce0c52fe6):za314b3ec97(zaee1dcfb86){
ze8a8dd3a38[(0x1c4c+1706-0x22f6)]=zfb3b01fed7;ze8a8dd3a38[(0x2338+387-0x24ba)]=
zcdf0d3e14c;ze8a8dd3a38[(0x34d+6175-0x1b6a)]=zbce0c52fe6;}int z1b5e623412::
z256f6dd853::z5bfe456e49()const{return za314b3ec97;}int z1b5e623412::z256f6dd853
::z6ce1fc9c8c(int zd32a44ac7a)const{return ze8a8dd3a38[(zd32a44ac7a+
(0xea+2602-0xb0d))/(0x17c3+1818-0x1ecc)];}const z1b5e623412::z256f6dd853 
z1b5e623412::z256f6dd853::NUMERIC((0x1f89+1334-0x24be),(0x53b+5725-0x1b8e),
(0xd5+2451-0xa5c),(0xb59+2073-0x1364));const z1b5e623412::z256f6dd853 
z1b5e623412::z256f6dd853::zaea7b2bca8((0x81b+342-0x96f),(0xf99+2233-0x1849),
(0x2089+1404-0x25fa),(0x178d+2402-0x20e2));const z1b5e623412::z256f6dd853 
z1b5e623412::z256f6dd853::BYTE((0xbdf+3667-0x1a2e),(0xa21+1773-0x1106),
(0x76a+7152-0x234a),(0x16c4+2055-0x1ebb));const z1b5e623412::z256f6dd853 
z1b5e623412::z256f6dd853::za1b0fe5c40((0x2077+472-0x2247),(0xfc4+5536-0x255c),
(0x646+4260-0x16e0),(0x2f1+2100-0xb19));const z1b5e623412::z256f6dd853 
z1b5e623412::z256f6dd853::z5a4530705f((0x6d1+2270-0xfa8),(0x47c+38-0x4a2),
(0x1420+2769-0x1ef1),(0xb7d+2977-0x171e));z1b5e623412 z1b5e623412::z40b9d4bf17(
const vector<uint8_t>&data){if(data.size()>static_cast<unsigned int>(INT_MAX))
throw std::length_error("\x44\x61\x74\x61\x20\x74\x6f\x6f\x20\x6c\x6f\x6e\x67");
z720ec6b9f8 zd247bcd734;for(uint8_t b:data)zd247bcd734.z502e42f76a(b,
(0xaa1+5633-0x209a));return z1b5e623412(z256f6dd853::BYTE,static_cast<int>(data.
size()),std::move(zd247bcd734));}z1b5e623412 z1b5e623412::z4a04032e91(const char
*digits){z720ec6b9f8 zd247bcd734;int zc9ffe07094=(0x2367+401-0x24f8);int 
z19de74b92e=(0x743+3302-0x1429);int zf79cec0f05=(0xac+3874-0xfce);for(;*digits!=
'\0';digits++,zf79cec0f05++){char zbdb9758852=*digits;if(zbdb9758852<
((char)(0x412+5734-0x1a48))||zbdb9758852>((char)(0x148d+2509-0x1e21)))throw std
::domain_error(
"\x53\x74\x72\x69\x6e\x67\x20\x63\x6f\x6e\x74\x61\x69\x6e\x73\x20\x6e\x6f\x6e\x2d\x6e\x75\x6d\x65\x72\x69\x63\x20\x63\x68\x61\x72\x61\x63\x74\x65\x72\x73"
);zc9ffe07094=zc9ffe07094*(0x6d8+8244-0x2702)+(zbdb9758852-
((char)(0xccf+4589-0x1e8c)));z19de74b92e++;if(z19de74b92e==(0x1365+1035-0x176d))
{zd247bcd734.z502e42f76a(static_cast<uint32_t>(zc9ffe07094),(0x1739+310-0x1865))
;zc9ffe07094=(0x374+2373-0xcb9);z19de74b92e=(0x13d4+3062-0x1fca);}}if(
z19de74b92e>(0x523+7020-0x208f))zd247bcd734.z502e42f76a(static_cast<uint32_t>(
zc9ffe07094),z19de74b92e*(0xb3c+4966-0x1e9f)+(0x159c+700-0x1857));return 
z1b5e623412(z256f6dd853::NUMERIC,zf79cec0f05,std::move(zd247bcd734));}
z1b5e623412 z1b5e623412::za8e44351c4(const char*text){z720ec6b9f8 zd247bcd734;
int zc9ffe07094=(0x14ab+1660-0x1b27);int z19de74b92e=(0x9c9+2058-0x11d3);int 
zf79cec0f05=(0xea8+5179-0x22e3);for(;*text!='\0';text++,zf79cec0f05++){const 
char*z21fc38b1fe=std::strchr(z49b2d039dc,*text);if(z21fc38b1fe==nullptr)throw 
std::domain_error(
"\x53\x74\x72\x69\x6e\x67\x20\x63\x6f\x6e\x74\x61\x69\x6e\x73\x20\x75\x6e\x65\x6e\x63\x6f\x64\x61\x62\x6c\x65\x20\x63\x68\x61\x72\x61\x63\x74\x65\x72\x73\x20\x69\x6e\x20\x61\x6c\x70\x68\x61\x6e\x75\x6d\x65\x72\x69\x63\x20\x6d\x6f\x64\x65"
);zc9ffe07094=zc9ffe07094*(0x847+1986-0xfdc)+static_cast<int>(z21fc38b1fe-
z49b2d039dc);z19de74b92e++;if(z19de74b92e==(0x431+4261-0x14d4)){zd247bcd734.
z502e42f76a(static_cast<uint32_t>(zc9ffe07094),(0x1d6+5776-0x185b));zc9ffe07094=
(0x695+229-0x77a);z19de74b92e=(0x585+3152-0x11d5);}}if(z19de74b92e>
(0x77a+1332-0xcae))zd247bcd734.z502e42f76a(static_cast<uint32_t>(zc9ffe07094),
(0xf56+5797-0x25f5));return z1b5e623412(z256f6dd853::zaea7b2bca8,zf79cec0f05,std
::move(zd247bcd734));}vector<z1b5e623412>z1b5e623412::z8500ffd8c2(const char*
text){vector<z1b5e623412>result;if(*text=='\0');else if(ze8899fd68a(text))result
.push_back(z4a04032e91(text));else if(z01b004b2b9(text))result.push_back(
za8e44351c4(text));else{vector<uint8_t>z34ff3b6590;for(;*text!='\0';text++)
z34ff3b6590.push_back(static_cast<uint8_t>(*text));result.push_back(z40b9d4bf17(
z34ff3b6590));}return result;}z1b5e623412 z1b5e623412::zc22c182213(long 
zd2ff54314c){z720ec6b9f8 zd247bcd734;if(zd2ff54314c<(0x1973+1237-0x1e48))throw 
std::domain_error(
"\x45\x43\x49\x20\x61\x73\x73\x69\x67\x6e\x6d\x65\x6e\x74\x20\x76\x61\x6c\x75\x65\x20\x6f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65"
);else if(zd2ff54314c<((0x59f+7453-0x22bb)<<(0x433+813-0x759)))zd247bcd734.
z502e42f76a(static_cast<uint32_t>(zd2ff54314c),(0x13a4+3996-0x2338));else if(
zd2ff54314c<((0x20d7+1373-0x2633)<<(0x18c6+1884-0x2014))){zd247bcd734.
z502e42f76a((0x152b+3583-0x2328),(0x22a5+830-0x25e1));zd247bcd734.z502e42f76a(
static_cast<uint32_t>(zd2ff54314c),(0x367+5699-0x199c));}else if(zd2ff54314c<
1000000L){zd247bcd734.z502e42f76a((0x15c4+500-0x17b2),(0x205+2154-0xa6c));
zd247bcd734.z502e42f76a(static_cast<uint32_t>(zd2ff54314c),(0x1c54+94-0x1c9d));}
else throw std::domain_error(
"\x45\x43\x49\x20\x61\x73\x73\x69\x67\x6e\x6d\x65\x6e\x74\x20\x76\x61\x6c\x75\x65\x20\x6f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65"
);return z1b5e623412(z256f6dd853::z5a4530705f,(0xfc7+4271-0x2076),std::move(
zd247bcd734));}z1b5e623412::z1b5e623412(z256f6dd853 zdf9c4e1009,int zd2c4d9c00b,
const std::vector<bool>&z89d10be160):zaee1dcfb86(zdf9c4e1009),ze135af6c1c(
zd2c4d9c00b),data(z89d10be160){if(zd2c4d9c00b<(0x14f8+2874-0x2032))throw std::
domain_error("\x49\x6e\x76\x61\x6c\x69\x64\x20\x76\x61\x6c\x75\x65");}
z1b5e623412::z1b5e623412(z256f6dd853 zdf9c4e1009,int zd2c4d9c00b,std::vector<
bool>&&z89d10be160):zaee1dcfb86(zdf9c4e1009),ze135af6c1c(zd2c4d9c00b),data(std::
move(z89d10be160)){if(zd2c4d9c00b<(0x901+672-0xba1))throw std::domain_error(
"\x49\x6e\x76\x61\x6c\x69\x64\x20\x76\x61\x6c\x75\x65");}int z1b5e623412::
z82bc974bf9(const vector<z1b5e623412>&ze0e6a80a83,int z0f114fbf26){int result=
(0xbe7+756-0xedb);for(const z1b5e623412&z2172dd4474:ze0e6a80a83){int z97cdc44330
=z2172dd4474.zaee1dcfb86.z6ce1fc9c8c(z0f114fbf26);if(z2172dd4474.ze135af6c1c>=(
1L<<z97cdc44330))return-(0x544+2528-0xf23);if((0x1403+3497-0x21a8)+z97cdc44330>
INT_MAX-result)return-(0x505+3515-0x12bf);result+=(0x74+1485-0x63d)+z97cdc44330;
if(z2172dd4474.data.size()>static_cast<unsigned int>(INT_MAX-result))return-
(0x146+8493-0x2272);result+=static_cast<int>(z2172dd4474.data.size());}return 
result;}bool z1b5e623412::z01b004b2b9(const char*text){for(;*text!='\0';text++){
if(std::strchr(z49b2d039dc,*text)==nullptr)return false;}return true;}bool 
z1b5e623412::ze8899fd68a(const char*text){for(;*text!='\0';text++){char 
zbdb9758852=*text;if(zbdb9758852<((char)(0x315+2323-0xbf8))||zbdb9758852>
((char)(0xe19+3291-0x1abb)))return false;}return true;}z1b5e623412::z256f6dd853 
z1b5e623412::zeead436a72()const{return zaee1dcfb86;}int z1b5e623412::z1ea36e5689
()const{return ze135af6c1c;}const std::vector<bool>&z1b5e623412::ze69b1d231d()
const{return data;}const char*z1b5e623412::z49b2d039dc=
"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x20\x24\x25\x2a\x2b\x2d\x2e\x2f\x3a"
;int z3c5f83bd40::z84fd37b11f(z590c984210 zf3cf2af54e){switch(zf3cf2af54e){case 
z590c984210::z1ef8edc2c5:return(0x37f+5966-0x1acc);case z590c984210::z1b6b42fc3f
:return(0x9f4+6006-0x216a);case z590c984210::z19c3013ba1:return
(0x1712+1978-0x1ec9);case z590c984210::z6434537aa3:return(0x1f47+1087-0x2384);
default:throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");}}z3c5f83bd40 
z3c5f83bd40::z02069379ee(const char*text,z590c984210 zf3cf2af54e){vector<
z1b5e623412>ze0e6a80a83=z1b5e623412::z8500ffd8c2(text);return z7517157a9f(
ze0e6a80a83,zf3cf2af54e);}z3c5f83bd40 z3c5f83bd40::z58633d93cd(const vector<
uint8_t>&data,z590c984210 zf3cf2af54e){vector<z1b5e623412>ze0e6a80a83{
z1b5e623412::z40b9d4bf17(data)};return z7517157a9f(ze0e6a80a83,zf3cf2af54e);}
z3c5f83bd40 z3c5f83bd40::z7517157a9f(const vector<z1b5e623412>&ze0e6a80a83,
z590c984210 zf3cf2af54e,int z1d37f2c6cb,int zb774db64cb,int mask,bool 
z09eb3baa8e){if(!(z347e35b683<=z1d37f2c6cb&&z1d37f2c6cb<=zb774db64cb&&
zb774db64cb<=z00b5cd821b)||mask<-(0xc22+4643-0x1e44)||mask>(0x54+5901-0x175a))
throw std::invalid_argument(
"\x49\x6e\x76\x61\x6c\x69\x64\x20\x76\x61\x6c\x75\x65");int z0f114fbf26,
z49d05638fb;for(z0f114fbf26=z1d37f2c6cb;;z0f114fbf26++){int zafa8e0d61f=
z359c5fb056(z0f114fbf26,zf3cf2af54e)*(0x465+4809-0x1726);z49d05638fb=z1b5e623412
::z82bc974bf9(ze0e6a80a83,z0f114fbf26);if(z49d05638fb!=-(0x1c78+2382-0x25c5)&&
z49d05638fb<=zafa8e0d61f)break;if(z0f114fbf26>=zb774db64cb){std::ostringstream 
zfe83f70912;if(z49d05638fb==-(0x64a+6829-0x20f6))zfe83f70912<<
"\x53\x65\x67\x6d\x65\x6e\x74\x20\x74\x6f\x6f\x20\x6c\x6f\x6e\x67";else{
zfe83f70912<<"\x44\x61\x74\x61\x20\x6c\x65\x6e\x67\x74\x68\x20\x3d\x20"<<
z49d05638fb<<"\x20\x62\x69\x74\x73\x2c\x20";zfe83f70912<<
"\x4d\x61\x78\x20\x63\x61\x70\x61\x63\x69\x74\x79\x20\x3d\x20"<<zafa8e0d61f<<
"\x20\x62\x69\x74\x73";}throw z23205d62d7(zfe83f70912.str());}}if(z49d05638fb==-
(0xb86+5776-0x2215))throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");for(z590c984210 
z3d4cb85cee:vector<z590c984210>{z590c984210::z1b6b42fc3f,z590c984210::
z19c3013ba1,z590c984210::z6434537aa3}){if(z09eb3baa8e&&z49d05638fb<=z359c5fb056(
z0f114fbf26,z3d4cb85cee)*(0xd0+5979-0x1823))zf3cf2af54e=z3d4cb85cee;}z720ec6b9f8
 zd247bcd734;for(const z1b5e623412&z2172dd4474:ze0e6a80a83){zd247bcd734.
z502e42f76a(static_cast<uint32_t>(z2172dd4474.zeead436a72().z5bfe456e49()),
(0xbec+3360-0x1908));zd247bcd734.z502e42f76a(static_cast<uint32_t>(z2172dd4474.
z1ea36e5689()),z2172dd4474.zeead436a72().z6ce1fc9c8c(z0f114fbf26));zd247bcd734.
insert(zd247bcd734.end(),z2172dd4474.ze69b1d231d().begin(),z2172dd4474.
ze69b1d231d().end());}if(zd247bcd734.size()!=static_cast<unsigned int>(
z49d05638fb))throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");size_t 
zafa8e0d61f=static_cast<size_t>(z359c5fb056(z0f114fbf26,zf3cf2af54e))*
(0x417+6287-0x1c9e);if(zd247bcd734.size()>zafa8e0d61f)throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");zd247bcd734.
z502e42f76a((0x1be7+212-0x1cbb),std::min((0x975+4393-0x1a9a),static_cast<int>(
zafa8e0d61f-zd247bcd734.size())));zd247bcd734.z502e42f76a((0x34f+5728-0x19af),(
(0x4ea+8623-0x2691)-static_cast<int>(zd247bcd734.size()%(0xecc+3271-0x1b8b)))%
(0x157+4381-0x126c));if(zd247bcd734.size()%(0x816+3496-0x15b6)!=
(0x918+5153-0x1d39))throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");for(uint8_t 
z9121cbd47e=(0x1579+879-0x17fc);zd247bcd734.size()<zafa8e0d61f;z9121cbd47e^=
(0xac0+2464-0x1374)^(0x1991+954-0x1d3a))zd247bcd734.z502e42f76a(z9121cbd47e,
(0x1b2+901-0x52f));vector<uint8_t>zc2930c8b21(zd247bcd734.size()/
(0x2fd+8891-0x25b0));for(size_t zdae0797406=(0x767+1055-0xb86);zdae0797406<
zd247bcd734.size();zdae0797406++)zc2930c8b21[zdae0797406>>(0x20af+1070-0x24da)]
|=(zd247bcd734.at(zdae0797406)?(0xb1f+4936-0x1e66):(0x940+4027-0x18fb))<<(
(0x33b+7640-0x210c)-(zdae0797406&(0x1221+1782-0x1910)));return z3c5f83bd40(
z0f114fbf26,zf3cf2af54e,zc2930c8b21,mask);}z3c5f83bd40::z3c5f83bd40(int 
zd32a44ac7a,z590c984210 zf3cf2af54e,const vector<uint8_t>&zc2930c8b21,int 
zea1e3d34f8):z0f114fbf26(zd32a44ac7a),zd2d4f80ecd(zf3cf2af54e){if(zd32a44ac7a<
z347e35b683||zd32a44ac7a>z00b5cd821b)throw std::domain_error(
"\x56\x65\x72\x73\x69\x6f\x6e\x20\x76\x61\x6c\x75\x65\x20\x6f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65"
);if(zea1e3d34f8<-(0x33f+925-0x6db)||zea1e3d34f8>(0x976+866-0xcd1))throw std::
domain_error(
"\x4d\x61\x73\x6b\x20\x76\x61\x6c\x75\x65\x20\x6f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65"
);size=zd32a44ac7a*(0x858+5377-0x1d55)+(0xd76+2585-0x177e);size_t z5f9644194f=
static_cast<size_t>(size);z90d2fc7de3=vector<vector<bool> >(z5f9644194f,vector<
bool>(z5f9644194f));zd4cd416c40=vector<vector<bool> >(z5f9644194f,vector<bool>(
z5f9644194f));zd7c9232045();const vector<uint8_t>zb333b3ae90=zbe1adbc5b9(
zc2930c8b21);z15be5c29dd(zb333b3ae90);if(zea1e3d34f8==-(0xa9b+2495-0x1459)){long
 z5c9e193831=LONG_MAX;for(int zdae0797406=(0x116+7106-0x1cd8);zdae0797406<
(0x14f1+3131-0x2124);zdae0797406++){z1230ab18de(zdae0797406);zf6577773b2(
zdae0797406);long zb58bc42e93=z1e4f2c54f9();if(zb58bc42e93<z5c9e193831){
zea1e3d34f8=zdae0797406;z5c9e193831=zb58bc42e93;}z1230ab18de(zdae0797406);}}if(
zea1e3d34f8<(0x340+7866-0x21fa)||zea1e3d34f8>(0x19ca+403-0x1b56))throw std::
logic_error("\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");this
->mask=zea1e3d34f8;z1230ab18de(zea1e3d34f8);zf6577773b2(zea1e3d34f8);zd4cd416c40
.clear();zd4cd416c40.shrink_to_fit();}int z3c5f83bd40::z38334b03c3()const{return
 z0f114fbf26;}int z3c5f83bd40::z21480ff6d2()const{return size;}z3c5f83bd40::
z590c984210 z3c5f83bd40::z83bd2359db()const{return zd2d4f80ecd;}int z3c5f83bd40
::z149dc4901d()const{return mask;}bool z3c5f83bd40::zec6734e8d5(int x,int y)
const{return(0x1a49+818-0x1d7b)<=x&&x<size&&(0x1aeb+1881-0x2244)<=y&&y<size&&
z6e1b61e746(x,y);}std::string z3c5f83bd40::ze9ce39a19a(int z7068e5a047)const{if(
z7068e5a047<(0x373+3188-0xfe7))throw std::domain_error(
"\x42\x6f\x72\x64\x65\x72\x20\x6d\x75\x73\x74\x20\x62\x65\x20\x6e\x6f\x6e\x2d\x6e\x65\x67\x61\x74\x69\x76\x65"
);if(z7068e5a047>INT_MAX/(0x209f+553-0x22c6)||z7068e5a047*(0xaef+2978-0x168f)>
INT_MAX-size)throw std::overflow_error(
"\x42\x6f\x72\x64\x65\x72\x20\x74\x6f\x6f\x20\x6c\x61\x72\x67\x65");std::
ostringstream zfe83f70912;zfe83f70912<<
"\x3c\x3f\x78\x6d\x6c\x20\x76\x65\x72\x73\x69\x6f\x6e\x3d" "\"" "\x31\x2e\x30" "\"" "\x20\x65\x6e\x63\x6f\x64\x69\x6e\x67\x3d" "\"" "\x55\x54\x46\x2d\x38" "\"" "\x3f\x3e" "\n"
;zfe83f70912<<
"\x3c\x21\x44\x4f\x43\x54\x59\x50\x45\x20\x73\x76\x67\x20\x50\x55\x42\x4c\x49\x43\x20" "\"" "\x2d\x2f\x2f\x57\x33\x43\x2f\x2f\x44\x54\x44\x20\x53\x56\x47\x20\x31\x2e\x31\x2f\x2f\x45\x4e" "\"" "\x20" "\"" "\x68\x74\x74\x70\x3a\x2f\x2f\x77\x77\x77\x2e\x77\x33\x2e\x6f\x72\x67\x2f\x47\x72\x61\x70\x68\x69\x63\x73\x2f\x53\x56\x47\x2f\x31\x2e\x31\x2f\x44\x54\x44\x2f\x73\x76\x67\x31\x31\x2e\x64\x74\x64" "\"" "\x3e" "\n"
;zfe83f70912<<
"\x3c\x73\x76\x67\x20\x78\x6d\x6c\x6e\x73\x3d" "\"" "\x68\x74\x74\x70\x3a\x2f\x2f\x77\x77\x77\x2e\x77\x33\x2e\x6f\x72\x67\x2f\x32\x30\x30\x30\x2f\x73\x76\x67" "\"" "\x20\x76\x65\x72\x73\x69\x6f\x6e\x3d" "\"" "\x31\x2e\x31" "\"" "\x20\x76\x69\x65\x77\x42\x6f\x78\x3d" "\"" "\x30\x20\x30\x20"
;zfe83f70912<<(size+z7068e5a047*(0x1d28+723-0x1ff9))<<"\x20"<<(size+z7068e5a047*
(0x644+5387-0x1b4d))<<
"\"" "\x20\x73\x74\x72\x6f\x6b\x65\x3d" "\"" "\x6e\x6f\x6e\x65" "\"" "\x3e" "\n"
;zfe83f70912<<
"\t" "\x3c\x72\x65\x63\x74\x20\x77\x69\x64\x74\x68\x3d" "\"" "\x31\x30\x30\x25" "\"" "\x20\x68\x65\x69\x67\x68\x74\x3d" "\"" "\x31\x30\x30\x25" "\"" "\x20\x66\x69\x6c\x6c\x3d" "\"" "\x23\x46\x46\x46\x46\x46\x46" "\"" "\x2f\x3e" "\n"
;zfe83f70912<<"\t" "\x3c\x70\x61\x74\x68\x20\x64\x3d" "\"";for(int y=
(0x14a2+2528-0x1e82);y<size;y++){for(int x=(0x417+5747-0x1a8a);x<size;x++){if(
zec6734e8d5(x,y)){if(x!=(0xd62+5667-0x2385)||y!=(0xa41+845-0xd8e))zfe83f70912<<
"\x20";zfe83f70912<<"\x4d"<<(x+z7068e5a047)<<"\x2c"<<(y+z7068e5a047)<<
"\x68\x31\x76\x31\x68\x2d\x31\x7a";}}}zfe83f70912<<
"\"" "\x20\x66\x69\x6c\x6c\x3d" "\"" "\x23\x30\x30\x30\x30\x30\x30" "\"" "\x2f\x3e" "\n"
;zfe83f70912<<"\x3c\x2f\x73\x76\x67\x3e" "\n";return zfe83f70912.str();}void 
z3c5f83bd40::zd7c9232045(){for(int zdae0797406=(0x9b3+3263-0x1672);zdae0797406<
size;zdae0797406++){zd251a03c31((0x7f5+1589-0xe24),zdae0797406,zdae0797406%
(0xbaa+1236-0x107c)==(0x29c+8411-0x2377));zd251a03c31(zdae0797406,
(0xc97+3259-0x194c),zdae0797406%(0x889+1247-0xd66)==(0x228+831-0x567));}
zdf299815e4((0x1d67+992-0x2144),(0x120d+4372-0x231e));zdf299815e4(size-
(0x7c3+3085-0x13cc),(0x476+1081-0x8ac));zdf299815e4((0xdca+1498-0x13a1),size-
(0x12a4+4317-0x237d));const vector<int>z4e4720726f=z7f6612d664();size_t 
z98c42c5b20=z4e4720726f.size();for(size_t zdae0797406=(0x13c8+3948-0x2334);
zdae0797406<z98c42c5b20;zdae0797406++){for(size_t z211d9af9f8=
(0x4f7+6032-0x1c87);z211d9af9f8<z98c42c5b20;z211d9af9f8++){if(!((zdae0797406==
(0x1074+2504-0x1a3c)&&z211d9af9f8==(0xa94+4957-0x1df1))||(zdae0797406==
(0x646+204-0x712)&&z211d9af9f8==z98c42c5b20-(0xf5f+1035-0x1369))||(zdae0797406==
z98c42c5b20-(0x53c+5224-0x19a3)&&z211d9af9f8==(0xceb+3621-0x1b10))))z2db0555fbd(
z4e4720726f.at(zdae0797406),z4e4720726f.at(z211d9af9f8));}}zf6577773b2(
(0x284+9257-0x26ad));zf55ec4dede();}void z3c5f83bd40::zf6577773b2(int 
zea1e3d34f8){int data=z84fd37b11f(zd2d4f80ecd)<<(0xce6+6592-0x26a3)|zea1e3d34f8;
int rem=data;for(int zdae0797406=(0x1631+1465-0x1bea);zdae0797406<
(0x1763+362-0x18c3);zdae0797406++)rem=(rem<<(0xa5a+6887-0x2540))^((rem>>
(0x7d9+1029-0xbd5))*(0x831+5023-0x1699));int z568a220ffa=(data<<(0x2b7+78-0x2fb)
|rem)^21522;if(z568a220ffa>>(0x1cda+902-0x2051)!=(0x1522+3243-0x21cd))throw std
::logic_error("\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");
for(int zdae0797406=(0x1457+3948-0x23c3);zdae0797406<=(0xf14+4233-0x1f98);
zdae0797406++)zd251a03c31((0x54b+4351-0x1642),zdae0797406,z74547d9cfd(
z568a220ffa,zdae0797406));zd251a03c31((0x129f+832-0x15d7),(0xbe9+2968-0x177a),
z74547d9cfd(z568a220ffa,(0xaac+4675-0x1ce9)));zd251a03c31((0x275+5835-0x1938),
(0x7bd+7297-0x2436),z74547d9cfd(z568a220ffa,(0xdc7+3416-0x1b18)));zd251a03c31(
(0x2fc+8528-0x2445),(0x1585+3822-0x246b),z74547d9cfd(z568a220ffa,
(0x18f4+3093-0x2501)));for(int zdae0797406=(0x1316+51-0x1340);zdae0797406<
(0x1c72+2365-0x25a0);zdae0797406++)zd251a03c31((0x2b2+689-0x555)-zdae0797406,
(0x1194+1812-0x18a0),z74547d9cfd(z568a220ffa,zdae0797406));for(int zdae0797406=
(0x2dd+3749-0x1182);zdae0797406<(0x1b29+1357-0x206e);zdae0797406++)zd251a03c31(
size-(0x12cb+4029-0x2287)-zdae0797406,(0x118f+91-0x11e2),z74547d9cfd(z568a220ffa
,zdae0797406));for(int zdae0797406=(0xeba+6001-0x2623);zdae0797406<
(0x199+1400-0x702);zdae0797406++)zd251a03c31((0x10a5+3395-0x1de0),size-
(0x3bb+437-0x561)+zdae0797406,z74547d9cfd(z568a220ffa,zdae0797406));zd251a03c31(
(0x9e7+2938-0x1559),size-(0x14d8+994-0x18b2),true);}void z3c5f83bd40::
zf55ec4dede(){if(z0f114fbf26<(0x112c+2948-0x1ca9))return;int rem=z0f114fbf26;for
(int zdae0797406=(0x12fd+3409-0x204e);zdae0797406<(0x1163+3986-0x20e9);
zdae0797406++)rem=(rem<<(0x7f0+1311-0xd0e))^((rem>>(0xc18+3666-0x1a5f))*7973);
long z568a220ffa=static_cast<long>(z0f114fbf26)<<(0x173c+2401-0x2091)|rem;if(
z568a220ffa>>(0xc3c+5605-0x220f)!=(0xaf0+1193-0xf99))throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");for(int 
zdae0797406=(0x469+6243-0x1ccc);zdae0797406<(0x1cdc+2277-0x25af);zdae0797406++){
bool z9894daa966=z74547d9cfd(z568a220ffa,zdae0797406);int a=size-
(0x6ac+3680-0x1501)+zdae0797406%(0x1329+201-0x13ef);int b=zdae0797406/
(0x865+4307-0x1935);zd251a03c31(a,b,z9894daa966);zd251a03c31(b,a,z9894daa966);}}
void z3c5f83bd40::zdf299815e4(int x,int y){for(int z53004d47a8=-
(0x2b1+7583-0x204c);z53004d47a8<=(0x1c19+1689-0x22ae);z53004d47a8++){for(int 
ze7aa1b4c56=-(0x38b+5688-0x19bf);ze7aa1b4c56<=(0xf84+735-0x125f);ze7aa1b4c56++){
int z0575c41146=std::max(std::abs(ze7aa1b4c56),std::abs(z53004d47a8));int 
z8c8922da19=x+ze7aa1b4c56,zd09dd24503=y+z53004d47a8;if((0x516+3750-0x13bc)<=
z8c8922da19&&z8c8922da19<size&&(0x1548+3633-0x2379)<=zd09dd24503&&zd09dd24503<
size)zd251a03c31(z8c8922da19,zd09dd24503,z0575c41146!=(0x211+9421-0x26dc)&&
z0575c41146!=(0x10da+1139-0x1549));}}}void z3c5f83bd40::z2db0555fbd(int x,int y)
{for(int z53004d47a8=-(0xee0+288-0xffe);z53004d47a8<=(0xac6+2662-0x152a);
z53004d47a8++){for(int ze7aa1b4c56=-(0xf2+7048-0x1c78);ze7aa1b4c56<=
(0x1cda+2102-0x250e);ze7aa1b4c56++)zd251a03c31(x+ze7aa1b4c56,y+z53004d47a8,std::
max(std::abs(ze7aa1b4c56),std::abs(z53004d47a8))!=(0x759+5895-0x1e5f));}}void 
z3c5f83bd40::zd251a03c31(int x,int y,bool z0ef59dd5f3){size_t zc5b7eb4720=
static_cast<size_t>(x);size_t z664bcc5310=static_cast<size_t>(y);z90d2fc7de3.at(
z664bcc5310).at(zc5b7eb4720)=z0ef59dd5f3;zd4cd416c40.at(z664bcc5310).at(
zc5b7eb4720)=true;}bool z3c5f83bd40::z6e1b61e746(int x,int y)const{return 
z90d2fc7de3.at(static_cast<size_t>(y)).at(static_cast<size_t>(x));}vector<
uint8_t>z3c5f83bd40::zbe1adbc5b9(const vector<uint8_t>&data)const{if(data.size()
!=static_cast<unsigned int>(z359c5fb056(z0f114fbf26,zd2d4f80ecd)))throw std::
invalid_argument(
"\x49\x6e\x76\x61\x6c\x69\x64\x20\x61\x72\x67\x75\x6d\x65\x6e\x74");int 
z38dfb1fd61=zf73f648e88[static_cast<int>(zd2d4f80ecd)][z0f114fbf26];int 
z157a9ee03f=zfa2a1f592b[static_cast<int>(zd2d4f80ecd)][z0f114fbf26];int 
zf3d11ce9e0=z33991fbc31(z0f114fbf26)/(0x1099+5294-0x253f);int z25ca63d0fe=
z38dfb1fd61-zf3d11ce9e0%z38dfb1fd61;int zbc0224232e=zf3d11ce9e0/z38dfb1fd61;
vector<vector<uint8_t> >z5d9923bfcc;const vector<uint8_t>z7cc7a01aa7=z0f9af9fc3d
(z157a9ee03f);for(int zdae0797406=(0x162f+2029-0x1e1c),k=(0x1761+3248-0x2411);
zdae0797406<z38dfb1fd61;zdae0797406++){vector<uint8_t>z53dc48de88(data.cbegin()+
k,data.cbegin()+(k+zbc0224232e-z157a9ee03f+(zdae0797406<z25ca63d0fe?
(0x1b1a+36-0x1b3e):(0x42b+8340-0x24be))));k+=static_cast<int>(z53dc48de88.size()
);const vector<uint8_t>zbf3f6cd858=z9ba94cbd50(z53dc48de88,z7cc7a01aa7);if(
zdae0797406<z25ca63d0fe)z53dc48de88.push_back((0x6b2+870-0xa18));z53dc48de88.
insert(z53dc48de88.end(),zbf3f6cd858.cbegin(),zbf3f6cd858.cend());z5d9923bfcc.
push_back(std::move(z53dc48de88));}vector<uint8_t>result;for(size_t zdae0797406=
(0x1854+266-0x195e);zdae0797406<z5d9923bfcc.at((0x189a+162-0x193c)).size();
zdae0797406++){for(size_t z211d9af9f8=(0x563+1120-0x9c3);z211d9af9f8<z5d9923bfcc
.size();z211d9af9f8++){if(zdae0797406!=static_cast<unsigned int>(zbc0224232e-
z157a9ee03f)||z211d9af9f8>=static_cast<unsigned int>(z25ca63d0fe))result.
push_back(z5d9923bfcc.at(z211d9af9f8).at(zdae0797406));}}if(result.size()!=
static_cast<unsigned int>(zf3d11ce9e0))throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");return result;}
void z3c5f83bd40::z15be5c29dd(const vector<uint8_t>&data){if(data.size()!=
static_cast<unsigned int>(z33991fbc31(z0f114fbf26)/(0x722+5495-0x1c91)))throw 
std::invalid_argument(
"\x49\x6e\x76\x61\x6c\x69\x64\x20\x61\x72\x67\x75\x6d\x65\x6e\x74");size_t 
zdae0797406=(0x3bf+6222-0x1c0d);for(int right=size-(0xe48+5555-0x23fa);right>=
(0x101f+2860-0x1b4a);right-=(0x10cb+2031-0x18b8)){if(right==(0xea3+4310-0x1f73))
right=(0x8+9757-0x2620);for(int z431489de01=(0x147c+968-0x1844);z431489de01<size
;z431489de01++){for(int z211d9af9f8=(0xe23+3112-0x1a4b);z211d9af9f8<
(0xdb0+1269-0x12a3);z211d9af9f8++){size_t x=static_cast<size_t>(right-
z211d9af9f8);bool zbb2c9a8f92=((right+(0xe77+1216-0x1336))&(0x907+1187-0xda8))==
(0x33f+5581-0x190c);size_t y=static_cast<size_t>(zbb2c9a8f92?size-
(0xbcc+2955-0x1756)-z431489de01:z431489de01);if(!zd4cd416c40.at(y).at(x)&&
zdae0797406<data.size()*(0xe8f+5474-0x23e9)){z90d2fc7de3.at(y).at(x)=z74547d9cfd
(data.at(zdae0797406>>(0xff0+4256-0x208d)),(0x955+1343-0xe8d)-static_cast<int>(
zdae0797406&(0x21b4+449-0x236e)));zdae0797406++;}}}}if(zdae0797406!=data.size()*
(0x6e7+6617-0x20b8))throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");}void 
z3c5f83bd40::z1230ab18de(int zea1e3d34f8){if(zea1e3d34f8<(0x10+2196-0x8a4)||
zea1e3d34f8>(0x174b+3745-0x25e5))throw std::domain_error(
"\x4d\x61\x73\x6b\x20\x76\x61\x6c\x75\x65\x20\x6f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65"
);size_t z5f9644194f=static_cast<size_t>(size);for(size_t y=(0x14e0+1540-0x1ae4)
;y<z5f9644194f;y++){for(size_t x=(0x5e9+333-0x736);x<z5f9644194f;x++){bool 
zcf296f02bf;switch(zea1e3d34f8){case(0x686+7960-0x259e):zcf296f02bf=(x+y)%
(0x352+404-0x4e4)==(0x37a+5948-0x1ab6);break;case(0x8d4+7573-0x2668):zcf296f02bf
=y%(0x2055+674-0x22f5)==(0x839+3784-0x1701);break;case(0x3ac+7234-0x1fec):
zcf296f02bf=x%(0xdd0+3400-0x1b15)==(0x1d1+4042-0x119b);break;case
(0xb02+2459-0x149a):zcf296f02bf=(x+y)%(0x450+2635-0xe98)==(0xc15+357-0xd7a);
break;case(0x1248+3376-0x1f74):zcf296f02bf=(x/(0x184c+341-0x199e)+y/
(0x3f4+546-0x614))%(0x462+2407-0xdc7)==(0x4b7+6146-0x1cb9);break;case
(0x1063+1508-0x1642):zcf296f02bf=x*y%(0x178f+1113-0x1be6)+x*y%
(0xa30+6205-0x226a)==(0x19ef+2740-0x24a3);break;case(0x6f3+5193-0x1b36):
zcf296f02bf=(x*y%(0x2234+770-0x2534)+x*y%(0x23d9+302-0x2504))%
(0x943+2670-0x13af)==(0xfc9+4043-0x1f94);break;case(0x2a5+7004-0x1dfa):
zcf296f02bf=((x+y)%(0x13d8+540-0x15f2)+x*y%(0x1758+920-0x1aed))%
(0x1763+181-0x1816)==(0x1678+3106-0x229a);break;default:throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");}z90d2fc7de3.at(
y).at(x)=z90d2fc7de3.at(y).at(x)^(zcf296f02bf&!zd4cd416c40.at(y).at(x));}}}long 
z3c5f83bd40::z1e4f2c54f9()const{long result=(0x108a+2526-0x1a68);for(int y=
(0x2ca+7246-0x1f18);y<size;y++){bool z4cbc0b85f7=false;int z28df46e91c=
(0x963+3930-0x18bd);std::array<int,(0x80f+5741-0x1e75)>zb978164446={};for(int x=
(0x2309+124-0x2385);x<size;x++){if(z6e1b61e746(x,y)==z4cbc0b85f7){z28df46e91c++;
if(z28df46e91c==(0x5c8+6268-0x1e3f))result+=z8c9d7d1daa;else if(z28df46e91c>
(0x1aa0+1566-0x20b9))result++;}else{z3569aa693b(z28df46e91c,zb978164446);if(!
z4cbc0b85f7)result+=z9a6f931e13(zb978164446)*z1264521603;z4cbc0b85f7=z6e1b61e746
(x,y);z28df46e91c=(0x1d00+284-0x1e1b);}}result+=z1223a1d8b4(z4cbc0b85f7,
z28df46e91c,zb978164446)*z1264521603;}for(int x=(0xc03+2475-0x15ae);x<size;x++){
bool z4cbc0b85f7=false;int zc93150e157=(0x632+7529-0x239b);std::array<int,
(0x2474+69-0x24b2)>zb978164446={};for(int y=(0xd4b+122-0xdc5);y<size;y++){if(
z6e1b61e746(x,y)==z4cbc0b85f7){zc93150e157++;if(zc93150e157==(0x8ba+5671-0x1edc)
)result+=z8c9d7d1daa;else if(zc93150e157>(0x2c7+8772-0x2506))result++;}else{
z3569aa693b(zc93150e157,zb978164446);if(!z4cbc0b85f7)result+=z9a6f931e13(
zb978164446)*z1264521603;z4cbc0b85f7=z6e1b61e746(x,y);zc93150e157=
(0x1df+3214-0xe6c);}}result+=z1223a1d8b4(z4cbc0b85f7,zc93150e157,zb978164446)*
z1264521603;}for(int y=(0x7e4+2104-0x101c);y<size-(0x58+799-0x376);y++){for(int 
x=(0x4e2+3899-0x141d);x<size-(0xfb+5663-0x1719);x++){bool z12ee0fe6d8=
z6e1b61e746(x,y);if(z12ee0fe6d8==z6e1b61e746(x+(0x1bb+5465-0x1713),y)&&
z12ee0fe6d8==z6e1b61e746(x,y+(0x547+1955-0xce9))&&z12ee0fe6d8==z6e1b61e746(x+
(0x67b+7694-0x2488),y+(0x309+6074-0x1ac2)))result+=zc90fbd058b;}}int za4864414c0
=(0xa40+2607-0x146f);for(const vector<bool>&zdeaa629292:z90d2fc7de3){for(bool 
z12ee0fe6d8:zdeaa629292){if(z12ee0fe6d8)za4864414c0++;}}int z4d943009b1=size*
size;int k=static_cast<int>((std::abs(za4864414c0*20L-z4d943009b1*10L)+
z4d943009b1-(0x15d3+13-0x15df))/z4d943009b1)-(0x1600+2707-0x2092);result+=k*
z0c4fc248ce;return result;}vector<int>z3c5f83bd40::z7f6612d664()const{if(
z0f114fbf26==(0x388+5013-0x171c))return vector<int>();else{int z98c42c5b20=
z0f114fbf26/(0x32b+4950-0x167a)+(0x1171+2603-0x1b9a);int z785890a0b4=(
z0f114fbf26==(0x582+5879-0x1c59))?(0xe87+2759-0x1934):(z0f114fbf26*
(0x994+1118-0xdee)+z98c42c5b20*(0x1f0+1203-0x6a1)+(0x16d9+1723-0x1d93))/(
z98c42c5b20*(0x1fc8+746-0x22b0)-(0x1eb0+2084-0x26d2))*(0x1783+2895-0x22d0);
vector<int>result;for(int zdae0797406=(0x242+6010-0x19bc),pos=size-
(0x1116+4886-0x2425);zdae0797406<z98c42c5b20-(0x3e7+4519-0x158d);zdae0797406++,
pos-=z785890a0b4)result.insert(result.begin(),pos);result.insert(result.begin(),
(0xe4d+2627-0x188a));return result;}}int z3c5f83bd40::z33991fbc31(int 
zd32a44ac7a){if(zd32a44ac7a<z347e35b683||zd32a44ac7a>z00b5cd821b)throw std::
domain_error(
"\x56\x65\x72\x73\x69\x6f\x6e\x20\x6e\x75\x6d\x62\x65\x72\x20\x6f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65"
);int result=((0x90a+350-0xa58)*zd32a44ac7a+(0x1229+1752-0x1881))*zd32a44ac7a+
(0x13bf+3691-0x21ea);if(zd32a44ac7a>=(0x1a1+4057-0x1178)){int z98c42c5b20=
zd32a44ac7a/(0x1af7+2517-0x24c5)+(0x17a8+3033-0x237f);result-=(
(0x13f6+1048-0x17f5)*z98c42c5b20-(0xe0+3494-0xe7c))*z98c42c5b20-
(0x1230+4489-0x2382);if(zd32a44ac7a>=(0x1313+3662-0x215a))result-=
(0x870+871-0xbb3);}if(!((0x1b04+2083-0x2257)<=result&&result<=29648))throw std::
logic_error("\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");
return result;}int z3c5f83bd40::z359c5fb056(int zd32a44ac7a,z590c984210 
zf3cf2af54e){return z33991fbc31(zd32a44ac7a)/(0x1c7+2599-0xbe6)-zfa2a1f592b[
static_cast<int>(zf3cf2af54e)][zd32a44ac7a]*zf73f648e88[static_cast<int>(
zf3cf2af54e)][zd32a44ac7a];}vector<uint8_t>z3c5f83bd40::z0f9af9fc3d(int 
z02e404e748){if(z02e404e748<(0x92+8514-0x21d3)||z02e404e748>(0x2ff+2666-0xc6a))
throw std::domain_error(
"\x44\x65\x67\x72\x65\x65\x20\x6f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65");
vector<uint8_t>result(static_cast<size_t>(z02e404e748));result.at(result.size()-
(0x578+1497-0xb50))=(0xc37+3272-0x18fe);uint8_t z0cc8a22122=(0x940+4103-0x1946);
for(int zdae0797406=(0xfa6+5062-0x236c);zdae0797406<z02e404e748;zdae0797406++){
for(size_t z211d9af9f8=(0xc58+339-0xdab);z211d9af9f8<result.size();z211d9af9f8++
){result.at(z211d9af9f8)=z3a9284966c(result.at(z211d9af9f8),z0cc8a22122);if(
z211d9af9f8+(0xe95+1461-0x1449)<result.size())result.at(z211d9af9f8)^=result.at(
z211d9af9f8+(0x1b0+9540-0x26f3));}z0cc8a22122=z3a9284966c(z0cc8a22122,
(0x11ba+2475-0x1b63));}return result;}vector<uint8_t>z3c5f83bd40::z9ba94cbd50(
const vector<uint8_t>&data,const vector<uint8_t>&z33736c2a49){vector<uint8_t>
result(z33736c2a49.size());for(uint8_t b:data){uint8_t zfdad8bbcbb=b^result.at(
(0x8c1+5620-0x1eb5));result.erase(result.begin());result.push_back(
(0x332+5865-0x1a1b));for(size_t zdae0797406=(0x61c+6556-0x1fb8);zdae0797406<
result.size();zdae0797406++)result.at(zdae0797406)^=z3a9284966c(z33736c2a49.at(
zdae0797406),zfdad8bbcbb);}return result;}uint8_t z3c5f83bd40::z3a9284966c(
uint8_t x,uint8_t y){int z9b8b1f1d80=(0xd5d+5465-0x22b6);for(int zdae0797406=
(0xd7f+1727-0x1437);zdae0797406>=(0x1cdc+1778-0x23ce);zdae0797406--){z9b8b1f1d80
=(z9b8b1f1d80<<(0x504+1302-0xa19))^((z9b8b1f1d80>>(0x1a98+2496-0x2451))*
(0x566+6407-0x1d50));z9b8b1f1d80^=((y>>zdae0797406)&(0xf+3141-0xc53))*x;}if(
z9b8b1f1d80>>(0x772+7114-0x2334)!=(0xf0d+6044-0x26a9))throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");return 
static_cast<uint8_t>(z9b8b1f1d80);}int z3c5f83bd40::z9a6f931e13(const std::array
<int,(0x28f+2534-0xc6e)>&zb978164446)const{int n=zb978164446.at(
(0x9a3+2464-0x1342));if(n>size*(0x24a+3983-0x11d6))throw std::logic_error(
"\x41\x73\x73\x65\x72\x74\x69\x6f\x6e\x20\x65\x72\x72\x6f\x72");bool z255208324c
=n>(0x8c6+4592-0x1ab6)&&zb978164446.at((0xa5a+7228-0x2694))==n&&zb978164446.at(
(0x1188+2091-0x19b0))==n*(0x8b8+324-0x9f9)&&zb978164446.at((0x1d15+736-0x1ff1))
==n&&zb978164446.at((0x12ff+140-0x1386))==n;return(z255208324c&&zb978164446.at(
(0x11e5+4468-0x2359))>=n*(0x136c+3318-0x205e)&&zb978164446.at(
(0x1606+4026-0x25ba))>=n?(0x18e9+3403-0x2633):(0x8d4+7183-0x24e3))+(z255208324c
&&zb978164446.at((0x6ad+7515-0x2402))>=n*(0x4d8+133-0x559)&&zb978164446.at(
(0x922+1597-0xf5f))>=n?(0x11a9+2635-0x1bf3):(0x629+8017-0x257a));}int 
z3c5f83bd40::z1223a1d8b4(bool z4d9de8d45a,int z9e7ae813b1,std::array<int,
(0x262+2319-0xb6a)>&zb978164446)const{if(z4d9de8d45a){z3569aa693b(z9e7ae813b1,
zb978164446);z9e7ae813b1=(0x11ef+298-0x1319);}z9e7ae813b1+=size;z3569aa693b(
z9e7ae813b1,zb978164446);return z9a6f931e13(zb978164446);}void z3c5f83bd40::
z3569aa693b(int z9e7ae813b1,std::array<int,(0x99a+4735-0x1c12)>&zb978164446)
const{if(zb978164446.at((0xa29+207-0xaf8))==(0xe8a+6061-0x2637))z9e7ae813b1+=
size;std::copy_backward(zb978164446.cbegin(),zb978164446.cend()-
(0x1405+1574-0x1a2a),zb978164446.end());zb978164446.at((0xf23+4289-0x1fe4))=
z9e7ae813b1;}bool z3c5f83bd40::z74547d9cfd(long x,int zdae0797406){return((x>>
zdae0797406)&(0x9f0+1835-0x111a))!=(0x293+4946-0x15e5);}const int z3c5f83bd40::
z8c9d7d1daa=(0xaea+5471-0x2046);const int z3c5f83bd40::zc90fbd058b=
(0x7cf+2850-0x12ee);const int z3c5f83bd40::z1264521603=(0x17e2+480-0x199a);const
 int z3c5f83bd40::z0c4fc248ce=(0x25b+6217-0x1a9a);const int8_t z3c5f83bd40::
zfa2a1f592b[(0x463+8204-0x246b)][(0xb05+2843-0x15f7)]={{-(0x6f2+6320-0x1fa1),
(0x17f9+1782-0x1ee8),(0x81+9231-0x2486),(0x5e3+4668-0x1810),(0x1784+1787-0x1e6b)
,(0x69c+4903-0x19a9),(0x108f+4051-0x2050),(0x1324+4430-0x245e),
(0x57a+4766-0x1800),(0x225c+153-0x22d7),(0x640+3211-0x12b9),(0x198+2653-0xbe1),
(0x160f+3679-0x2456),(0x6a7+112-0x6fd),(0x4d4+3558-0x129c),(0xe6f+3486-0x1bf7),
(0xb0f+1303-0x100e),(0x864+6673-0x2259),(0xa8b+6327-0x2324),(0x41d+889-0x77a),
(0x2b6+5201-0x16eb),(0x1632+2235-0x1ed1),(0x8c1+5731-0x1f08),(0xf92+1291-0x147f)
,(0x15f6+1942-0x1d6e),(0x984+5199-0x1db9),(0xc29+2522-0x15e7),
(0x54b+7426-0x222f),(0x65f+394-0x7cb),(0x13db+3692-0x2229),(0x32d+6239-0x1b6e),
(0xfb2+3660-0x1de0),(0x945+3573-0x171c),(0x193+7404-0x1e61),(0x25f+5359-0x1730),
(0x563+6559-0x1ee4),(0x15cf+3565-0x239e),(0x1d7+3971-0x113c),
(0x163b+1816-0x1d35),(0xb37+2518-0x14ef),(0x121b+1186-0x169f)},{-
(0x323+2527-0xd01),(0x1e5c+98-0x1eb4),(0x1d2a+403-0x1ead),(0x591+4245-0x160c),
(0x1f0d+480-0x20db),(0xccd+4880-0x1fc5),(0x8fa+1155-0xd6d),(0x11f4+2036-0x19d6),
(0xb61+1288-0x1053),(0xc94+3577-0x1a77),(0x21e0+1196-0x2672),(0x727+1652-0xd7d),
(0x3c3+1589-0x9e2),(0x7cd+6471-0x20fe),(0xc9b+6524-0x25ff),(0x274+1539-0x85f),
(0x1482+843-0x17b1),(0x1468+2183-0x1cd3),(0x14b+4820-0x1405),(0xe2f+2128-0x1665)
,(0x991+5364-0x1e6b),(0x18dd+1107-0x1d16),(0x43c+5475-0x1983),
(0x120f+2851-0x1d16),(0x373+4263-0x13fe),(0xa1a+3762-0x18b0),
(0x14ec+2189-0x1d5d),(0x1f3d+1933-0x26ae),(0x1ac0+2891-0x25ef),
(0x17f7+104-0x1843),(0x7f4+1654-0xe4e),(0x107+7675-0x1ee6),(0x9a4+7047-0x250f),
(0x1df8+1519-0x23cb),(0x51c+6194-0x1d32),(0x711+5447-0x1c3c),(0x9c9+3010-0x156f)
,(0x441+7268-0x2089),(0x494+7023-0x1fe7),(0x1deb+1358-0x231d),(0x5d7+1421-0xb48)
},{-(0x1958+1792-0x2057),(0x3c+4315-0x110a),(0x1161+2019-0x192e),
(0x164+3747-0xff5),(0x222+5685-0x183d),(0x700+5447-0x1c35),(0x1825+371-0x1980),
(0xda9+4117-0x1dac),(0x842+6009-0x1fa5),(0x116a+747-0x1441),(0x223b+1170-0x26b5)
,(0xdef+3510-0x1b89),(0x606+7098-0x21a6),(0xb06+3500-0x189a),(0x2c1+9035-0x25f8)
,(0x48a+5921-0x1b8d),(0x1a7d+823-0x1d9c),(0xe5+8762-0x2303),(0xaf8+6590-0x249a),
(0x8a+262-0x176),(0x8dc+398-0xa4c),(0x1cf+3995-0x114e),(0x1dc8+1459-0x235d),
(0x11b+265-0x206),(0x143c+1541-0x1a23),(0xf25+4789-0x21bc),(0x1926+2608-0x233a),
(0xb98+1535-0x1179),(0x46d+1229-0x91c),(0xff6+1107-0x142b),(0x534+7723-0x2341),
(0x298+679-0x521),(0x931+3442-0x1685),(0x6a1+735-0x962),(0x62+2252-0x910),
(0x92+7418-0x1d6e),(0x100f+3869-0x1f0e),(0x12db+3990-0x2253),(0x9b5+4429-0x1ae4)
,(0x1e8f+1937-0x2602),(0x161+39-0x16a)},{-(0x1e33+1241-0x230b),
(0x13c1+339-0x1503),(0xcb9+6276-0x2521),(0xec2+17-0xebd),(0x5c7+1815-0xcce),
(0xca+3083-0xcbf),(0x331+6293-0x1baa),(0xc05+3900-0x1b27),(0x86+2810-0xb66),
(0x17cf+1381-0x1d1c),(0x96d+6224-0x21a1),(0x66f+479-0x836),(0x175+664-0x3f1),
(0x1431+1517-0x1a08),(0x7d9+798-0xadf),(0x1c77+573-0x1e9c),(0x1882+1044-0x1c78),
(0x1adb+2094-0x22ed),(0x12c3+407-0x143e),(0xda0+78-0xdd4),(0x1304+1847-0x1a1f),
(0x2016+87-0x204f),(0x1382+4854-0x2660),(0x1a23+1392-0x1f75),(0xc09+871-0xf52),
(0x451+1701-0xad8),(0x88f+6843-0x232c),(0x1ad6+1139-0x1f2b),(0x1b97+1276-0x2075)
,(0x105a+2659-0x1a9f),(0xd8a+3615-0x1b8b),(0x500+2175-0xd61),
(0x1064+4038-0x200c),(0x18cc+1154-0x1d30),(0x1b46+2892-0x2674),
(0x125c+1186-0x16e0),(0x19bc+2162-0x2210),(0x2104+670-0x2384),(0x47+1435-0x5c4),
(0x13e6+2993-0x1f79),(0x339+6377-0x1c04)},};const int8_t z3c5f83bd40::
zf73f648e88[(0x11a6+2770-0x1c74)][(0x7b4+7410-0x247d)]={{-(0xa30+5526-0x1fc5),
(0x952+58-0x98b),(0x841+2464-0x11e0),(0xb36+1213-0xff2),(0xae9+4813-0x1db5),
(0x10b1+4803-0x2373),(0x18f4+722-0x1bc4),(0x107+627-0x378),(0x1777+1165-0x1c02),
(0x264+2101-0xa97),(0x1e01+878-0x216b),(0x894+7558-0x2616),(0x2279+940-0x2621),
(0x6f9+7517-0x2452),(0x92f+5261-0x1db8),(0x64a+3811-0x1527),(0x10e8+2172-0x195e)
,(0xb5d+593-0xda8),(0x10f5+2143-0x194e),(0x812+1839-0xf3a),(0xa97+1689-0x1128),
(0xaa+3944-0x100a),(0x1440+4207-0x24a6),(0xd57+5536-0x22ee),(0x98+7009-0x1bef),
(0xf3d+3567-0x1d20),(0x1c3d+1476-0x21f5),(0xc52+3685-0x1aab),(0x1b29+872-0x1e84)
,(0x239c+516-0x2592),(0x5b0+5011-0x1934),(0xda1+5529-0x232a),(0xa3a+541-0xc46),
(0x8a4+1858-0xfd4),(0x575+902-0x8e8),(0x189b+603-0x1ae3),(0xb44+3731-0x19c3),
(0x124d+4452-0x239c),(0x55f+7596-0x22f5),(0x2ab+441-0x44c),(0x188b+3143-0x24b9)}
,{-(0x9d4+4156-0x1a0f),(0x6c1+562-0x8f2),(0xf6d+3653-0x1db1),(0x594+1144-0xa0b),
(0xfc+5899-0x1805),(0x19c0+375-0x1b35),(0x2e5+1331-0x814),(0x1356+3744-0x21f2),
(0xce3+4067-0x1cc2),(0x11a3+4966-0x2504),(0x1874+2983-0x2416),
(0xa26+6138-0x221b),(0x116b+3595-0x1f6e),(0x1322+2933-0x1e8e),
(0x3e5+8982-0x26f2),(0x1124+2150-0x1980),(0x479+6850-0x1f31),(0xef4+754-0x11db),
(0x2b0+7746-0x20e5),(0x1ac4+1862-0x21fc),(0xa7c+7021-0x25d9),(0x643+658-0x8c4),
(0xd83+4960-0x20d2),(0x28+228-0xfa),(0x11ab+5072-0x2567),(0x561+6457-0x1e85),
(0xcd7+2815-0x17bf),(0x169a+413-0x181e),(0x23bf+751-0x2694),(0x27c+5275-0x16fb),
(0x18b2+1114-0x1cef),(0x128f+2536-0x1c58),(0x5f4+2459-0xf6e),(0xab6+4826-0x1d6d)
,(0x1201+417-0x137d),(0x1c21+2688-0x267b),(0x116b+3457-0x1ec4),
(0xa2a+2743-0x14b6),(0x580+4072-0x153b),(0x2083+1351-0x259b),(0x707+6655-0x20d5)
},{-(0x138f+1965-0x1b3b),(0x22af+974-0x267c),(0x1542+34-0x1563),
(0x612+2743-0x10c7),(0x14b+8636-0x2305),(0xea5+1627-0x14fc),(0x1f0+3461-0xf71),
(0x16b9+1474-0x1c75),(0x10fc+3350-0x1e0c),(0xbe7+5158-0x2005),
(0x99a+2331-0x12ad),(0x1ca+4418-0x1304),(0xb1f+4265-0x1bbe),(0x122b+373-0x1394),
(0xa4c+3621-0x1861),(0x210f+1419-0x268e),(0x11e9+5276-0x2674),(0xe52+521-0x104b)
,(0x99c+5629-0x1f87),(0x2231+864-0x257c),(0x706+513-0x8f3),(0xcf4+5238-0x2153),
(0x492+8711-0x2682),(0x890+692-0xb2b),(0x663+4485-0x17cd),(0x190f+368-0x1a62),
(0x1d34+161-0x1db3),(0xc04+1304-0x10fa),(0x13f7+1438-0x1972),(0x3f0+8226-0x23ec)
,(0x263+6365-0x1b18),(0xb81+3570-0x1948),(0xd16+1276-0x11e5),
(0x10c5+1049-0x14ae),(0x13ca+4418-0x24d9),(0x6ef+1199-0xb69),
(0x2040+1169-0x2499),(0x61+6883-0x1b09),(0x666+7181-0x2235),(0x1cdb+1697-0x233b)
,(0xa4c+251-0xb03)},{-(0x1938+3488-0x26d7),(0x97+6920-0x1b9e),
(0x96a+2364-0x12a5),(0xcdf+4080-0x1ccd),(0x2082+7-0x2085),(0xef1+3561-0x1cd6),
(0x223f+692-0x24ef),(0x189a+3677-0x26f2),(0x923+1720-0xfd5),(0xd8a+4319-0x1e61),
(0xff2+2453-0x197f),(0xb77+2894-0x16ba),(0x202+7902-0x20d5),(0x140f+2171-0x1c7a)
,(0xd64+4860-0x2050),(0xfb7+3919-0x1ef4),(0x94f+7184-0x254f),
(0x17bb+1186-0x1c4a),(0x185b+751-0x1b35),(0xa01+378-0xb62),(0x1008+1456-0x159f),
(0x348+800-0x64f),(0x2f0+4318-0x13ac),(0x1279+4041-0x2224),(0x17eb+1580-0x1df7),
(0x49d+7815-0x2301),(0xa54+4297-0x1af8),(0x479+689-0x702),(0x21f+9270-0x262b),
(0x103+2982-0xc7c),(0x7c9+3984-0x1729),(0x3ab+6446-0x1ca6),(0xbb8+4554-0x1d4c),
(0x13c2+1071-0x17b8),(0x86+5603-0x162d),(0xbdf+5396-0x20b4),(0xc87+6268-0x24c1),
(0x1a33+2072-0x2205),(0x3f0+4280-0x145e),(0xe6d+2235-0x16db),
(0x132b+4602-0x24d4)},};z23205d62d7::z23205d62d7(const std::string&msg):std::
length_error(msg){}z720ec6b9f8::z720ec6b9f8():std::vector<bool>(){}void 
z720ec6b9f8::z502e42f76a(std::uint32_t val,int z006e4786fa){if(z006e4786fa<
(0x1d30+1278-0x222e)||z006e4786fa>(0x15fb+2897-0x212d)||val>>z006e4786fa!=
(0x1edc+821-0x2211))throw std::domain_error(
"\x56\x61\x6c\x75\x65\x20\x6f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65");for(
int zdae0797406=z006e4786fa-(0xa89+3800-0x1960);zdae0797406>=(0x4d9+4254-0x1577)
;zdae0797406--)this->push_back(((val>>zdae0797406)&(0x78b+2760-0x1252))!=
(0x158c+380-0x1708));}}
