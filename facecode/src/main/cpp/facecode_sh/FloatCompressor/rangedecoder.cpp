
#include <stdexcept>
#include "rangedecoder.h"
inline void zf3d47e2890::zdd53d384d8(){while(z9c6625587b<=zd2276fea96){
z30ab05f4ce=(z30ab05f4ce<<(0x4e5+3209-0x1166))|((z1e555ded70<<z6d555d9cf9)&
(0xb89+3313-0x177b));z1e555ded70=z6fc994109a();z30ab05f4ce|=z1e555ded70>>(
(0xc76+1725-0x132b)-z6d555d9cf9);z9c6625587b<<=(0x150c+2641-0x1f55);}}unsigned 
int zf3d47e2890::decode(z37813a3193*z455b3eaada){unsigned int z6ae01fcc17;
unsigned int z8bd883661f;unsigned int z1100762b33;unsigned int z851c7c1f54;
unsigned int zb8bb802765=z455b3eaada->zb8bb802765;zdd53d384d8();z02713e5fee=this
->z9c6625587b>>zb8bb802765;z8bd883661f=z30ab05f4ce/z02713e5fee;
#ifdef z58cc7bdac3
z8bd883661f=z8bd883661f;
#else
z8bd883661f=((z8bd883661f>>zb8bb802765)?((0x348+3599-0x1156)<<zb8bb802765)-
(0xf53+2361-0x188b):z8bd883661f);
#endif
z6ae01fcc17=z455b3eaada->z97836430af(z8bd883661f);z455b3eaada->z8245bb11f9(
z6ae01fcc17,&z1100762b33,&z8bd883661f);z851c7c1f54=z02713e5fee*z8bd883661f;
z30ab05f4ce-=z851c7c1f54;
#ifdef z58cc7bdac3
this->z9c6625587b=z02713e5fee*z1100762b33;
#else
if((z8bd883661f+z1100762b33)<(unsigned int)((0x36c+4885-0x1680)<<zb8bb802765)){
this->z9c6625587b=z02713e5fee*z1100762b33;}else{this->z9c6625587b-=z851c7c1f54;}
#endif
z455b3eaada->zeb0c9f382f(z6ae01fcc17);return z6ae01fcc17;}unsigned int 
zf3d47e2890::decode(unsigned int z9c6625587b){unsigned int z851c7c1f54;unsigned 
int z139d7e174e;if(z9c6625587b>4194303){z851c7c1f54=z65f8c61707();z9c6625587b=
z9c6625587b>>(0x3c5+3709-0x1232);z9c6625587b++;z139d7e174e=decode(z9c6625587b)<<
(0x10e4+5194-0x251e);return(z139d7e174e|z851c7c1f54);}zdd53d384d8();z02713e5fee=
this->z9c6625587b/z9c6625587b;z851c7c1f54=z30ab05f4ce/z02713e5fee;
#ifdef z58cc7bdac3
z851c7c1f54=z851c7c1f54;
#else
z851c7c1f54=(z851c7c1f54>=z9c6625587b?z9c6625587b-(0x1659+1199-0x1b07):
z851c7c1f54);
#endif
z139d7e174e=(z02713e5fee*z851c7c1f54);z30ab05f4ce-=z139d7e174e;
#ifdef z58cc7bdac3
this->z9c6625587b=z02713e5fee;
#else
if(z851c7c1f54+(0xdb7+3456-0x1b36)<z9c6625587b){this->z9c6625587b=z02713e5fee;}
else{this->z9c6625587b-=z139d7e174e;}
#endif
return z851c7c1f54;}unsigned char zf3d47e2890::z132240762d(){unsigned char 
z851c7c1f54=z2973f16608((0xe23+3825-0x1d0c));zeb0c9f382f((0x1380+1374-0x18dd),
z851c7c1f54,(unsigned int)(0x61+8572-0x21dc)<<(0xe2f+5865-0x2510));return 
z851c7c1f54;}unsigned short zf3d47e2890::z65f8c61707(){unsigned short 
z851c7c1f54=z2973f16608((0x142c+4085-0x2411));zeb0c9f382f((0xa3f+822-0xd74),
z851c7c1f54,(unsigned int)(0xfac+1366-0x1501)<<(0x889+2071-0x1090));return 
z851c7c1f54;}unsigned int zf3d47e2890::z4f170a03c9(){unsigned int zc8c077beea=
z65f8c61707();unsigned int z1a3cb6cf9c=z65f8c61707();return z1a3cb6cf9c*65536+
zc8c077beea;}float zf3d47e2890::z5950c0f7a6(){float f;*((unsigned int*)(&f))=
z4f170a03c9();return f;}void zf3d47e2890::done(){zdd53d384d8();}unsigned int 
zf3d47e2890::z2973f16608(unsigned int shift){unsigned int z851c7c1f54;
zdd53d384d8();z02713e5fee=z9c6625587b>>shift;z851c7c1f54=z30ab05f4ce/z02713e5fee
;
#ifdef z58cc7bdac3
return z851c7c1f54;
#else
return(z851c7c1f54>>shift?((unsigned int)(0x7a9+7189-0x23bd)<<shift)-
(0xbc7+3550-0x19a4):z851c7c1f54);
#endif
}void zf3d47e2890::zeb0c9f382f(unsigned int z3ac2b996f2,unsigned int z770c211071
,unsigned int z4a299d826b){unsigned int z851c7c1f54;z851c7c1f54=z02713e5fee*
z770c211071;z30ab05f4ce-=z851c7c1f54;
#ifdef z58cc7bdac3
this->z9c6625587b=z02713e5fee*z3ac2b996f2;
#else
if(z770c211071+z3ac2b996f2<z4a299d826b){this->z9c6625587b=z02713e5fee*
z3ac2b996f2;}else{this->z9c6625587b-=z851c7c1f54;}
#endif
}zf3d47e2890::zf3d47e2890(unsigned char*zcfba936831,int z89d819d668){this->
zcfba936831=zcfba936831;this->z89d819d668=z89d819d668;z26ff2ca2e2=
(0xac7+3397-0x180c);z1e555ded70=z6fc994109a();z30ab05f4ce=z1e555ded70>>(
(0x578+1385-0xad9)-z6d555d9cf9);z9c6625587b=(unsigned int)(0xb1b+991-0xef9)<<
z6d555d9cf9;}zf3d47e2890::~zf3d47e2890(){}inline unsigned int zf3d47e2890::
z6fc994109a(){int zbdb9758852;if(z26ff2ca2e2<z89d819d668){zbdb9758852=
zcfba936831[z26ff2ca2e2++];}else{zbdb9758852=EOF;}return zbdb9758852;}
