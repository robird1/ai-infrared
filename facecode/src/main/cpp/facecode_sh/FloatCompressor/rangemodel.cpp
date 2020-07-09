
#include "rangemodel.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
z37813a3193::z37813a3193(unsigned int n,unsigned int*init,int compress,int 
z69a9aa5033,int zb8bb802765){this->n=n;this->z69a9aa5033=z69a9aa5033;this->
zb8bb802765=zb8bb802765;z821f10f1d4=(unsigned short*)malloc((n+
(0xa38+6880-0x2517))*sizeof(unsigned short));zb8debb7372=(unsigned short*)malloc
((n+(0x1014+1044-0x1427))*sizeof(unsigned short));if(zb8bb802765==
(0xd5+5328-0x1595)){z821f10f1d4[n]=65535;}else{z821f10f1d4[n]=((0xb2+228-0x195)
<<zb8bb802765);}z821f10f1d4[(0x14e6+4474-0x2660)]=(0x8d7+4518-0x1a7d);if(
compress){search=NULL;}else{z01223d7170=zb8bb802765-z4a974632b2;search=(unsigned
 short*)malloc((((0x1c20+1516-0x220b)<<z4a974632b2)+(0x50c+1410-0xa8d))*sizeof(
unsigned short));search[(0x7bd+3124-0x13f0)<<z4a974632b2]=n-(0xb94+1674-0x121d);
}reset(init);}z37813a3193::~z37813a3193(){free(z821f10f1d4);free(zb8debb7372);if
(search!=NULL){free(search);}}void z37813a3193::z1d55f52e46(){int zdae0797406,
zbdb9758852,z42aab8c9e1;if(z795ab6b03d){z8a4ff817b5++;left=z795ab6b03d;
z795ab6b03d=(0x83d+115-0x8b0);return;}if(z8f2577d8b1!=z69a9aa5033){z8f2577d8b1
<<=(0x597+2349-0xec3);if(z8f2577d8b1>z69a9aa5033){z8f2577d8b1=z69a9aa5033;}}
zbdb9758852=z42aab8c9e1=z821f10f1d4[n];for(zdae0797406=n-(0xc79+1162-0x1102);
zdae0797406;zdae0797406--){int z851c7c1f54=zb8debb7372[zdae0797406];zbdb9758852
-=z851c7c1f54;z821f10f1d4[zdae0797406]=zbdb9758852;z851c7c1f54=z851c7c1f54>>
(0x180+6852-0x1c43)|(0x1a6a+1378-0x1fcb);z42aab8c9e1-=z851c7c1f54;zb8debb7372[
zdae0797406]=z851c7c1f54;}if(zbdb9758852!=zb8debb7372[(0x8a4+3506-0x1656)]){
throw(std::runtime_error(
"\x46\x61\x63\x65\x43\x6f\x64\x65\x3a\x3a\x45\x72\x72\x6f\x72\x3a\x20\x43\x6f\x6d\x70\x72\x65\x73\x73\x69\x6f\x6e\x20\x72\x65\x73\x63\x61\x6c\x69\x6e\x67"
));}zb8debb7372[(0xdb5+1315-0x12d8)]=zb8debb7372[(0xe1c+4843-0x2107)]>>
(0x1068+5384-0x256f)|(0xa2a+3300-0x170d);z42aab8c9e1-=zb8debb7372[
(0x1526+3930-0x2480)];z8a4ff817b5=z42aab8c9e1/z8f2577d8b1;z795ab6b03d=
z42aab8c9e1%z8f2577d8b1;left=z8f2577d8b1-z795ab6b03d;if(search!=NULL){
zdae0797406=n;while(zdae0797406){int zfa64e9e672,end;end=(z821f10f1d4[
zdae0797406]-(0x24bd+517-0x26c1))>>z01223d7170;zdae0797406--;zfa64e9e672=
z821f10f1d4[zdae0797406]>>z01223d7170;while(zfa64e9e672<=end){search[zfa64e9e672
]=zdae0797406;zfa64e9e672++;}}}}void z37813a3193::reset(unsigned int*init){int 
zdae0797406,end,z3b6e0e5edc;z8f2577d8b1=n>>(0x34f+3584-0x114b)|
(0xdec+5075-0x21bd);z795ab6b03d=(0x1120+3775-0x1fdf);if(init==NULL){z3b6e0e5edc=
z821f10f1d4[n]/n;end=z821f10f1d4[n]%n;for(zdae0797406=(0x13bd+4200-0x2425);
zdae0797406<end;zdae0797406++){zb8debb7372[zdae0797406]=z3b6e0e5edc+
(0xd67+1007-0x1155);}for(;zdae0797406<n;zdae0797406++){zb8debb7372[zdae0797406]=
z3b6e0e5edc;}}else{int z7a0ef4bf64=(0x731+7349-0x23e6);for(zdae0797406=
(0x61c+6955-0x2147);zdae0797406<n;zdae0797406++){if(init[zdae0797406]){
z7a0ef4bf64++;}}z3b6e0e5edc=z821f10f1d4[n]/z7a0ef4bf64;end=z821f10f1d4[n]%
z7a0ef4bf64;for(zdae0797406=(0x837+2341-0x115c);zdae0797406<n;zdae0797406++){if(
init[zdae0797406]){if(end){zb8debb7372[zdae0797406]=z3b6e0e5edc+
(0x10c1+2176-0x1940);end--;}else{zb8debb7372[zdae0797406]=z3b6e0e5edc;}}else{
zb8debb7372[zdae0797406]=(0xec4+2553-0x18bd);}}}z1d55f52e46();}void z37813a3193
::z8245bb11f9(unsigned int z6ae01fcc17,unsigned int*z3ac2b996f2,unsigned int*
z770c211071){*z3ac2b996f2=z821f10f1d4[z6ae01fcc17+(0xb09+3231-0x17a7)]-(*
z770c211071=z821f10f1d4[z6ae01fcc17]);}unsigned int z37813a3193::z97836430af(
unsigned int z770c211071){unsigned int z416634f46a,z5f6897cc86;unsigned short*
z851c7c1f54;z851c7c1f54=search+(z770c211071>>z01223d7170);z416634f46a=*
z851c7c1f54;z5f6897cc86=*(z851c7c1f54+(0x71+1971-0x823))+(0x1d8b+737-0x206b);
while(z416634f46a+(0x1f6+2368-0xb35)<z5f6897cc86){int z1a3f3e3567=(z416634f46a+
z5f6897cc86)>>(0x2e9+3767-0x119f);if(z770c211071<z821f10f1d4[z1a3f3e3567]){
z5f6897cc86=z1a3f3e3567;}else{z416634f46a=z1a3f3e3567;}}return z416634f46a;}void
 z37813a3193::zeb0c9f382f(unsigned int z6ae01fcc17){if(left<=
(0x1340+4555-0x250b)){z1d55f52e46();}left--;zb8debb7372[z6ae01fcc17]+=
z8a4ff817b5;}
