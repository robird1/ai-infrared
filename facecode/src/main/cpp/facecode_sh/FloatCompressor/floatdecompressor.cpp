
#include "floatdecompressor.h"
#include <stdlib.h>
#include <iostream>
#define z1b8314b330 (0x11a8+4487-0x2323)
z2344c2fefe::z2344c2fefe(){z3fc6615718=(zf3d47e2890**)malloc(sizeof(zf3d47e2890*
)*(0x1aa0+2962-0x262f));}z2344c2fefe::~z2344c2fefe(){free(z3fc6615718);}void 
z2344c2fefe::init(const std::string&data){ze00cef6a81=(0x829+7631-0x25f8);int a=
int(((unsigned char)(data[(0x5db+3549-0x13b8)])<<(0x939+4985-0x1caa)|(unsigned 
char)(data[(0x506+4719-0x1774)])));int b=int(((unsigned char)(data[
(0x1d65+2013-0x2540)])<<(0xea+8295-0x2149)|(unsigned char)(data[
(0x1ca+4889-0x14e0)])));int zbdb9758852=int(((unsigned char)(data[
(0x3b1+8017-0x22fe)])<<(0x1de5+322-0x1f1f)|(unsigned char)(data[
(0xa09+4086-0x19fa)])));int z7dfd43b573=int(((unsigned char)(data[
(0x67d+3825-0x1568)])<<(0x3c6+2809-0xeb7)|(unsigned char)(data[(0xc19+40-0xc3a)]
)));z0536f9626e=new zf3d47e2890(reinterpret_cast<unsigned char*>(const_cast<char
*>(data.c_str()+(0x248+6078-0x19fe))),a);z3fc6615718[(0x220a+87-0x2261)]=new 
zf3d47e2890(reinterpret_cast<unsigned char*>(const_cast<char*>(data.c_str()+
(0x92b+1827-0x1046)+a)),b);z3fc6615718[(0xd89+4445-0x1ee5)]=new zf3d47e2890(
reinterpret_cast<unsigned char*>(const_cast<char*>(data.c_str()+
(0x1128+2352-0x1a50)+a+b)),zbdb9758852);z3fc6615718[(0xd2f+6160-0x253d)]=new 
zf3d47e2890(reinterpret_cast<unsigned char*>(const_cast<char*>(data.c_str()+
(0x1ac0+2937-0x2631)+a+b+zbdb9758852)),z7dfd43b573);zafbedb0124();}void 
z2344c2fefe::zafbedb0124(){int zdae0797406;zd53c2818c6=(z37813a3193**)malloc(
sizeof(z37813a3193*)*(0x22e9+1035-0x25f4));zb778bd1e7e=(z37813a3193**)malloc(
sizeof(z37813a3193*)*(0x11b5+3311-0x1da4));for(zdae0797406=(0xb65+3060-0x1759);
zdae0797406<(0x12ed+996-0x15d1);zdae0797406++){zd53c2818c6[zdae0797406]=nullptr;
zb778bd1e7e[zdae0797406]=nullptr;}z79607c34ba=nullptr;z98108322e8=(z37813a3193**
)malloc(sizeof(z37813a3193*)*(0x148+6831-0x1be0));for(zdae0797406=
(0xcb5+1254-0x119b);zdae0797406<(0x1d68+1178-0x21eb);zdae0797406++){z98108322e8[
zdae0797406]=nullptr;}}void z2344c2fefe::zb72f554e78(){int zdae0797406;for(
zdae0797406=(0x450+8770-0x2692);zdae0797406<(0x34f+3204-0xed3);zdae0797406++){if
(zd53c2818c6[zdae0797406])delete zd53c2818c6[zdae0797406];if(zb778bd1e7e[
zdae0797406])delete zb778bd1e7e[zdae0797406];}free(zd53c2818c6);free(zb778bd1e7e
);if(z79607c34ba)delete z79607c34ba;for(zdae0797406=(0x8df+3644-0x171b);
zdae0797406<(0xa5b+5952-0x2184);zdae0797406++){if(z98108322e8[zdae0797406])
delete z98108322e8[zdae0797406];}free(z98108322e8);}void z2344c2fefe::
z2244c89e7f(){z0536f9626e->done();z3fc6615718[(0xc4b+5191-0x2092)]->done();
z3fc6615718[(0x634+2552-0x102b)]->done();z3fc6615718[(0x81+1676-0x70b)]->done();
zb72f554e78();}
#define z2717e7f442    (0xd3f+2323-0x1652)
#define z9ac03b1bf4  (0x1299+2049-0x1a99)
#define zc482686bdc  (0xb1f+450-0xcdf)
#define z9e8dd13430  (0x831+1987-0xff1)
#define z50485f18f0     (0x996+4644-0x1bb6)
#define z2036b01a27   (0xbe8+3039-0x17c2)
#define z1e9dd20166   (0x6ef+7145-0x22d2)
#define ze2c2a589bc   (0x726+5524-0x1cb3)
#define zbfdd9b99b7    (0x1ead+1709-0x2552)
int z2344c2fefe::z766011cfe2(int zbf6821d4bb,zf3d47e2890*z3d0a817d22,z37813a3193
**z71c26d2918){if(z71c26d2918[zbf6821d4bb&(0xde7+2838-0x17fe)]==nullptr){
z71c26d2918[zbf6821d4bb&(0x794+3546-0x146f)]=new z37813a3193((0xffd+476-0x11d0),
nullptr,(0x1bd2+1098-0x201c),8192,(0x14df+1726-0x1b8d));}int z4867b8a0bd=
z3d0a817d22->decode(z71c26d2918[zbf6821d4bb&(0xa1a+2402-0x127d)]);if(z4867b8a0bd
==z50485f18f0){return zbf6821d4bb;}else{if((z9ac03b1bf4<=z4867b8a0bd)&&(
z4867b8a0bd<=ze2c2a589bc)){return zbf6821d4bb-(z50485f18f0-z4867b8a0bd);}else{if
(z79607c34ba==nullptr){z79607c34ba=new z37813a3193((0x1697+2063-0x1da7),nullptr,
(0x6af+5529-0x1c48),(0x21e7+5370-0x16e1),(0x8e7+5822-0x1f95));}if(z4867b8a0bd==
zbfdd9b99b7){return(zbf6821d4bb&(0x3e4+8917-0x25b9))|z3d0a817d22->decode(
z79607c34ba);}else{return((~zbf6821d4bb)&(0x1036+1457-0x14e7))|z3d0a817d22->
decode(z79607c34ba);}}}}int z2344c2fefe::z3cf5b4c2c5(int z6564c95102,int 
z43709a4f3c,zf3d47e2890**z2851c57324,z37813a3193**zd2f3f62252){int k,z0e11a40194
,zbdb9758852,z164f05b188;if(zd2f3f62252[z6564c95102]==nullptr){zd2f3f62252[
z6564c95102]=new z37813a3193((0xf89+4900-0x2296),nullptr,(0x97b+93-0x9d8),
(0x21cb+492-0x21b7),(0x1aab+404-0x1c2f));}k=z2851c57324[(0xd02+6640-0x26f2)]->
decode(zd2f3f62252[z6564c95102]);if(k<z1b8314b330){if(z98108322e8[k]==nullptr){
z98108322e8[k]=new z37813a3193(((0x10bd+2321-0x19cd)<<(k+(0x1911+2304-0x2210))),
nullptr,(0x18ad+1492-0x1e81),(0x2295+2551-0xc8c),(0xb06+1942-0x128c));}
zbdb9758852=z2851c57324[(0xb94+6181-0x23b8)]->decode(z98108322e8[k]);zbdb9758852
-=(((0xcba+6649-0x26b2)<<k)-(0xbba+1861-0x12fe));}else{z0e11a40194=k-z1b8314b330
+(0x54d+1910-0xcc2);if(z98108322e8[k]==nullptr){if(z98108322e8[z1b8314b330-
(0x1eb0+2003-0x2682)]==nullptr){z98108322e8[z1b8314b330-(0xfab+4844-0x2296)]=new
 z37813a3193((0x3c8+8287-0x2426)<<z1b8314b330,nullptr,(0x2b5+8042-0x221f),8192,
(0x1282+4002-0x2214));}z98108322e8[k]=new z37813a3193((0xfc+5878-0x17f1)<<
z0e11a40194,nullptr,(0xeb1+3884-0x1ddd),(0x2496+2105-0x24cf),
(0x1434+2203-0x1cbf));}zbdb9758852=z2851c57324[(0x7a5+5501-0x1d21)]->decode(
z98108322e8[z1b8314b330-(0x3c9+1932-0xb54)]);z164f05b188=z2851c57324[
(0x174b+989-0x1b26)]->decode(z98108322e8[k]);zbdb9758852-=(((0x148+6937-0x1c60)
<<(z1b8314b330-(0x117a+133-0x11fe)))-(0x2dc+8783-0x252a));if(zbdb9758852<
(0xa8c+2726-0x1532)){zbdb9758852=-(((-zbdb9758852)<<z0e11a40194)|z164f05b188);}
else{zbdb9758852=(zbdb9758852<<z0e11a40194)|z164f05b188;}}int za9d6095e8c=
zbdb9758852+z43709a4f3c;if(za9d6095e8c<(0x24b+9316-0x26af)){return za9d6095e8c+(
(0x15c3+2981-0x2167)<<(0x3ff+6626-0x1dca));}else if(za9d6095e8c>=(
(0x1e2d+1880-0x2584)<<(0xca4+4301-0x1d5a))){return za9d6095e8c-(
(0x480+2790-0xf65)<<(0x88b+1235-0xd47));}else{return za9d6095e8c;}}z00b5545762 
z2344c2fefe::z68d0ffc068(){z00b5545762 zb9bc57c8bb;z486d11ca6a z4867b8a0bd;
z486d11ca6a za9d6095e8c;z486d11ca6a zbf6821d4bb=(((z58c77ab927&)ze00cef6a81)&
4286578688)>>(0x5cd+8030-0x2514);z486d11ca6a z43709a4f3c=(((z58c77ab927&)
ze00cef6a81)&8388607);z4867b8a0bd=z766011cfe2(zbf6821d4bb,z0536f9626e,
zd53c2818c6);if(zbf6821d4bb==z4867b8a0bd){za9d6095e8c=z3cf5b4c2c5(z4867b8a0bd&
(0xdf7+1699-0x139b),z43709a4f3c,z3fc6615718,zb778bd1e7e);}else{za9d6095e8c=
z3cf5b4c2c5(z4867b8a0bd&(0xa8d+904-0xd16),(0x193+7415-0x1e8a),z3fc6615718,
zb778bd1e7e);}((z58c77ab927&)zb9bc57c8bb)=(z4867b8a0bd<<(0x836+739-0xb02))|
za9d6095e8c;ze00cef6a81=zb9bc57c8bb;return zb9bc57c8bb;}void z2344c2fefe::
z68d0ffc068(const int size,std::vector<float>*z9954a0ec44){z9954a0ec44->reserve(
size);for(int zdae0797406=(0x2cc+6491-0x1c27);zdae0797406<size;zdae0797406++)
z9954a0ec44->push_back(z68d0ffc068());}std::string z2344c2fefe::z4fd721283d(
const int size){unsigned char z4e3bc1720c[size*(0x1871+3610-0x2687)];for(int 
zdae0797406=(0x65+1178-0x4ff);zdae0797406<size;zdae0797406++){char f[
(0x110a+4119-0x211d)];float z6789126fc4=z68d0ffc068();memcpy(f,&z6789126fc4,
(0x768+5023-0x1b03));z4e3bc1720c[zdae0797406*(0x160b+1331-0x1b3a)]=f[
(0x3d1+1325-0x8fe)];z4e3bc1720c[zdae0797406*(0xd81+5640-0x2385)+
(0x8ca+4432-0x1a19)]=f[(0x5e9+1652-0xc5c)];z4e3bc1720c[zdae0797406*
(0xc2a+3416-0x197e)+(0x3d6+2659-0xe37)]=f[(0x143a+1573-0x1a5d)];z4e3bc1720c[
zdae0797406*(0xb67+1556-0x1177)+(0xa9a+1929-0x1220)]=f[(0xeb4+2445-0x183e)];}
return std::string(reinterpret_cast<const char*>(z4e3bc1720c),sizeof(z4e3bc1720c
));}
