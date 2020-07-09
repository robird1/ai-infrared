
#include "facecodeencoder_.h"
fc::FaceCodeEncoder_::FaceCodeEncoder_(){zbbb31ba957=fc::zb6abe21710::
z583359b2da((0xb1c+3151-0x1758));z7262f8b448=ZSTD_createCCtx();z4802f01a37=std::
unique_ptr<za848461805>(new za848461805());}void fc::FaceCodeEncoder_::init(
const fc::Version&z9954a0ec44,const fc::Device&z7dfd43b573,const std::vector<
float>&f,const bool zbdb9758852){std::vector<float>().swap(zba61b40ae7);std::
vector<fc::any>().swap(z51ffa25fd4);switch(z7dfd43b573){case fc::Device::UL100:{
z6d51c28ec3=((char)(0x986+6341-0x221a));break;}case fc::Device::iCatch4:{
z6d51c28ec3=((char)(0x136+7598-0x1eb2));break;}case fc::Device::RPI3BP:{
z6d51c28ec3=((char)(0x575+6008-0x1cba));break;}case fc::Device::ANY:{z6d51c28ec3
=((char)(0xc10+2881-0x1721));break;}default:throw(std::runtime_error(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x45\x72\x72\x6f\x72\x3a\x20\x46\x43\x20\x44\x65\x76\x69\x63\x65\x20\x6e\x6f\x74\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64"
));}switch(z9954a0ec44){case fc::Version::FC1a:{if(f.size()!=z8012218f33)throw(
std::runtime_error(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x45\x72\x72\x6f\x72\x3a\x20\x57\x72\x6f\x6e\x67\x20\x73\x69\x7a\x65\x20\x6f\x66\x20\x66\x6c\x6f\x61\x74\x20\x76\x65\x63\x74\x6f\x72"
));z8292b6ea34="\x31\x61";std::vector<float>z6b2dc32187;fc::zb6abe21710::
zdd53d384d8(f,zcaf2ec8b61,z412161793e,&z6b2dc32187);z4802f01a37->init();if(
zbdb9758852){for(int zdae0797406=(0x137+2302-0xa35);zdae0797406<z8012218f33;
zdae0797406++){z4802f01a37->compress(fc::zb6abe21710::z05fc9f353a(z6b2dc32187[
zdae0797406],(0xcf5+482-0xec2)));}}else{for(int zdae0797406=(0x1f77+808-0x229f);
zdae0797406<z8012218f33;zdae0797406++)z4802f01a37->compress(z6b2dc32187[
zdae0797406]);}z4802f01a37->z2244c89e7f();z6b300a2c67=z4802f01a37->zf534e62b4b()
;fc::zb6abe21710::z10a058d443(z6b2dc32187,zcaf2ec8b61,z412161793e,&zba61b40ae7);
}break;default:throw(std::runtime_error(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x45\x72\x72\x6f\x72\x3a\x20\x46\x43\x20\x56\x65\x72\x73\x69\x6f\x6e\x20\x6e\x6f\x74\x20\x73\x75\x70\x70\x6f\x72\x74\x65\x64"
));}}bool fc::FaceCodeEncoder_::set_public_RSA_encryption_key(const CryptoPP::
RSA::PublicKey&z4fbab6334e){if(z4fbab6334e.GetModulus().BitCount()!=
RSA_KEY_SIZE_BITS)return false;z33cc0b3ea4.AssignFrom(z4fbab6334e);return true;}
bool fc::FaceCodeEncoder_::set_private_RSA_signature_key(const CryptoPP::RSA::
PrivateKey&z0d1a80852c){if(z0d1a80852c.GetModulus().BitCount()!=
RSA_KEY_SIZE_BITS)return false;z04f47ebd64.AssignFrom(z0d1a80852c);return true;}
void fc::FaceCodeEncoder_::print_data(){for(unsigned int zdae0797406=
(0xc0b+689-0xebc);zdae0797406<z51ffa25fd4.size();zdae0797406++){if(z51ffa25fd4[
zdae0797406].type()==typeid(int))std::cout<<fc::any_cast<int>(z51ffa25fd4[
zdae0797406])<<std::endl;else if(z51ffa25fd4[zdae0797406].type()==typeid(float))
std::cout<<fc::any_cast<float>(z51ffa25fd4[zdae0797406])<<std::endl;else if(
z51ffa25fd4[zdae0797406].type()==typeid(double))std::cout<<fc::any_cast<double>(
z51ffa25fd4[zdae0797406])<<std::endl;else if(z51ffa25fd4[zdae0797406].type()==
typeid(std::string))std::cout<<fc::any_cast<std::string>(z51ffa25fd4[zdae0797406
])<<std::endl;else if(z51ffa25fd4[zdae0797406].type()==typeid(long))std::cout<<
fc::any_cast<long>(z51ffa25fd4[zdae0797406])<<std::endl;else if(z51ffa25fd4[
zdae0797406].type()==typeid(char))std::cout<<fc::any_cast<char>(z51ffa25fd4[
zdae0797406])<<std::endl;}}std::string fc::FaceCodeEncoder_::z632644358d(){
z80f98819c4 z1785ec7c6f;std::string z89d10be160=z6c1faf6f40();z1785ec7c6f<<
z89d10be160;for(unsigned int zdae0797406=(0x4b3+182-0x569);zdae0797406<
z51ffa25fd4.size();zdae0797406++){if(z51ffa25fd4[zdae0797406].type()==typeid(int
))z1785ec7c6f<<fc::zb6abe21710::zd97f9decbf<int>(fc::any_cast<int>(z51ffa25fd4[
zdae0797406]));else if(z51ffa25fd4[zdae0797406].type()==typeid(float))
z1785ec7c6f<<fc::zb6abe21710::zd97f9decbf<float>(fc::any_cast<float>(z51ffa25fd4
[zdae0797406]));else if(z51ffa25fd4[zdae0797406].type()==typeid(double))
z1785ec7c6f<<fc::zb6abe21710::zd97f9decbf<double>(fc::any_cast<double>(
z51ffa25fd4[zdae0797406]));else if(z51ffa25fd4[zdae0797406].type()==typeid(long)
)z1785ec7c6f<<fc::zb6abe21710::zd97f9decbf<long>(fc::any_cast<long>(z51ffa25fd4[
zdae0797406]));else if(z51ffa25fd4[zdae0797406].type()==typeid(char))z1785ec7c6f
<<fc::zb6abe21710::zd97f9decbf<char>(fc::any_cast<char>(z51ffa25fd4[zdae0797406]
));else if(z51ffa25fd4[zdae0797406].type()==typeid(std::string))z1785ec7c6f<<
z51ffa25fd4[zdae0797406];}return z1785ec7c6f.zbb7731c9cc();}std::string fc::
FaceCodeEncoder_::z6c1faf6f40(){char zbdb9758852[z51ffa25fd4.size()];for(
unsigned int zdae0797406=(0x15d7+3075-0x21da);zdae0797406<z51ffa25fd4.size();
zdae0797406++){if(z51ffa25fd4[zdae0797406].type()==typeid(int))zbdb9758852[
zdae0797406]=((char)(0x106c+388-0x118c));else if(z51ffa25fd4[zdae0797406].type()
==typeid(float))zbdb9758852[zdae0797406]=((char)(0xbc0+1547-0x1165));else if(
z51ffa25fd4[zdae0797406].type()==typeid(double))zbdb9758852[zdae0797406]=
((char)(0x3ad+5451-0x1889));else if(z51ffa25fd4[zdae0797406].type()==typeid(std
::string))zbdb9758852[zdae0797406]=((char)(0xb23+1705-0x1159));else if(
z51ffa25fd4[zdae0797406].type()==typeid(long))zbdb9758852[zdae0797406]=
((char)(0xded+2311-0x1688));else if(z51ffa25fd4[zdae0797406].type()==typeid(char
))zbdb9758852[zdae0797406]=((char)(0x300+3233-0xf3e));}return std::string(
zbdb9758852,z51ffa25fd4.size());}bool fc::FaceCodeEncoder_::encode(std::vector<
char>*ze147f31b6c,const fc::FaceCodeEncoder::MODE&zaee1dcfb86,const char 
zb04edbed70[(0x1648+4202-0x26b0)]){switch(zaee1dcfb86){case fc::FaceCodeEncoder
::MODE::RSA_ENCRYPTION:return fc::FaceCodeEncoder_::z826d3c3166(ze147f31b6c,
zb04edbed70);case fc::FaceCodeEncoder::MODE::RSA_NO:return fc::FaceCodeEncoder_
::encode(ze147f31b6c,zb04edbed70);case fc::FaceCodeEncoder::MODE::
RSA_ENCRYPTION_SIGNATURE:return fc::FaceCodeEncoder_::z41074818ec(ze147f31b6c,
zb04edbed70);case fc::FaceCodeEncoder::MODE::RSA_SIGNATURE:return fc::
FaceCodeEncoder_::z4ac0bfeea1(ze147f31b6c,zb04edbed70);}return false;}bool fc::
FaceCodeEncoder_::encode(std::vector<char>*ze147f31b6c,const char zb04edbed70[
(0x1501+2206-0x1d9d)]){std::string data,z601b82a028;if(!z8292b6ea34.compare(
"\x31\x61")){data=fc::zb6abe21710::zd97f9decbf<unsigned int>(z6b300a2c67.size())
+z6b300a2c67+z632644358d();if(!fc::zb6abe21710::zfbdd9b1d7e(data,zbbb31ba957,
z7262f8b448,&z601b82a028))return false;}else return false;std::string 
zc52f7d2a20=fc::za2341d21ce::za12655acbc(std::string(z8292b6ea34+z6d51c28ec3+
data));std::string z7dfd43b573=z8292b6ea34+z6d51c28ec3+
((char)(0x5f2+5277-0x1a2b))+zb04edbed70[(0xf76+5712-0x25c6)]+zb04edbed70[
(0xd1f+5044-0x20d2)]+zc52f7d2a20+fc::za2341d21ce::zbd0a8b580a(z601b82a028,
zc52f7d2a20);*ze147f31b6c=std::vector<char>(z7dfd43b573.begin(),z7dfd43b573.end(
));return true;}bool fc::FaceCodeEncoder_::z826d3c3166(std::vector<char>*
ze147f31b6c,const char*zb04edbed70){std::vector<char>z9954a0ec44;if(!encode(&
z9954a0ec44))return false;std::string zbe81101a21(z9954a0ec44.begin()+
z490fea18bc,z9954a0ec44.begin()+z490fea18bc+ze4756f313d);std::string message=std
::string(z9954a0ec44.begin()+z490fea18bc+ze4756f313d,z9954a0ec44.end());fc::
za2341d21ce::z92f64bfcf0 zb271657760;zb271657760=fc::za2341d21ce::zea1bd5af49(
zbe81101a21,message);std::string zca51509d66=fc::za2341d21ce::z4b285ff686(
z33cc0b3ea4,zbe81101a21);std::string z426825fb03=z8292b6ea34+z6d51c28ec3+
((char)(0xbd7+5190-0x1fbc))+zb04edbed70[(0x382+4667-0x15bd)]+zb04edbed70[
(0x1a97+328-0x1bde)]+zb271657760.zb03064e0ff+zca51509d66+zb271657760.zd70abecf74
;*ze147f31b6c=std::vector<char>(z426825fb03.begin(),z426825fb03.end());return 
true;}bool fc::FaceCodeEncoder_::z4ac0bfeea1(std::vector<char>*ze147f31b6c,const
 char zb04edbed70[(0x6c+1830-0x790)]){std::vector<char>z9954a0ec44;if(!encode(&
z9954a0ec44))return false;std::string zc52f7d2a20(z9954a0ec44.begin()+
z490fea18bc,z9954a0ec44.begin()+z490fea18bc+ze4756f313d);std::string z1b036b7299
=fc::za2341d21ce::z8c6b4183f9(z04f47ebd64,zc52f7d2a20);std::string message=std::
string(z9954a0ec44.begin()+z490fea18bc+ze4756f313d,z9954a0ec44.end());std::
string z7dfd43b573=z8292b6ea34+z6d51c28ec3+((char)(0x788+4501-0x18ba))+
zb04edbed70[(0xb5a+6847-0x2619)]+zb04edbed70[(0x978+4479-0x1af6)]+z1b036b7299+
zc52f7d2a20+message;*ze147f31b6c=std::vector<char>(z7dfd43b573.begin(),
z7dfd43b573.end());return true;}bool fc::FaceCodeEncoder_::z41074818ec(std::
vector<char>*ze147f31b6c,const char*zb04edbed70){std::vector<char>z9954a0ec44;if
(!z4ac0bfeea1(&z9954a0ec44))return false;std::string z1b036b7299(z9954a0ec44.
begin()+z490fea18bc,z9954a0ec44.begin()+z490fea18bc+z26e4df40b8);std::string 
zbe81101a21(z9954a0ec44.begin()+z490fea18bc+z26e4df40b8,z9954a0ec44.begin()+
z490fea18bc+z26e4df40b8+ze4756f313d);std::string message=std::string(z9954a0ec44
.begin()+z490fea18bc+z26e4df40b8+ze4756f313d,z9954a0ec44.end());fc::za2341d21ce
::z92f64bfcf0 zb271657760;zb271657760=fc::za2341d21ce::zea1bd5af49(zbe81101a21,
message);std::string zca51509d66=fc::za2341d21ce::z4b285ff686(z33cc0b3ea4,
zbe81101a21);std::string z7dfd43b573=z8292b6ea34+z6d51c28ec3+
((char)(0x1118+2736-0x1b66))+zb04edbed70[(0xcdc+1348-0x1220)]+zb04edbed70[
(0x747+4088-0x173e)]+z1b036b7299+zb271657760.zb03064e0ff+zca51509d66+zb271657760
.zd70abecf74;*ze147f31b6c=std::vector<char>(z7dfd43b573.begin(),z7dfd43b573.end(
));return true;}bool fc::FaceCodeEncoder_::update_encryption_key(const std::
vector<char>&ze147f31b6c,const CryptoPP::RSA::PrivateKey&zca5d41add1,const 
CryptoPP::RSA::PublicKey&zf9c74244eb,std::vector<char>*zcaa1a0b9c4,const char 
zb04edbed70[(0x841+5074-0x1c11)]){if(ze147f31b6c.size()<z4d2715c1d8+z126460a381+
z20bab0af5c+ze4756f313d+(0x15f+2071-0x972))return false;int z27662fd564=
z4d2715c1d8+z126460a381;if(ze147f31b6c[z27662fd564]==((char)(0xa1d+4876-0x1cc8))
||ze147f31b6c[z27662fd564]==((char)(0x198+6957-0x1c95))||ze147f31b6c[z27662fd564
]==((char)(0x1341+4015-0x22bc))){std::string zb03064e0ff(ze147f31b6c.begin()+
z490fea18bc,ze147f31b6c.begin()+z490fea18bc+zbda7cbaa43);std::string zc52f7d2a20
(ze147f31b6c.begin()+z490fea18bc+zbda7cbaa43,ze147f31b6c.begin()+z490fea18bc+
zbda7cbaa43+zaaa411a714);std::string zffd10f4903;if(!fc::za2341d21ce::
z665dc26c9f(zca5d41add1,zc52f7d2a20,&zffd10f4903))return false;std::string 
z465a4270b0=fc::za2341d21ce::z4b285ff686(zf9c74244eb,zffd10f4903);std::string 
z878675891b=std::string(ze147f31b6c.begin(),ze147f31b6c.begin()+z490fea18bc)+
zb03064e0ff+z465a4270b0+std::string(ze147f31b6c.begin()+z490fea18bc+zbda7cbaa43+
zaaa411a714,ze147f31b6c.end());*zcaa1a0b9c4=std::vector<char>(z878675891b.begin(
),z878675891b.end());}else if(ze147f31b6c[z27662fd564]==
((char)(0x241+1813-0x8f4))||ze147f31b6c[z27662fd564]==
((char)(0x16cf+1490-0x1c70))||ze147f31b6c[z27662fd564]==
((char)(0x662+463-0x7fc))){std::string zb67d437521(ze147f31b6c.begin()+
z490fea18bc,ze147f31b6c.begin()+z490fea18bc+z26e4df40b8+zbda7cbaa43);std::string
 zc52f7d2a20(ze147f31b6c.begin()+z490fea18bc+z26e4df40b8+zbda7cbaa43,ze147f31b6c
.begin()+z490fea18bc+z26e4df40b8+zbda7cbaa43+zaaa411a714);std::string 
zffd10f4903;if(!fc::za2341d21ce::z665dc26c9f(zca5d41add1,zc52f7d2a20,&
zffd10f4903))return false;std::string z465a4270b0=fc::za2341d21ce::z4b285ff686(
zf9c74244eb,zffd10f4903);std::string z878675891b=std::string(ze147f31b6c.begin()
,ze147f31b6c.begin()+z490fea18bc)+zb67d437521+z465a4270b0+std::string(
ze147f31b6c.begin()+z490fea18bc+z26e4df40b8+zbda7cbaa43+zaaa411a714,ze147f31b6c.
end());*zcaa1a0b9c4=std::vector<char>(z878675891b.begin(),z878675891b.end());}
else{return false;}if(zb04edbed70!=nullptr){zcaa1a0b9c4->at(z4d2715c1d8+
z126460a381+z83a89c5820)=zb04edbed70[(0x390+2497-0xd51)];zcaa1a0b9c4->at(
z4d2715c1d8+z126460a381+z83a89c5820+(0xa17+2490-0x13d0))=zb04edbed70[
(0x8b8+1516-0xea3)];}return true;}void fc::FaceCodeEncoder_::to_base91(const std
::vector<char>&ze147f31b6c,std::string*zba7e3b8b7b){int z27662fd564=z4d2715c1d8+
z126460a381;*zba7e3b8b7b=std::string(ze147f31b6c.begin(),ze147f31b6c.begin()+
z490fea18bc)+base<(0x3c0+1317-0x88a)>::encode(std::string(ze147f31b6c.begin()+
z490fea18bc,ze147f31b6c.end()));if(zba7e3b8b7b->at(z27662fd564)==
((char)(0x1353+4361-0x23fb)))zba7e3b8b7b->at(z27662fd564)=
((char)(0x393+965-0x728));else if(zba7e3b8b7b->at(z27662fd564)==
((char)(0x505+4076-0x148f)))zba7e3b8b7b->at(z27662fd564)=
((char)(0x82b+1963-0xfa5));else if(zba7e3b8b7b->at(z27662fd564)==
((char)(0x2d7+2733-0xd21)))zba7e3b8b7b->at(z27662fd564)=
((char)(0x9a2+5124-0x1d74));else if(zba7e3b8b7b->at(z27662fd564)==
((char)(0x1cb6+2260-0x2526)))zba7e3b8b7b->at(z27662fd564)=
((char)(0x104+9428-0x25a5));}void fc::FaceCodeEncoder_::to_base64(const std::
vector<char>&ze147f31b6c,std::string*zba7e3b8b7b){int z27662fd564=z4d2715c1d8+
z126460a381;*zba7e3b8b7b=std::string(ze147f31b6c.begin(),ze147f31b6c.begin()+
z490fea18bc)+base<(0x694+962-0xa16)>::encode(std::string(ze147f31b6c.begin()+
z490fea18bc,ze147f31b6c.end()));if(zba7e3b8b7b->at(z27662fd564)==
((char)(0x3b1+3299-0x1033)))zba7e3b8b7b->at(z27662fd564)=
((char)(0x639+5707-0x1c50));else if(zba7e3b8b7b->at(z27662fd564)==
((char)(0x2138+934-0x247c)))zba7e3b8b7b->at(z27662fd564)=
((char)(0x58c+4489-0x16e0));else if(zba7e3b8b7b->at(z27662fd564)==
((char)(0xa7d+5891-0x211d)))zba7e3b8b7b->at(z27662fd564)=
((char)(0x19eb+1866-0x20ff));else if(zba7e3b8b7b->at(z27662fd564)==
((char)(0x886+4708-0x1a86)))zba7e3b8b7b->at(z27662fd564)=
((char)(0x1a2+1938-0x8fd));}template<typename T>void fc::FaceCodeEncoder_::
push_data(T&a){z51ffa25fd4.push_back(a);}template void fc::FaceCodeEncoder_::
push_data<int>(int&T);template void fc::FaceCodeEncoder_::push_data<float>(float
&T);template void fc::FaceCodeEncoder_::push_data<double>(double&T);template 
void fc::FaceCodeEncoder_::push_data(long&T);template void fc::FaceCodeEncoder_
::push_data(std::string&T);template void fc::FaceCodeEncoder_::push_data(char&T)
;void ze6d06dfe4f(const std::string&data,const fc::FaceCodeEncoder::QR_EC&
z09c8d9f9b9,const int mask,const char&zbdb9758852,std::vector<unsigned char>*
z1b622d3039){std::vector<unsigned char>().swap(*z1b622d3039);zbaf424f1e4::
z3c5f83bd40::z590c984210 zbf3f6cd858;switch(z09c8d9f9b9){case fc::
FaceCodeEncoder::QR_EC::L:zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::
z1ef8edc2c5;break;case fc::FaceCodeEncoder::QR_EC::M:zbf3f6cd858=zbaf424f1e4::
z3c5f83bd40::z590c984210::z1b6b42fc3f;break;case fc::FaceCodeEncoder::QR_EC::Q:
zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::z19c3013ba1;break;case fc::
FaceCodeEncoder::QR_EC::H:zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::
z6434537aa3;break;default:zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::
z1ef8edc2c5;}std::vector<zbaf424f1e4::z1b5e623412>ze0e6a80a83=zbaf424f1e4::
z1b5e623412::z8500ffd8c2(data.c_str());zbaf424f1e4::z3c5f83bd40 z2a2c2aed28=
zbaf424f1e4::z3c5f83bd40::z7517157a9f(ze0e6a80a83,zbf3f6cd858,(0x6b+2793-0xb53),
(0xe45+1870-0x156b),mask,true);const int s=z2a2c2aed28.z21480ff6d2()>
(0xd84+6423-0x269b)?z2a2c2aed28.z21480ff6d2():(0x1297+1284-0x179a);for(int y=
(0x60b+3350-0x1321);y<s;y++)for(int x=(0x10b1+3639-0x1ee8);x<s;x++)z2a2c2aed28.
zec6734e8d5(x,y)!=(0x32d+5642-0x1937)?z1b622d3039->push_back(zbdb9758852):
z1b622d3039->push_back((0x3cf+8213-0x23e4));}void zdc77f2bcf4(const std::string&
data,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,const int mask,const int 
z4d042ae111,std::string*z9f2f90f313){zbaf424f1e4::z3c5f83bd40::z590c984210 
zbf3f6cd858;switch(z09c8d9f9b9){case fc::FaceCodeEncoder::QR_EC::L:zbf3f6cd858=
zbaf424f1e4::z3c5f83bd40::z590c984210::z1ef8edc2c5;break;case fc::
FaceCodeEncoder::QR_EC::M:zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::
z1b6b42fc3f;break;case fc::FaceCodeEncoder::QR_EC::Q:zbf3f6cd858=zbaf424f1e4::
z3c5f83bd40::z590c984210::z19c3013ba1;break;case fc::FaceCodeEncoder::QR_EC::H:
zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::z6434537aa3;break;default:
zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::z1ef8edc2c5;}std::vector<
zbaf424f1e4::z1b5e623412>ze0e6a80a83=zbaf424f1e4::z1b5e623412::z8500ffd8c2(data.
c_str());zbaf424f1e4::z3c5f83bd40 z2a2c2aed28=zbaf424f1e4::z3c5f83bd40::
z7517157a9f(ze0e6a80a83,zbf3f6cd858,(0x4c4+6818-0x1f65),(0x7e9+4507-0x195c),mask
,true);*z9f2f90f313=z2a2c2aed28.ze9ce39a19a(z4d042ae111);}void zb299e5654a(const
 std::vector<char>&data,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,const int 
mask,const char&zbdb9758852,std::vector<unsigned char>*z1b622d3039){std::vector<
unsigned char>().swap(*z1b622d3039);zbaf424f1e4::z3c5f83bd40::z590c984210 
zbf3f6cd858;switch(z09c8d9f9b9){case fc::FaceCodeEncoder::QR_EC::L:zbf3f6cd858=
zbaf424f1e4::z3c5f83bd40::z590c984210::z1ef8edc2c5;break;case fc::
FaceCodeEncoder::QR_EC::M:zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::
z1b6b42fc3f;break;case fc::FaceCodeEncoder::QR_EC::Q:zbf3f6cd858=zbaf424f1e4::
z3c5f83bd40::z590c984210::z19c3013ba1;break;case fc::FaceCodeEncoder::QR_EC::H:
zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::z6434537aa3;break;default:
zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::z1ef8edc2c5;}std::vector<
uint8_t>z2e2d75bb9f(data.begin(),data.end());std::vector<zbaf424f1e4::
z1b5e623412>ze0e6a80a83;ze0e6a80a83.push_back(zbaf424f1e4::z1b5e623412::
zc22c182213((0x6c0+4-0x6c3)));ze0e6a80a83.push_back(zbaf424f1e4::z1b5e623412::
z40b9d4bf17(z2e2d75bb9f));zbaf424f1e4::z3c5f83bd40 z2a2c2aed28=zbaf424f1e4::
z3c5f83bd40::z7517157a9f(ze0e6a80a83,zbf3f6cd858,(0x22d+1044-0x640),
(0x4e0+6538-0x1e42),mask,true);const int s=z2a2c2aed28.z21480ff6d2()>
(0x1a65+2390-0x23bb)?z2a2c2aed28.z21480ff6d2():(0xb2+643-0x334);for(int y=
(0x119d+2048-0x199d);y<s;y++)for(int x=(0x46b+3395-0x11ae);x<s;x++)z2a2c2aed28.
zec6734e8d5(x,y)!=(0x1413+2218-0x1cbd)?z1b622d3039->push_back(zbdb9758852):
z1b622d3039->push_back((0x1647+3765-0x24fc));}void zf0368ff64a(const std::vector
<char>&data,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,const int mask,const 
int z4d042ae111,std::string*z9f2f90f313){zbaf424f1e4::z3c5f83bd40::z590c984210 
zbf3f6cd858;switch(z09c8d9f9b9){case fc::FaceCodeEncoder::QR_EC::L:zbf3f6cd858=
zbaf424f1e4::z3c5f83bd40::z590c984210::z1ef8edc2c5;break;case fc::
FaceCodeEncoder::QR_EC::M:zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::
z1b6b42fc3f;break;case fc::FaceCodeEncoder::QR_EC::Q:zbf3f6cd858=zbaf424f1e4::
z3c5f83bd40::z590c984210::z19c3013ba1;break;case fc::FaceCodeEncoder::QR_EC::H:
zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::z6434537aa3;break;default:
zbf3f6cd858=zbaf424f1e4::z3c5f83bd40::z590c984210::z1ef8edc2c5;}std::vector<
uint8_t>z2e2d75bb9f(data.begin(),data.end());std::vector<zbaf424f1e4::
z1b5e623412>ze0e6a80a83;ze0e6a80a83.push_back(zbaf424f1e4::z1b5e623412::
zc22c182213((0x779+1292-0xc84)));ze0e6a80a83.push_back(zbaf424f1e4::z1b5e623412
::z40b9d4bf17(z2e2d75bb9f));zbaf424f1e4::z3c5f83bd40 z2a2c2aed28=zbaf424f1e4::
z3c5f83bd40::z7517157a9f(ze0e6a80a83,zbf3f6cd858,(0xa9a+4425-0x1be2),
(0x1248+5092-0x2604),mask,true);*z9f2f90f313=z2a2c2aed28.ze9ce39a19a(z4d042ae111
);}void fc::FaceCodeEncoder_::generate_QR_data(const std::vector<char>&
ze147f31b6c,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,const int mask,std::
vector<unsigned char>*z1b622d3039){zb299e5654a(ze147f31b6c,z09c8d9f9b9,mask,
(0x213+8396-0x22de),z1b622d3039);}void fc::FaceCodeEncoder_::generate_QR_data(
const std::string&zba7e3b8b7b,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,const
 int mask,std::vector<unsigned char>*z1b622d3039){ze6d06dfe4f(zba7e3b8b7b,
z09c8d9f9b9,mask,(0xf9d+4994-0x231e),z1b622d3039);}void fc::FaceCodeEncoder_::
generate_QR_svg(const std::vector<char>&ze147f31b6c,const fc::FaceCodeEncoder::
QR_EC&z09c8d9f9b9,const int mask,const int z4d042ae111,std::string*z9f2f90f313){
zf0368ff64a(ze147f31b6c,z09c8d9f9b9,mask,z4d042ae111,z9f2f90f313);}void fc::
FaceCodeEncoder_::generate_QR_svg(const std::string&zba7e3b8b7b,const fc::
FaceCodeEncoder::QR_EC&z09c8d9f9b9,const int mask,const int z4d042ae111,std::
string*z9f2f90f313){zdc77f2bcf4(zba7e3b8b7b,z09c8d9f9b9,mask,z4d042ae111,
z9f2f90f313);}void za57f44e790(std::vector<unsigned char>&za043798239,std::
vector<unsigned char>&zb75dd80783,std::vector<unsigned char>*z1b622d3039){if(
za043798239.size()==zb75dd80783.size()){std::transform(za043798239.begin(),
za043798239.end(),zb75dd80783.begin(),std::back_inserter(*z1b622d3039),std::plus
<unsigned char>());}else if(za043798239.size()>zb75dd80783.size()){std::vector<
unsigned char>z21fc38b1fe(za043798239.size(),(0x4bf+6991-0x200e));int 
z8183dbcc31=sqrt(zb75dd80783.size());int zfe83f70912=sqrt(za043798239.size());
for(int zdae0797406=(0x11f0+5243-0x266b);zdae0797406<z8183dbcc31;zdae0797406++)
std::copy(zb75dd80783.begin()+zdae0797406*z8183dbcc31,zb75dd80783.begin()+
zdae0797406*z8183dbcc31+z8183dbcc31,z21fc38b1fe.begin()+zdae0797406*zfe83f70912+
floor(abs(z8183dbcc31-zfe83f70912)/(0x615+8104-0x25bb))*(zfe83f70912+
(0xa13+5904-0x2122)));std::transform(za043798239.begin(),za043798239.end(),
z21fc38b1fe.begin(),std::back_inserter(*z1b622d3039),std::plus<unsigned char>())
;}else{std::vector<unsigned char>z21fc38b1fe(zb75dd80783.size(),
(0x1a3+7549-0x1f20));int z8183dbcc31=sqrt(zb75dd80783.size());int zfe83f70912=
sqrt(za043798239.size());for(int zdae0797406=(0x109d+1414-0x1623);zdae0797406<
zfe83f70912;zdae0797406++)std::copy(za043798239.begin()+zdae0797406*zfe83f70912,
za043798239.begin()+zdae0797406*zfe83f70912+zfe83f70912,z21fc38b1fe.begin()+
zdae0797406*z8183dbcc31+floor(abs(z8183dbcc31-zfe83f70912)/(0x4b1+968-0x877))*(
z8183dbcc31+(0x1862+560-0x1a91)));std::transform(zb75dd80783.begin(),zb75dd80783
.end(),z21fc38b1fe.begin(),std::back_inserter(*z1b622d3039),std::plus<unsigned 
char>());}}void fc::FaceCodeEncoder_::generate_QR_data_color(const std::vector<
char>&ze147f31b6c,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,std::vector<
unsigned char>*z1b622d3039){std::size_t const z8f422e5b64=ceil(ze147f31b6c.size(
)/(0x94a+3846-0x184e));std::vector<char>z2a7b88e3d1(ze147f31b6c.begin(),
ze147f31b6c.begin()+z8f422e5b64);std::vector<char>z4c560b0326(ze147f31b6c.begin(
)+z8f422e5b64,ze147f31b6c.end());std::vector<unsigned char>za043798239,
zb75dd80783;zb299e5654a(z2a7b88e3d1,z09c8d9f9b9,-(0x13c7+1955-0x1b69),
(0x89a+4576-0x1a79),&za043798239);zb299e5654a(z4c560b0326,z09c8d9f9b9,-
(0xcbc+5480-0x2223),(0x125b+1911-0x19d0),&zb75dd80783);za57f44e790(za043798239,
zb75dd80783,z1b622d3039);}void fc::FaceCodeEncoder_::generate_QR_data_color(
const std::string&zba7e3b8b7b,const fc::FaceCodeEncoder::QR_EC&z09c8d9f9b9,std::
vector<unsigned char>*z1b622d3039){std::string z2a7b88e3d1=zba7e3b8b7b.substr(
(0x48f+5292-0x193b),ceil(zba7e3b8b7b.size()/(0xc6b+1050-0x1083)));std::string 
z4c560b0326=zba7e3b8b7b.substr(ceil(zba7e3b8b7b.size()/(0x817+891-0xb90)));std::
vector<unsigned char>za043798239,zb75dd80783;ze6d06dfe4f(z2a7b88e3d1,z09c8d9f9b9
,-(0xee0+501-0x10d4),(0x125a+1359-0x17a8),&za043798239);ze6d06dfe4f(z4c560b0326,
z09c8d9f9b9,-(0x4c4+6605-0x1e90),(0x1522+2105-0x1d59),&zb75dd80783);za57f44e790(
za043798239,zb75dd80783,z1b622d3039);}std::string fc::FaceCodeEncoder_::
get_version()const{return z8292b6ea34;}std::string fc::FaceCodeEncoder_::
get_device()const{switch(z6d51c28ec3){case((char)(0x57+156-0xc3)):return
"\x41\x4e\x59";case((char)(0x1c08+394-0x1d61)):return"\x55\x4c\x31\x30\x30";case
((char)(0x10a4+2786-0x1b54)):return"\x69\x43\x61\x74\x63\x68\x34";case
((char)(0xb87+5267-0x1fe7)):return
"\x52\x61\x73\x70\x62\x65\x72\x72\x79\x20\x50\x69\x20\x33\x42\x2b";}return
"\x6e\x6f\x6e\x65";}std::vector<float>fc::FaceCodeEncoder_::get_code()const{
return zba61b40ae7;}int fc::FaceCodeEncoder_::get_data_size()const{return 
z51ffa25fd4.size();}std::pair<fc::Type,void*>fc::FaceCodeEncoder_::get_data(
const int&z27662fd564){if(z27662fd564<(0x7ec+7180-0x23f8)||z27662fd564>(int)
z51ffa25fd4.size())throw std::out_of_range(
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
);}fc::FaceCodeEncoder_::~FaceCodeEncoder_(){if(z7262f8b448)ZSTD_freeCCtx(
z7262f8b448);if(zbbb31ba957)ZSTD_freeCDict(zbbb31ba957);}
