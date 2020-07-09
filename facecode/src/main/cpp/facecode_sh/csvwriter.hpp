
#ifndef z9ac4a75dc4
#define z9ac4a75dc4
#include <fstream>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include "any.hpp"
using namespace std;class z80f98819c4{public:z80f98819c4(){this->zeb6f10b574=
true;this->z02ca928f42="\x3b";this->zcdd0340de6=-(0x19d7+3299-0x26b9);this->
zd60b8431f7=(0x1ce6+700-0x1fa2);}z80f98819c4(int z6b2d168982){this->zeb6f10b574=
true;this->z02ca928f42="\x3b";this->zcdd0340de6=z6b2d168982;this->zd60b8431f7=
(0xc7b+3725-0x1b08);}z80f98819c4(string z02ca928f42){this->zeb6f10b574=true;this
->z02ca928f42=z02ca928f42;this->zcdd0340de6=-(0xa29+7378-0x26fa);this->
zd60b8431f7=(0x12d3+154-0x136d);}z80f98819c4(string z02ca928f42,int z6b2d168982)
{this->zeb6f10b574=true;this->z02ca928f42=z02ca928f42;this->zcdd0340de6=
z6b2d168982;this->zd60b8431f7=(0x1a4a+2976-0x25ea);cout<<this->z02ca928f42<<endl
;}z80f98819c4&z4312cac6ae(const char*str){return this->z4312cac6ae(string(str));
}z80f98819c4&z4312cac6ae(char*str){return this->z4312cac6ae(string(str));}
z80f98819c4&z4312cac6ae(string str){size_t z0dcc29f7da=str.find("\"",
(0xcfc+6349-0x25c9));bool z7490f553fe=z0dcc29f7da!=string::npos;while(
z0dcc29f7da!=string::npos){str.insert(z0dcc29f7da,"\"");z0dcc29f7da=str.find(
"\"",z0dcc29f7da+(0xc66+2901-0x17b9));}if(z7490f553fe){str="\""+str+"\"";}else 
if(str.find(this->z02ca928f42)!=string::npos){str="\""+str+"\"";}return this->
z4312cac6ae<string>(str);}template<typename T>z80f98819c4&z4312cac6ae(T str){if(
this->zcdd0340de6>-(0xd8+4081-0x10c8)){if(this->zd60b8431f7==this->zcdd0340de6){
this->z930f3751bf();}}if(zd60b8431f7>(0x1679+218-0x1753))this->z6c73fb9766<<this
->z02ca928f42;this->z6c73fb9766<<str;this->zd60b8431f7++;return*this;}template<
typename T>z80f98819c4&operator<<(const T&t){return this->z4312cac6ae(t);}
z80f98819c4&operator<<(fc::any const&z9954a0ec44){if(z9954a0ec44.type()==typeid(
int))return this->z4312cac6ae(fc::any_cast<int>(z9954a0ec44));else if(
z9954a0ec44.type()==typeid(float))return this->z4312cac6ae(fc::any_cast<float>(
z9954a0ec44));else if(z9954a0ec44.type()==typeid(double))return this->
z4312cac6ae(fc::any_cast<double>(z9954a0ec44));else if(z9954a0ec44.type()==
typeid(std::string))return this->z4312cac6ae(fc::any_cast<std::string>(
z9954a0ec44));else if(z9954a0ec44.type()==typeid(long))return this->z4312cac6ae(
fc::any_cast<long>(z9954a0ec44));else if(z9954a0ec44.type()==typeid(char))return
 this->z4312cac6ae(fc::any_cast<char>(z9954a0ec44));else if(z9954a0ec44.type()==
typeid(short))return this->z4312cac6ae(fc::any_cast<short>(z9954a0ec44));else 
throw std::runtime_error(
"\x45\x72\x72\x6f\x72\x3a\x57\x72\x69\x74\x65\x72\x20\x3c\x3c");}void operator+=
(z80f98819c4&z1785ec7c6f){this->z6c73fb9766<<endl<<z1785ec7c6f;}string 
zbb7731c9cc(){return z6c73fb9766.str();}friend ostream&operator<<(std::ostream&
os,z80f98819c4&z1785ec7c6f){return os<<z1785ec7c6f.zbb7731c9cc();}z80f98819c4&
z930f3751bf(){if(!this->zeb6f10b574||this->zcdd0340de6>-(0x381+1601-0x9c1)){
z6c73fb9766<<endl;}else{this->zeb6f10b574=false;}zd60b8431f7=(0x573+6709-0x1fa8)
;return*this;}bool z275a44fdb5(string z3740df047c){return z275a44fdb5(
z3740df047c,false);}bool z275a44fdb5(string z3740df047c,bool append){ofstream 
z8bb9d3d64d;if(append)z8bb9d3d64d.open(z3740df047c.c_str(),ios::out|ios::app);
else z8bb9d3d64d.open(z3740df047c.c_str(),ios::out|ios::trunc);if(!z8bb9d3d64d.
is_open())return false;if(append)z8bb9d3d64d<<endl;z8bb9d3d64d<<this->
zbb7731c9cc();z8bb9d3d64d.close();return z8bb9d3d64d.good();}void z5529d8efe6(
int z85098f9c20){this->zcdd0340de6=z85098f9c20;}void z5bb7b8868e(){this->
zcdd0340de6=-(0xb2d+5450-0x2076);}protected:bool zeb6f10b574;string z02ca928f42;
int zcdd0340de6;int zd60b8431f7;stringstream z6c73fb9766;};
#endif 

