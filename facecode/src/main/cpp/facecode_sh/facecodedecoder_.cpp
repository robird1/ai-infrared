
#include "facecodedecoder_.h"
fc::FaceCodeDecoder_::FaceCodeDecoder_(){z47946b1a48=fc::zb6abe21710::
z958fd1b03e();z68914b4d84=ZSTD_createDCtx();zb52cd36923=std::unique_ptr<
z2344c2fefe>(new z2344c2fefe());std::string zf000340631=fc::zb6abe21710::
zbbf6e99270();if(zf000340631=="\x42\x43\x4d\x32\x38\x33\x35")z6d51c28ec3=
((char)(0x1557+1635-0x1b87));else if(zf000340631=="\x55\x4c\x31\x30\x30")
z6d51c28ec3=((char)(0x4da+264-0x5b1));else if(zf000340631==
"\x69\x43\x61\x74\x63\x68\x34")z6d51c28ec3=((char)(0x3c6+8725-0x25a9));else 
z6d51c28ec3=((char)(0x1489+995-0x17f2));}bool fc::FaceCodeDecoder_::decode(const
 std::vector<char>&z7dfd43b573){z8292b6ea34=std::string(z7dfd43b573.begin(),
z7dfd43b573.begin()+z4d2715c1d8);char z6158fec8a6=z7dfd43b573[z4d2715c1d8];if(
z8292b6ea34!="\x31\x61"||z7dfd43b573.size()<z490fea18bc+ze4756f313d+
(0x5f3+7692-0x23fb)||(z6158fec8a6!=z6d51c28ec3&&z6158fec8a6!=
((char)(0x132+6185-0x192b))))return false;switch(z7dfd43b573.at(z4d2715c1d8+
z126460a381)){case((char)(0xd83+1555-0x1335)):return fc::FaceCodeDecoder_::
zeb9d7ce3df(z7dfd43b573);case((char)(0x1471+1885-0x1b6c)):return fc::
FaceCodeDecoder_::za6fde61b0e(z7dfd43b573);case((char)(0x1247+288-0x1304)):
return fc::FaceCodeDecoder_::z4283f72bc3(z7dfd43b573);case
((char)(0x10c3+3558-0x1e45)):return fc::FaceCodeDecoder_::zccb5769d0d(
z7dfd43b573);case((char)(0x1c2+8532-0x22e6)):return fc::FaceCodeDecoder_::
z5a36db6ce4(std::string(z7dfd43b573.begin(),z7dfd43b573.end()));case
((char)(0x1d4b+1338-0x2254)):return fc::FaceCodeDecoder_::ze01618f12f(std::
string(z7dfd43b573.begin(),z7dfd43b573.end()));case((char)(0x13cc+3257-0x2053)):
return fc::FaceCodeDecoder_::z0c056b70fa(std::string(z7dfd43b573.begin(),
z7dfd43b573.end()));case((char)(0x189b+3468-0x25f4)):return fc::FaceCodeDecoder_
::zc50f723fd5(std::string(z7dfd43b573.begin(),z7dfd43b573.end()));case
((char)(0x1c0d+1187-0x207c)):return fc::FaceCodeDecoder_::z8950aa19b8(std::
string(z7dfd43b573.begin(),z7dfd43b573.end()));case((char)(0x3e8+976-0x783)):
return fc::FaceCodeDecoder_::zc5e9e7490f(std::string(z7dfd43b573.begin(),
z7dfd43b573.end()));case((char)(0x172+9386-0x25e6)):return fc::FaceCodeDecoder_
::zf7daf2ffca(std::string(z7dfd43b573.begin(),z7dfd43b573.end()));case
((char)(0x3df+7715-0x21cb)):return fc::FaceCodeDecoder_::zbcc9453911(std::string
(z7dfd43b573.begin(),z7dfd43b573.end()));default:return false;}}bool fc::
FaceCodeDecoder_::decode(const std::string&z7dfd43b573){z8292b6ea34=std::string(
z7dfd43b573.begin(),z7dfd43b573.begin()+z4d2715c1d8);char z6158fec8a6=
z7dfd43b573[z4d2715c1d8];if(z8292b6ea34!="\x31\x61"||z7dfd43b573.size()<
z490fea18bc+ze4756f313d+(0x135d+67-0x139c)||(z6158fec8a6!=z6d51c28ec3&&
z6158fec8a6!=((char)(0x678+7355-0x2303))))return false;switch(z7dfd43b573.at(
z4d2715c1d8+z126460a381)){case((char)(0x16ad+68-0x1690)):return fc::
FaceCodeDecoder_::zeb9d7ce3df(std::vector<char>(z7dfd43b573.begin(),z7dfd43b573.
end()));case((char)(0xeb+121-0x102)):return fc::FaceCodeDecoder_::za6fde61b0e(
std::vector<char>(z7dfd43b573.begin(),z7dfd43b573.end()));case
((char)(0x70d+6295-0x1f41)):return fc::FaceCodeDecoder_::z4283f72bc3(std::vector
<char>(z7dfd43b573.begin(),z7dfd43b573.end()));case((char)(0x2014+695-0x2267)):
return fc::FaceCodeDecoder_::zccb5769d0d(std::vector<char>(z7dfd43b573.begin(),
z7dfd43b573.end()));case((char)(0x5f5+4297-0x168e)):return fc::FaceCodeDecoder_
::z5a36db6ce4(z7dfd43b573);case((char)(0xd30+272-0xe0f)):return fc::
FaceCodeDecoder_::ze01618f12f(z7dfd43b573);case((char)(0xf3d+4455-0x2072)):
return fc::FaceCodeDecoder_::z0c056b70fa(z7dfd43b573);case
((char)(0x1266+1848-0x196b)):return fc::FaceCodeDecoder_::zc50f723fd5(
z7dfd43b573);case((char)(0x127+3262-0xdb1)):return fc::FaceCodeDecoder_::
z8950aa19b8(z7dfd43b573);case((char)(0x2440+410-0x25a5)):return fc::
FaceCodeDecoder_::zc5e9e7490f(z7dfd43b573);case((char)(0xbd8+4574-0x1d80)):
return fc::FaceCodeDecoder_::zf7daf2ffca(z7dfd43b573);case
((char)(0x90c+5337-0x1dae)):return fc::FaceCodeDecoder_::zbcc9453911(z7dfd43b573
);default:return false;}}bool fc::FaceCodeDecoder_::set_public_RSA_signature_key
(const CryptoPP::RSA::PublicKey&z291e63aee2){if(z291e63aee2.GetModulus().
BitCount()!=RSA_KEY_SIZE_BITS)return false;zf83a932b82.AssignFrom(z291e63aee2);
return true;}bool fc::FaceCodeDecoder_::set_private_RSA_decryption_key(const 
CryptoPP::RSA::PrivateKey&z3943db4c39){if(z3943db4c39.GetModulus().BitCount()!=
RSA_KEY_SIZE_BITS)return false;zc06a26b9e7.AssignFrom(z3943db4c39);return true;}
bool fc::FaceCodeDecoder_::z68d0ffc068(const std::string&s,const std::string&
zc52f7d2a20,std::string*z7dfd43b573){if(z8292b6ea34=="\x31\x61"){return fc::
zb6abe21710::zf08ccce3b6(fc::za2341d21ce::zbd0a8b580a(s,zc52f7d2a20),z47946b1a48
,z68914b4d84,z7dfd43b573);}return false;}bool fc::FaceCodeDecoder_::zc50f723fd5(
const std::string&z7dfd43b573){std::string z0a0baf1cef=z7dfd43b573.substr(
(0x39d+1729-0xa5e),z490fea18bc)+base<(0x12bb+4320-0x2340)>::decode(z7dfd43b573.
substr(z490fea18bc));return zccb5769d0d(std::vector<char>(z0a0baf1cef.begin(),
z0a0baf1cef.end()));}bool fc::FaceCodeDecoder_::zbcc9453911(const std::string&
z7dfd43b573){std::string z0a0baf1cef=z7dfd43b573.substr((0xbc0+894-0xf3e),
z490fea18bc)+base<(0x47a+6577-0x1deb)>::decode(z7dfd43b573.substr(z490fea18bc));
return zccb5769d0d(std::vector<char>(z0a0baf1cef.begin(),z0a0baf1cef.end()));}
bool fc::FaceCodeDecoder_::zccb5769d0d(const std::vector<char>&z7dfd43b573){std
::vector<fc::any>().swap(z51ffa25fd4);std::string zc52f7d2a20(z7dfd43b573.begin(
)+z490fea18bc,z7dfd43b573.begin()+z490fea18bc+ze4756f313d);std::string message;
if(!fc::FaceCodeDecoder_::z68d0ffc068(std::string(z7dfd43b573.begin()+
z490fea18bc+ze4756f313d,z7dfd43b573.end()),zc52f7d2a20,&message))return false;if
(zc52f7d2a20.compare(fc::za2341d21ce::za12655acbc(std::string(z7dfd43b573.begin(
),z7dfd43b573.begin()+z4d2715c1d8+z126460a381)+message))==(0x1178+3014-0x1d3e)){
std::vector<std::string>z9954a0ec44;int zfd2800da92;memcpy(&zfd2800da92,&message
[(0x1d30+1617-0x2381)],sizeof(unsigned int));if(z8292b6ea34=="\x31\x61"){std::
vector<float>().swap(zba61b40ae7);ze50048830a=message.substr(sizeof(unsigned int
),zfd2800da92);zb52cd36923->init(ze50048830a);zb52cd36923->z68d0ffc068(
z8012218f33,&zba61b40ae7);zb52cd36923->z2244c89e7f();fc::zb6abe21710::
z10a058d443(zcaf2ec8b61,z412161793e,&zba61b40ae7);z9954a0ec44=fc::zb6abe21710::
ze153efa0be(message.substr(sizeof(unsigned int)+zfd2800da92),
((char)(0x4d9+7033-0x2017)));}for(unsigned int zdae0797406=(0x1ced+950-0x20a2);
zdae0797406<z9954a0ec44.size();zdae0797406++){switch(z9954a0ec44[
(0xa3d+6672-0x244d)].at(zdae0797406-(0x1776+3068-0x2371))){case
((char)(0x741+281-0x7f6)):z51ffa25fd4.push_back(*(reinterpret_cast<const int*>(
z9954a0ec44[zdae0797406].c_str())));break;case((char)(0x1a9+8909-0x2410)):
z51ffa25fd4.push_back(*(reinterpret_cast<const float*>(z9954a0ec44[zdae0797406].
c_str())));break;case((char)(0x1b72+1024-0x1f03)):z51ffa25fd4.push_back(*(
reinterpret_cast<const double*>(z9954a0ec44[zdae0797406].c_str())));break;case
((char)(0xf37+5870-0x25b2)):z51ffa25fd4.push_back(fc::any_cast<std::string>(
z9954a0ec44[zdae0797406]));break;case((char)(0x176a+986-0x1ad8)):z51ffa25fd4.
push_back(*(reinterpret_cast<const long*>(z9954a0ec44[zdae0797406].c_str())));
break;case((char)(0x81d+3122-0x13ec)):z51ffa25fd4.push_back(*((z9954a0ec44[
zdae0797406].c_str())));break;default:return false;}}return true;}else{return 
false;}}bool fc::FaceCodeDecoder_::z5a36db6ce4(const std::string&z7dfd43b573){
std::string z0a0baf1cef=z7dfd43b573.substr((0xf1+9144-0x24a9),z490fea18bc)+base<
(0xf77+4453-0x2081)>::decode(z7dfd43b573.substr(z490fea18bc));return zeb9d7ce3df
(std::vector<char>(z0a0baf1cef.begin(),z0a0baf1cef.end()));}bool fc::
FaceCodeDecoder_::z8950aa19b8(const std::string&z7dfd43b573){std::string 
z0a0baf1cef=z7dfd43b573.substr((0x182f+539-0x1a4a),z490fea18bc)+base<
(0x759+2225-0xfca)>::decode(z7dfd43b573.substr(z490fea18bc));return zeb9d7ce3df(
std::vector<char>(z0a0baf1cef.begin(),z0a0baf1cef.end()));}bool fc::
FaceCodeDecoder_::zeb9d7ce3df(const std::vector<char>&z7dfd43b573){std::vector<
fc::any>().swap(z51ffa25fd4);std::string zb03064e0ff(z7dfd43b573.begin()+
z490fea18bc,z7dfd43b573.begin()+z490fea18bc+zbda7cbaa43);std::string zc52f7d2a20
(z7dfd43b573.begin()+z490fea18bc+zbda7cbaa43,z7dfd43b573.begin()+z490fea18bc+
zbda7cbaa43+zaaa411a714);std::string zffd10f4903;if(!fc::za2341d21ce::
z665dc26c9f(zc06a26b9e7,zc52f7d2a20,&zffd10f4903))return false;std::string 
message=fc::za2341d21ce::z32d07e8312(zffd10f4903,zb03064e0ff,std::string(
z7dfd43b573.begin()+z490fea18bc+zbda7cbaa43+zaaa411a714,z7dfd43b573.end()));std
::string z736e09c875;if(!fc::FaceCodeDecoder_::z68d0ffc068(message,zffd10f4903,&
z736e09c875))return false;if(zffd10f4903.compare(fc::za2341d21ce::za12655acbc(
std::string(z7dfd43b573.begin(),z7dfd43b573.begin()+z4d2715c1d8+z126460a381)+
z736e09c875))==(0x139b+4198-0x2401)){std::vector<std::string>z9954a0ec44;
unsigned int zfd2800da92;memcpy(&zfd2800da92,&z736e09c875[(0x2f2+1819-0xa0d)],
sizeof(unsigned int));if(z8292b6ea34=="\x31\x61"){std::vector<float>().swap(
zba61b40ae7);ze50048830a=z736e09c875.substr(sizeof(unsigned int),zfd2800da92);
zb52cd36923->init(ze50048830a);zb52cd36923->z68d0ffc068(z8012218f33,&zba61b40ae7
);zb52cd36923->z2244c89e7f();fc::zb6abe21710::z10a058d443(zcaf2ec8b61,
z412161793e,&zba61b40ae7);z9954a0ec44=fc::zb6abe21710::ze153efa0be(z736e09c875.
substr(sizeof(unsigned int)+zfd2800da92),((char)(0x370+4809-0x15fe)));}for(
unsigned int zdae0797406=(0xb0f+2637-0x155b);zdae0797406<z9954a0ec44.size();
zdae0797406++){switch(z9954a0ec44[(0x1192+5145-0x25ab)].at(zdae0797406-
(0x7c2+282-0x8db))){case((char)(0x151+3991-0x1084)):z51ffa25fd4.push_back(*(
reinterpret_cast<const int*>(z9954a0ec44[zdae0797406].c_str())));break;case
((char)(0x902+4831-0x1b7b)):z51ffa25fd4.push_back(*(reinterpret_cast<const float
*>(z9954a0ec44[zdae0797406].c_str())));break;case((char)(0x20fb+692-0x2340)):
z51ffa25fd4.push_back(*(reinterpret_cast<const double*>(z9954a0ec44[zdae0797406]
.c_str())));break;case((char)(0x700+4043-0x1658)):z51ffa25fd4.push_back(fc::
any_cast<std::string>(z9954a0ec44[zdae0797406]));break;case
((char)(0x15d1+3025-0x2136)):z51ffa25fd4.push_back(*(reinterpret_cast<const long
*>(z9954a0ec44[zdae0797406].c_str())));break;case((char)(0xc77+3589-0x1a19)):
z51ffa25fd4.push_back(*((z9954a0ec44[zdae0797406].c_str())));break;default:
return false;}}return true;}else{return false;}}bool fc::FaceCodeDecoder_::
z0c056b70fa(const std::string&z7dfd43b573){std::string z0a0baf1cef=z7dfd43b573.
substr((0x1efb+241-0x1fec),z490fea18bc)+base<(0x1097+5474-0x259e)>::decode(
z7dfd43b573.substr(z490fea18bc));return z4283f72bc3(std::vector<char>(
z0a0baf1cef.begin(),z0a0baf1cef.end()));}bool fc::FaceCodeDecoder_::zf7daf2ffca(
const std::string&z7dfd43b573){std::string z0a0baf1cef=z7dfd43b573.substr(
(0xf3+1045-0x508),z490fea18bc)+base<(0x7d0+317-0x8cd)>::decode(z7dfd43b573.
substr(z490fea18bc));return z4283f72bc3(std::vector<char>(z0a0baf1cef.begin(),
z0a0baf1cef.end()));}bool fc::FaceCodeDecoder_::z4283f72bc3(const std::vector<
char>&z7dfd43b573){std::vector<fc::any>().swap(z51ffa25fd4);std::string 
z1b036b7299(z7dfd43b573.begin()+z490fea18bc,z7dfd43b573.begin()+z490fea18bc+
z26e4df40b8);std::string zc52f7d2a20(z7dfd43b573.begin()+z490fea18bc+z26e4df40b8
,z7dfd43b573.begin()+z490fea18bc+z26e4df40b8+ze4756f313d);std::string message;if
(!fc::FaceCodeDecoder_::z68d0ffc068(std::string(z7dfd43b573.begin()+z490fea18bc+
z26e4df40b8+ze4756f313d,z7dfd43b573.end()),zc52f7d2a20,&message))return false;if
(fc::za2341d21ce::ze3763691e2(zf83a932b82,zc52f7d2a20,z1b036b7299)==true&&
zc52f7d2a20.compare(fc::za2341d21ce::za12655acbc(std::string(z7dfd43b573.begin()
,z7dfd43b573.begin()+z4d2715c1d8+z126460a381)+message))==(0x112b+5518-0x26b9)){
std::vector<std::string>z9954a0ec44;int zfd2800da92;memcpy(&zfd2800da92,&message
[(0xaab+2135-0x1302)],sizeof(unsigned int));if(z8292b6ea34=="\x31\x61"){std::
vector<float>().swap(zba61b40ae7);ze50048830a=message.substr(sizeof(unsigned int
),zfd2800da92);zb52cd36923->init(ze50048830a);zb52cd36923->z68d0ffc068(
z8012218f33,&zba61b40ae7);zb52cd36923->z2244c89e7f();fc::zb6abe21710::
z10a058d443(zcaf2ec8b61,z412161793e,&zba61b40ae7);z9954a0ec44=fc::zb6abe21710::
ze153efa0be(message.substr(sizeof(unsigned int)+zfd2800da92),
((char)(0x126a+4123-0x224a)));}for(unsigned int zdae0797406=(0x1315+958-0x16d2);
zdae0797406<z9954a0ec44.size();zdae0797406++){switch(z9954a0ec44[
(0x1651+709-0x1916)].at(zdae0797406-(0x1375+3208-0x1ffc))){case
((char)(0xba+1644-0x6c2)):z51ffa25fd4.push_back(*(reinterpret_cast<const int*>(
z9954a0ec44[zdae0797406].c_str())));break;case((char)(0x187b+2794-0x22ff)):
z51ffa25fd4.push_back(*(reinterpret_cast<const float*>(z9954a0ec44[zdae0797406].
c_str())));break;case((char)(0x569+1134-0x968)):z51ffa25fd4.push_back(*(
reinterpret_cast<const double*>(z9954a0ec44[zdae0797406].c_str())));break;case
((char)(0xf79+237-0xff3)):z51ffa25fd4.push_back(fc::any_cast<std::string>(
z9954a0ec44[zdae0797406]));break;case((char)(0x102d+1726-0x167f)):z51ffa25fd4.
push_back(*(reinterpret_cast<const long*>(z9954a0ec44[zdae0797406].c_str())));
break;case((char)(0xaf8+6601-0x245e)):z51ffa25fd4.push_back(*((z9954a0ec44[
zdae0797406].c_str())));break;default:return false;}}return true;}else{return 
false;}}bool fc::FaceCodeDecoder_::ze01618f12f(const std::string&z7dfd43b573){
std::string z0a0baf1cef=z7dfd43b573.substr((0x11d0+2715-0x1c6b),z490fea18bc)+
base<(0x615+70-0x600)>::decode(z7dfd43b573.substr(z490fea18bc));return 
za6fde61b0e(std::vector<char>(z0a0baf1cef.begin(),z0a0baf1cef.end()));}bool fc::
FaceCodeDecoder_::zc5e9e7490f(const std::string&z7dfd43b573){std::string 
z0a0baf1cef=z7dfd43b573.substr((0x112f+5516-0x26bb),z490fea18bc)+base<
(0x120d+4227-0x2250)>::decode(z7dfd43b573.substr(z490fea18bc));return 
za6fde61b0e(std::vector<char>(z0a0baf1cef.begin(),z0a0baf1cef.end()));}bool fc::
FaceCodeDecoder_::za6fde61b0e(const std::vector<char>&z7dfd43b573){std::vector<
fc::any>().swap(z51ffa25fd4);std::string z1b036b7299(z7dfd43b573.begin()+
z490fea18bc,z7dfd43b573.begin()+z490fea18bc+z26e4df40b8);std::string zb03064e0ff
(z7dfd43b573.begin()+z490fea18bc+z26e4df40b8,z7dfd43b573.begin()+z490fea18bc+
z26e4df40b8+zbda7cbaa43);std::string zc52f7d2a20(z7dfd43b573.begin()+z490fea18bc
+z26e4df40b8+zbda7cbaa43,z7dfd43b573.begin()+z490fea18bc+z26e4df40b8+zbda7cbaa43
+zaaa411a714);std::string zffd10f4903;if(!fc::za2341d21ce::z665dc26c9f(
zc06a26b9e7,zc52f7d2a20,&zffd10f4903))return false;if(!fc::za2341d21ce::
ze3763691e2(zf83a932b82,zffd10f4903,z1b036b7299))return false;std::string 
z7e62702cfc=fc::za2341d21ce::z32d07e8312(zffd10f4903,zb03064e0ff,std::string(
z7dfd43b573.begin()+z490fea18bc+z26e4df40b8+zbda7cbaa43+zaaa411a714,z7dfd43b573.
end()));std::string message;if(!fc::FaceCodeDecoder_::z68d0ffc068(z7e62702cfc,
zffd10f4903,&message))return false;if(zffd10f4903.compare(fc::za2341d21ce::
za12655acbc(std::string(z7dfd43b573.begin(),z7dfd43b573.begin()+z4d2715c1d8+
z126460a381)+message))==(0x549+3873-0x146a)){std::vector<std::string>z9954a0ec44
;int zfd2800da92;memcpy(&zfd2800da92,&message[(0x1566+3552-0x2346)],sizeof(
unsigned int));if(z8292b6ea34=="\x31\x61"){std::vector<float>().swap(zba61b40ae7
);ze50048830a=message.substr(sizeof(unsigned int),zfd2800da92);zb52cd36923->init
(ze50048830a);zb52cd36923->z68d0ffc068(z8012218f33,&zba61b40ae7);zb52cd36923->
z2244c89e7f();fc::zb6abe21710::z10a058d443(zcaf2ec8b61,z412161793e,&zba61b40ae7)
;z9954a0ec44=fc::zb6abe21710::ze153efa0be(message.substr(sizeof(unsigned int)+
zfd2800da92),((char)(0x36d+1466-0x8ec)));}for(unsigned int zdae0797406=
(0x16da+3879-0x2600);zdae0797406<z9954a0ec44.size();zdae0797406++){switch(
z9954a0ec44[(0x229d+785-0x25ae)].at(zdae0797406-(0xbe4+5408-0x2103))){case
((char)(0xe9a+4229-0x1ebb)):z51ffa25fd4.push_back(*(reinterpret_cast<const int*>
(z9954a0ec44[zdae0797406].c_str())));break;case((char)(0xe5c+2788-0x18da)):
z51ffa25fd4.push_back(*(reinterpret_cast<const float*>(z9954a0ec44[zdae0797406].
c_str())));break;case((char)(0x1e5+7872-0x2036)):z51ffa25fd4.push_back(*(
reinterpret_cast<const double*>(z9954a0ec44[zdae0797406].c_str())));break;case
((char)(0x34f+5496-0x1854)):z51ffa25fd4.push_back(fc::any_cast<std::string>(
z9954a0ec44[zdae0797406]));break;case((char)(0xc7+4250-0x10f5)):z51ffa25fd4.
push_back(*(reinterpret_cast<const long*>(z9954a0ec44[zdae0797406].c_str())));
break;case((char)(0x96b+5808-0x1fb8)):z51ffa25fd4.push_back(*((z9954a0ec44[
zdae0797406].c_str())));break;default:return false;}}return true;}else{return 
false;}}std::vector<float>fc::FaceCodeDecoder_::get_code()const{return 
zba61b40ae7;}int fc::FaceCodeDecoder_::get_data_size()const{return z51ffa25fd4.
size();}std::string fc::FaceCodeDecoder_::get_version(const std::vector<char>&
z878675891b)const{return std::string(z878675891b.begin(),z878675891b.begin()+
z4d2715c1d8);}std::string fc::FaceCodeDecoder_::get_version(const std::string&
z878675891b)const{return std::string(z878675891b.begin(),z878675891b.begin()+
z4d2715c1d8);}std::string fc::FaceCodeDecoder_::get_device(const std::vector<
char>&z878675891b)const{switch(z878675891b.at(z4d2715c1d8)){case
((char)(0x18ab+2312-0x2183)):return"\x41\x4e\x59";case
((char)(0x1201+4355-0x22d3)):return"\x55\x4c\x31\x30\x30";case
((char)(0x3c8+666-0x630)):return"\x69\x43\x61\x74\x63\x68\x34";case
((char)(0x1478+939-0x17f0)):return
"\x52\x61\x73\x70\x62\x65\x72\x72\x79\x20\x50\x69\x20\x33\x42\x2b";}return
"\x6e\x6f\x6e\x65";}std::string fc::FaceCodeDecoder_::get_device(const std::
string&z878675891b)const{return get_device(std::vector<char>(z878675891b.begin()
,z878675891b.end()));}std::string fc::FaceCodeDecoder_::get_header(const std::
vector<char>&z878675891b)const{return std::string(z878675891b.begin()+
z4d2715c1d8+z126460a381+z83a89c5820,z878675891b.begin()+z490fea18bc);}std::
string fc::FaceCodeDecoder_::get_header(const std::string&z878675891b)const{
return std::string(z878675891b.begin()+z4d2715c1d8+z126460a381+z83a89c5820,
z878675891b.begin()+z490fea18bc);}std::string fc::FaceCodeDecoder_::get_type(
const std::vector<char>&z878675891b)const{switch(z878675891b.at(z4d2715c1d8+
z126460a381)){case((char)(0xc35+565-0xe09)):return
"\x65\x6e\x63\x72\x79\x70\x74\x65\x64\x20\x62\x69\x6e\x61\x72\x79";case
((char)(0xe03+1555-0x13b4)):return
"\x65\x6e\x63\x72\x79\x70\x74\x65\x64\x20\x73\x69\x67\x6e\x65\x64\x20\x62\x69\x6e\x61\x72\x79"
;case((char)(0x14ac+3792-0x2319)):return
"\x73\x69\x67\x6e\x65\x64\x20\x62\x69\x6e\x61\x72\x79";case
((char)(0x459+8780-0x2641)):return"\x62\x69\x6e\x61\x72\x79";case
((char)(0x261+2935-0xda8)):return
"\x65\x6e\x63\x72\x79\x70\x74\x65\x64\x20\x74\x65\x78\x74\x20\x62\x61\x73\x65\x20\x39\x31"
;case((char)(0xc83+5939-0x2385)):return
"\x65\x6e\x63\x72\x79\x70\x74\x65\x64\x20\x73\x69\x67\x6e\x65\x64\x20\x74\x65\x78\x74\x20\x62\x61\x73\x65\x20\x39\x31"
;case((char)(0x3a9+3175-0xfde)):return
"\x73\x69\x67\x6e\x65\x64\x20\x74\x65\x78\x74\x20\x62\x61\x73\x65\x20\x39\x31";
case((char)(0x1d18+2015-0x24c4)):return
"\x74\x65\x78\x74\x20\x62\x61\x73\x65\x20\x39\x31";case((char)(0xd32+249-0xdf7))
:return
"\x65\x6e\x63\x72\x79\x70\x74\x65\x64\x20\x74\x65\x78\x74\x20\x62\x61\x73\x65\x20\x36\x34"
;case((char)(0x12f5+4977-0x2631)):return
"\x65\x6e\x63\x72\x79\x70\x74\x65\x64\x20\x73\x69\x67\x6e\x65\x64\x20\x74\x65\x78\x74\x20\x62\x61\x73\x65\x20\x36\x34"
;case((char)(0x7fb+4378-0x18df)):return
"\x73\x69\x67\x6e\x65\x64\x20\x74\x65\x78\x74\x20\x62\x61\x73\x65\x20\x36\x34";
case((char)(0x12db+2906-0x1dfe)):return
"\x74\x65\x78\x74\x20\x62\x61\x73\x65\x20\x36\x34";}return"\x6e\x6f\x6e\x65";}
std::string fc::FaceCodeDecoder_::get_type(const std::string&z878675891b)const{
return get_type(std::vector<char>(z878675891b.begin(),z878675891b.end()));}std::
pair<fc::Type,void*>fc::FaceCodeDecoder_::get_data(const int&z27662fd564){if(
z27662fd564<(0x168d+3817-0x2576)||z27662fd564>(int)z51ffa25fd4.size())throw std
::out_of_range(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x67\x65\x74\x5f\x64\x61\x74\x61\x28\x29\x3a\x45\x72\x72\x6f\x72\x20\x4f\x75\x74\x20\x6f\x66\x20\x72\x61\x6e\x67\x65"
);if(z51ffa25fd4[z27662fd564].type()==typeid(int))return std::make_pair(fc::Type
::INT,(fc::any_cast<int>(&z51ffa25fd4[z27662fd564])));else if(z51ffa25fd4[
z27662fd564].type()==typeid(float))return std::make_pair(fc::Type::FLOAT,(fc::
any_cast<float>(&z51ffa25fd4[z27662fd564])));else if(z51ffa25fd4[z27662fd564].
type()==typeid(double))return std::make_pair(fc::Type::DOUBLE,(fc::any_cast<
double>(&z51ffa25fd4[z27662fd564])));else if(z51ffa25fd4[z27662fd564].type()==
typeid(std::string))return std::make_pair(fc::Type::STRING,(fc::any_cast<std::
string>(&z51ffa25fd4[z27662fd564])));else if(z51ffa25fd4[z27662fd564].type()==
typeid(long))return std::make_pair(fc::Type::LONG,fc::any_cast<long>(&
z51ffa25fd4[z27662fd564]));else if(z51ffa25fd4[z27662fd564].type()==typeid(char)
)return std::make_pair(fc::Type::CHAR,(fc::any_cast<char>(&z51ffa25fd4[
z27662fd564])));else throw std::runtime_error(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x67\x65\x74\x5f\x64\x61\x74\x61\x28\x29\x3a\x55\x6e\x6b\x6e\x6f\x77\x6e\x20\x74\x79\x70\x65"
);}void fc::FaceCodeDecoder_::print_data(){for(unsigned int zdae0797406=
(0x29c+4089-0x1295);zdae0797406<z51ffa25fd4.size();zdae0797406++){if(z51ffa25fd4
[zdae0797406].type()==typeid(int))std::cout<<fc::any_cast<int>(z51ffa25fd4[
zdae0797406])<<std::endl;else if(z51ffa25fd4[zdae0797406].type()==typeid(float))
std::cout<<fc::any_cast<float>(z51ffa25fd4[zdae0797406])<<std::endl;else if(
z51ffa25fd4[zdae0797406].type()==typeid(double))std::cout<<fc::any_cast<double>(
z51ffa25fd4[zdae0797406])<<std::endl;else if(z51ffa25fd4[zdae0797406].type()==
typeid(std::string))std::cout<<fc::any_cast<std::string>(z51ffa25fd4[zdae0797406
])<<std::endl;else if(z51ffa25fd4[zdae0797406].type()==typeid(long))std::cout<<
fc::any_cast<long>(z51ffa25fd4[zdae0797406])<<std::endl;else if(z51ffa25fd4[
zdae0797406].type()==typeid(char))std::cout<<fc::any_cast<char>(z51ffa25fd4[
zdae0797406])<<std::endl;}}fc::FaceCodeDecoder_::~FaceCodeDecoder_(){
ZSTD_freeDCtx(z68914b4d84);ZSTD_freeDDict(z47946b1a48);}
