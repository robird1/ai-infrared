
#include "rangeencoder.h"
#include <stdlib.h>
#include <string.h>
#include <iostream>
z1968b1832e::z1968b1832e(){zcfba936831=(unsigned char*)malloc(sizeof(unsigned 
char)*(0xbeb+7010-0x2365));z89d819d668=(0x45+3258-0xcff);z7e17da302f=
(0xde0+4405-0x1b2d);z30ab05f4ce=(0x1a07+3165-0x2664);z9c6625587b=z3a38a98b0d;
z1e555ded70=z667dad30e0;z02713e5fee=(0x983+2691-0x1406);z8701e2277d=
(0x3e0+5185-0x1821);}void z1968b1832e::encode(z37813a3193*z455b3eaada,unsigned 
int z6ae01fcc17){unsigned int z1100762b33;unsigned int z8bd883661f;unsigned int 
zbd207edc82,z851c7c1f54;unsigned int zb8bb802765=z455b3eaada->zb8bb802765;
z455b3eaada->z8245bb11f9(z6ae01fcc17,&z1100762b33,&z8bd883661f);zdd53d384d8();
zbd207edc82=z9c6625587b>>zb8bb802765;z851c7c1f54=zbd207edc82*z8bd883661f;
z30ab05f4ce+=z851c7c1f54;
#ifdef z58cc7bdac3
z9c6625587b=zbd207edc82*z1100762b33;
#else
if((z8bd883661f+z1100762b33)>>zb8bb802765){z9c6625587b-=z851c7c1f54;}else{
z9c6625587b=zbd207edc82*z1100762b33;}
#endif
z455b3eaada->zeb0c9f382f(z6ae01fcc17);}void z1968b1832e::encode(unsigned int 
z9c6625587b,unsigned int z6ae01fcc17){if(z9c6625587b>4194303){zf4c6440c90(
z6ae01fcc17&65535);z6ae01fcc17=z6ae01fcc17>>(0x17b+3709-0xfe8);z9c6625587b=
z9c6625587b>>(0x872+4388-0x1986);z9c6625587b++;}unsigned int zbd207edc82,
z851c7c1f54;zdd53d384d8();zbd207edc82=this->z9c6625587b/z9c6625587b;z851c7c1f54=
zbd207edc82*z6ae01fcc17;z30ab05f4ce+=z851c7c1f54;
#ifdef z58cc7bdac3
this->z9c6625587b=zbd207edc82;
#else
if(z6ae01fcc17+(0xd75+820-0x10a8)<z9c6625587b){this->z9c6625587b=zbd207edc82;}
else{this->z9c6625587b-=z851c7c1f54;}
#endif
}void z1968b1832e::zc660d85057(unsigned char zbdb9758852){unsigned int 
zbd207edc82,z851c7c1f54;zdd53d384d8();zbd207edc82=z9c6625587b>>
(0x1135+2031-0x191c);z851c7c1f54=zbd207edc82*(unsigned int)(zbdb9758852);
z30ab05f4ce+=z851c7c1f54;
#ifdef z58cc7bdac3
z9c6625587b=zbd207edc82;
#else
if(((unsigned int)(zbdb9758852)+(0xdd+4079-0x10cb))>>(0x1af9+2877-0x262e)){
z9c6625587b-=z851c7c1f54;}else{z9c6625587b=zbd207edc82;}
#endif
}void z1968b1832e::zf4c6440c90(unsigned short s){unsigned int zbd207edc82,
z851c7c1f54;zdd53d384d8();zbd207edc82=z9c6625587b>>(0x10a3+1465-0x164c);
z851c7c1f54=zbd207edc82*(unsigned int)(s);z30ab05f4ce+=z851c7c1f54;
#ifdef z58cc7bdac3
z9c6625587b=zbd207edc82;
#else
if(((unsigned int)(s)+(0xa8+1274-0x5a1))>>(0x1313+3187-0x1f76)){z9c6625587b-=
z851c7c1f54;}else{z9c6625587b=zbd207edc82;}
#endif
}void z1968b1832e::zc14539c46b(unsigned int zdae0797406){zf4c6440c90((unsigned 
short)(zdae0797406%65536));zf4c6440c90((unsigned short)(zdae0797406/65536));}
void z1968b1832e::z721a0f318e(float f){zc14539c46b(*((unsigned int*)(&f)));}
inline void z1968b1832e::zdd53d384d8(){while(z9c6625587b<=zd2276fea96){if(
z30ab05f4ce<(unsigned int)(0x9e5+1230-0xdb4)<<z1f94c1bd54){z7e876b65be(
z1e555ded70);for(;z02713e5fee;z02713e5fee--){z7e876b65be((0x11d7+3200-0x1d58));}
z1e555ded70=(unsigned char)(z30ab05f4ce>>z1f94c1bd54);}else if(z30ab05f4ce&
z3a38a98b0d){z7e876b65be(z1e555ded70+(0x20ab+797-0x23c7));for(;z02713e5fee;
z02713e5fee--){z7e876b65be((0x1beb+733-0x1ec8));}z1e555ded70=(unsigned char)(
z30ab05f4ce>>z1f94c1bd54);}else{z02713e5fee++;}z9c6625587b<<=
(0x1f58+1293-0x245d);z30ab05f4ce=(z30ab05f4ce<<(0xfb+1083-0x52e))&(z3a38a98b0d-
(0x1391+285-0x14ad));z8701e2277d++;}}unsigned int z1968b1832e::done(){unsigned 
int z851c7c1f54;zdd53d384d8();z8701e2277d+=(0x1954+925-0x1cec);if((z30ab05f4ce&(
zd2276fea96-(0x1aa7+559-0x1cd5)))<((z8701e2277d&0xffffffL)>>(0x2e4+3662-0x1131))
){z851c7c1f54=z30ab05f4ce>>z1f94c1bd54;}else{z851c7c1f54=(z30ab05f4ce>>
z1f94c1bd54)+(0x215+285-0x331);}if(z851c7c1f54>(0x9c9+1673-0xf53)){z7e876b65be(
z1e555ded70+(0x1416+1602-0x1a57));for(;z02713e5fee;z02713e5fee--){z7e876b65be(
(0x7d5+3294-0x14b3));}}else{z7e876b65be(z1e555ded70);for(;z02713e5fee;
z02713e5fee--){z7e876b65be((0xf5f+5667-0x2483));}}z7e876b65be(z851c7c1f54&
(0xb20+1405-0xf9e));return z8701e2277d;}z1968b1832e::~z1968b1832e(){if(
zcfba936831){free(zcfba936831);}}unsigned char*z1968b1832e::z0283856457(){return
 zcfba936831;}int z1968b1832e::z8d7d0695d7(){return z89d819d668;}long 
z1968b1832e::zd5fe172493(){return z8701e2277d*(0x1b48+2920-0x26a8);}int 
z1968b1832e::z26f4a07a77(){return z8701e2277d;}inline void z1968b1832e::
z7e876b65be(unsigned int zbdb9758852){if(zcfba936831){if(z89d819d668==
z7e17da302f){unsigned char*z56cab9975a=(unsigned char*)malloc(sizeof(unsigned 
char)*z7e17da302f*(0x170b+260-0x180d));memcpy(z56cab9975a,zcfba936831,sizeof(
unsigned char)*z7e17da302f);free(zcfba936831);zcfba936831=z56cab9975a;
z7e17da302f=z7e17da302f*(0x1a97+317-0x1bd2);}zcfba936831[z89d819d668++]=
zbdb9758852;}}
