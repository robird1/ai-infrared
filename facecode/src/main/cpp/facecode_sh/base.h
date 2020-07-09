
#pragma once
#include <string>
namespace{template<unsigned N>struct base{static std::string encode(const std::
string&binary);static std::string decode(const std::string&text);static bool 
encode(std::string&out,const std::string&binary);static bool decode(std::string&
out,const std::string&text);};typedef base<(0xa4a+4541-0x1bc7)>z51bcce0411;
typedef base<(0x235f+858-0x2664)>zbb52cbc145;typedef base<(0x45a+50-0x431)>
z299a2ced8b;template<>bool base<(0x6b8+7917-0x254a)>::encode(std::string&out,
const std::string&binary){const unsigned char zef7e9432d1[(0x1ac+9587-0x26c4)]={
((char)(0x14c2+4241-0x2512)),((char)(0x53a+7477-0x222d)),
((char)(0x5a7+1361-0xab5)),((char)(0x11f8+2436-0x1b38)),
((char)(0x1013+778-0x12d8)),((char)(0xbb3+1633-0x11ce)),
((char)(0x4b6+1995-0xc3a)),((char)(0x68f+1051-0xa62)),
((char)(0x33c+3881-0x121c)),((char)(0x1a3b+2334-0x230f)),
((char)(0xb0a+6026-0x2249)),((char)(0xfb9+5273-0x2406)),
((char)(0x1679+2233-0x1ee5)),((char)(0x5b5+1751-0xc3e)),
((char)(0x1541+3889-0x2423)),((char)(0x1ca+9351-0x2601)),
((char)(0x1030+2903-0x1b36)),((char)(0x19d1+3405-0x26cc)),
((char)(0x37b+3313-0x1019)),((char)(0x846+580-0xa36)),
((char)(0x8d8+7031-0x23fa)),((char)(0x1fcd+143-0x2006)),
((char)(0x247+3292-0xecc)),((char)(0x5a4+2477-0xef9)),((char)(0x29b+1234-0x714))
,((char)(0xab8+3466-0x17e8)),((char)(0x214+3547-0xf8e)),
((char)(0x8d5+2043-0x106e)),((char)(0x97+8153-0x200d)),
((char)(0x82f+2063-0xfda)),((char)(0x528+4657-0x16f4)),
((char)(0x4e3+6503-0x1de4)),((char)(0x11a2+1206-0x15f1)),
((char)(0xc6d+5385-0x210e)),((char)(0xf1d+725-0x1189)),
((char)(0x1cc2+533-0x1e6d)),((char)(0x1e0f+1961-0x254d)),
((char)(0x196b+1588-0x1f33)),((char)(0x1bb1+2016-0x2324)),
((char)(0x1bf5+669-0x1e24)),((char)(0x123f+3295-0x1eaf)),
((char)(0x657+3072-0x11e7)),((char)(0x618+3980-0x1533)),
((char)(0x90a+7667-0x268b)),((char)(0x16f6+430-0x1831)),
((char)(0x199d+1222-0x1def)),((char)(0x115b+1306-0x1600)),
((char)(0x7ca+5986-0x1eb6)),((char)(0x49f+6857-0x1ef1)),
((char)(0x3f9+3842-0x1283)),((char)(0x2326+970-0x2677)),
((char)(0x30c+827-0x5cd)),((char)(0x6a0+4949-0x19c5)),((char)(0x2ed+1664-0x93c))
,((char)(0x850+4758-0x1ab4)),((char)(0x165a+1465-0x1be0)),
((char)(0x8c0+6808-0x2324)),((char)(0x911+4023-0x1893)),
((char)(0x10a6+4826-0x234a)),((char)(0x1842+728-0x1ae3)),
((char)(0x1496+4416-0x259e)),((char)(0xfac+3637-0x1da8)),
((char)(0x1969+1792-0x2048)),((char)(0x3a6+725-0x658)),
((char)(0x1881+3279-0x252c)),((char)(0x11a5+3540-0x1f54)),
((char)(0xd41+1630-0x1379)),((char)(0x1eab+546-0x20a5)),
((char)(0x6fa+6859-0x219c)),((char)(0xbab+2387-0x14d4)),
((char)(0x2c1+4839-0x157d)),((char)(0x1058+5817-0x26e5)),
((char)(0x2703+5-0x26da)),((char)(0x1e1+4946-0x1504)),
((char)(0x4b2+8382-0x2536)),((char)(0x933+1773-0xfe5)),
((char)(0x7cd+5839-0x1e6f)),((char)(0x1c5+885-0x4fd)),'\\',
((char)(0x447+3385-0x1141)),((char)(0x1730+2694-0x2176)),
((char)(0xa3+402-0x1da)),((char)(0x6a2+2777-0x111e)),((char)(0x47a+2785-0xefd)),
((char)(0x5e8+7220-0x21bd)),((char)(0xdd4+4913-0x20a5)),
((char)(0x29c+3287-0xef8)),((char)(0xc74+2596-0x161c)),
((char)(0x138+8966-0x23c1)),((char)(0x1412+2556-0x1d90)),'\''};out.clear();const
 unsigned char*zd0baabdc72=(unsigned char*)binary.c_str();unsigned long 
z3ed798b443=(0xdb1+2509-0x177e);unsigned int zf96b1ad12e=(0x537+2369-0xe78);for(
size_t z006e4786fa=binary.size();z006e4786fa--;){z3ed798b443|=*zd0baabdc72++<<
zf96b1ad12e;zf96b1ad12e+=(0x1c3c+1912-0x23ac);if(zf96b1ad12e>(0xa24+7110-0x25dd)
){unsigned int val=z3ed798b443&8191;if(val>(0x6fa+6563-0x2045)){z3ed798b443>>=
(0x33b+1719-0x9e5);zf96b1ad12e-=(0x1a7f+1079-0x1ea9);}else{val=z3ed798b443&16383
;z3ed798b443>>=(0x808+2622-0x1238);zf96b1ad12e-=(0x1425+2450-0x1da9);}out.
push_back(zef7e9432d1[val%(0x1d5+8889-0x2433)]);out.push_back(zef7e9432d1[val/
(0x921+2107-0x1101)]);}}if(zf96b1ad12e){out.push_back(zef7e9432d1[z3ed798b443%
(0x730+125-0x752)]);if(zf96b1ad12e>(0xae4+1377-0x103e)||z3ed798b443>
(0xdb1+2769-0x1828))out.push_back(zef7e9432d1[z3ed798b443/(0xd6a+3509-0x1ac4)]);
}return true;}template<>bool base<(0x46a+224-0x4ef)>::decode(std::string&out,
const std::string&text){const unsigned char z1f0236681e[(0x11b6+920-0x144e)]={
(0xd62+1253-0x11ec),(0xcf4+5785-0x2332),(0x1e0+5841-0x1856),(0xf5d+2038-0x16f8),
(0x1cc7+2476-0x2618),(0x180a+3545-0x2588),(0xbff+4894-0x1ec2),(0xa26+357-0xb30),
(0x9d6+5757-0x1ff8),(0x13b4+556-0x1585),(0x1d95+2375-0x2681),(0x676+6111-0x1dfa)
,(0x4d1+1668-0xafa),(0x847+3288-0x14c4),(0x416+4894-0x16d9),(0x11a1+977-0x1517),
(0x632+882-0x949),(0x3a4+1177-0x7e2),(0x187b+1921-0x1fa1),(0x141b+4739-0x2643),
(0x5f3+8337-0x2629),(0x856+2847-0x131a),(0x62c+3916-0x151d),(0x744+1733-0xdae),
(0x201a+1871-0x270e),(0x16da+1575-0x1ca6),(0xb04+3283-0x177c),
(0x13a8+3479-0x20e4),(0x197b+3373-0x264d),(0x14a9+3666-0x22a0),
(0xa50+3451-0x1770),(0xdf+3487-0xe23),(0x27f+830-0x562),(0x6cd+3356-0x13ab),
(0x10ec+2835-0x1ba4),(0x20d5+991-0x2475),(0x868+5315-0x1ceb),(0x29c+2385-0xbac),
(0x1b31+719-0x1dbe),(0x883+7596-0x25d5),(0xe55+3953-0x1d83),(0xac8+1190-0xf2a),
(0x8c7+1987-0x1045),(0x5bb+4283-0x1630),(0x50c+4561-0x1696),(0x146b+1117-0x187c)
,(0x892+2066-0x105c),(0x1952+2863-0x2438),(0xfe4+5592-0x2588),
(0xfb8+2891-0x1ace),(0x93+2414-0x9cb),(0x2016+21-0x1ff4),(0x958+4265-0x19c9),
(0xd91+948-0x110c),(0x11b2+2803-0x1c6b),(0x12a3+4693-0x24bd),(0x221+4078-0x11d3)
,(0x84b+7109-0x23d3),(0x157a+3971-0x24b3),(0x801+7450-0x24d0),
(0x1e70+310-0x1f4b),(0x365+3044-0xefc),(0x8b0+7264-0x24b5),(0x1a80+112-0x1aa1),
(0x509+8057-0x2432),(0x8f5+1541-0xefa),(0x18cb+533-0x1adf),(0x15e1+3689-0x2448),
(0x1ae9+2761-0x25af),(0xbeb+2310-0x14ed),(0x178a+491-0x1970),(0x3fc+4709-0x165b)
,(0xf17+5702-0x2556),(0x860+7644-0x2634),(0xf73+2057-0x1773),(0x579+7309-0x21fc)
,(0xcef+2229-0x1599),(0x846+7594-0x25e4),(0x1b9+5573-0x1771),(0xcd7+5000-0x2051)
,(0x162c+3488-0x23bd),(0xeef+2725-0x1984),(0x16f1+2949-0x2265),
(0x1e8+6946-0x1cf8),(0xb83+7017-0x26d9),(0x1c5+3624-0xfd9),(0x71d+2303-0x1007),
(0xa3a+2200-0x12bc),(0x196+6440-0x1aa7),(0xc25+3883-0x1b38),(0xe12+18-0xe0b),
(0x934+230-0x9c9),(0x1c5d+1288-0x2117),(0x120c+454-0x1380),(0x16f3+326-0x17e6),
(0x8c1+3598-0x167b),(0xcf9+594-0xef6),(0x16a1+4154-0x26c1),(0x5d3+5652-0x1bcc),
(0x908+5334-0x1dc2),(0x26d+2943-0xdcf),(0xc6a+2105-0x1485),(0xda+8683-0x22a6),
(0xd76+5221-0x21bb),(0x106b+5308-0x2506),(0x168d+193-0x172c),(0x1d48+816-0x2055)
,(0x120f+3578-0x1fe5),(0x5d9+1521-0xba5),(0x1edd+1373-0x2414),
(0xb82+4616-0x1d63),(0xc6b+3873-0x1b64),(0x20e0+1198-0x2565),(0xbdc+3056-0x17a2)
,(0x851+6784-0x22a6),(0xe44+1589-0x144d),(0x44+6563-0x19ba),(0x18e1+1407-0x1e32)
,(0x11ef+5218-0x2622),(0x11b9+2226-0x1a3b),(0x3d3+1737-0xa6b),
(0x140d+1174-0x1871),(0x32f+6329-0x1bb5),(0x18d1+198-0x1941),(0x1696+717-0x190c)
,(0xb8c+4025-0x1aed),(0x10fc+5037-0x2450),(0x837+5811-0x1e8f),(0xb7d+1229-0xfef)
,(0x1705+975-0x1a79),(0x987+3815-0x1813),(0x18cb+3610-0x268a),
(0x1f5b+264-0x2008),(0xebc+5586-0x2433),(0x201c+507-0x21bc),(0x11ac+1704-0x17f9)
,(0xc59+2957-0x178b),(0x347+4278-0x13a2),(0x18bd+3398-0x25a8),(0x325+656-0x55a),
(0x1417+363-0x1527),(0x785+7670-0x2520),(0x148+191-0x1ac),(0x7b8+6524-0x20d9),
(0x246+4757-0x1480),(0x9e8+137-0xa16),(0x4f5+7742-0x22d8),(0xfbc+4789-0x2216),
(0xe4f+3618-0x1c16),(0x9a1+4868-0x1c4a),(0x1374+239-0x1408),(0x7f1+1030-0xb9c),
(0x16dd+137-0x170b),(0x93a+435-0xa92),(0x19d6+2353-0x22ac),(0x1444+3621-0x220e),
(0x1225+601-0x1423),(0x10e2+4254-0x2125),(0x2213+72-0x2200),(0x13b4+4682-0x25a3)
,(0x1279+2464-0x1bbe),(0xe7c+2308-0x1725),(0x4f8+5315-0x1960),
(0x1dc2+484-0x1f4b),(0x268+7721-0x2036),(0x39d+5578-0x190c),(0x9b9+6909-0x245b),
(0xa0b+4841-0x1c99),(0x420+4457-0x152e),(0xa12+411-0xb52),(0xda8+1734-0x1413),
(0xef9+697-0x1157),(0x152c+2471-0x1e78),(0x85d+3669-0x1657),(0x8d6+5727-0x1eda),
(0x5f3+5226-0x1a02),(0x20cf+1132-0x24e0),(0x1d9a+869-0x20a4),
(0x10c5+3773-0x1f27),(0x130c+1840-0x19e1),(0x15aa+1683-0x1be2),
(0xc25+1132-0x1036),(0x142+8463-0x21f6),(0x603+5923-0x1ccb),(0xf42+4480-0x2067),
(0x140d+2654-0x1e10),(0x45f+8452-0x2508),(0xcd1+2875-0x17b1),(0xf81+4451-0x2089)
,(0xd18+5257-0x2146),(0x960+4200-0x196d),(0xc06+917-0xf40),(0x914+3044-0x149d),
(0x222a+340-0x2323),(0x115d+867-0x1465),(0x1281+1597-0x1863),(0x47b+8887-0x26d7)
,(0x16ca+2285-0x1f5c),(0x20d2+1263-0x2566),(0x1ed3+164-0x1f1c),
(0x6fa+3533-0x146c),(0xa1f+1262-0xeb2),(0x1abf+546-0x1c86),(0xa9c+3265-0x1702),
(0xa9c+2442-0x13cb),(0xa43+6126-0x21d6),(0x1db+2231-0xa37),(0xdb6+2233-0x1614),
(0x12f0+4279-0x234c),(0x30d+4558-0x1480),(0x1926+3015-0x2492),
(0xbdc+5663-0x21a0),(0x216+7507-0x1f0e),(0x61f+1230-0xa92),(0x8d2+2534-0x125d),
(0x651+5772-0x1c82),(0x4ef+4952-0x17ec),(0xd0c+3197-0x192e),(0x2f2+8606-0x2435),
(0x1577+356-0x1680),(0xd85+2485-0x16df),(0x17fd+2795-0x228d),(0x24a+5323-0x16ba)
,(0xc05+0-0xbaa),(0xd14+2904-0x1811),(0x1cf+7115-0x1d3f),(0x1044+1099-0x1434),
(0x174d+2294-0x1fe8),(0x2303+196-0x236c),(0x6a+5111-0x1406),(0x287+3991-0x11c3),
(0x72f+2144-0xf34),(0x1259+3478-0x1f94),(0x1786+3678-0x2589),(0x44c+7658-0x21db)
,(0x1124+4520-0x2271),(0x3dd+1657-0x9fb),(0x83c+7633-0x25b2),(0x46f+1139-0x887),
(0x3cd+6945-0x1e93),(0x13bf+1359-0x18b3),(0x774+2497-0x10da),(0x21f+1501-0x7a1),
(0x1f3c+265-0x1fea),(0x1dfa+1817-0x24b8),(0xb92+3002-0x16f1),(0x476+2190-0xca9),
(0x321+5168-0x16f6),(0xd9a+3430-0x1aa5),(0x63b+5852-0x1cbc),(0x18e6+2610-0x22bd)
,(0x399+5423-0x186d),(0x7f7+5737-0x1e05),(0x2bc+6919-0x1d68),(0x286+7893-0x2100)
,(0x1ae7+3142-0x26d2)};out.clear();const unsigned char*zd0baabdc72=(unsigned 
char*)text.c_str();unsigned long z3ed798b443=(0xa5a+5081-0x1e33);unsigned int 
zf96b1ad12e=(0xe62+5535-0x2401);int val=-(0x67a+5555-0x1c2c);for(size_t 
z006e4786fa=text.size();z006e4786fa--;){unsigned int z7dfd43b573=z1f0236681e[*
zd0baabdc72++];if(z7dfd43b573==(0x2d0+6002-0x19e7))continue;if(val==-
(0xb3f+4165-0x1b83))val=z7dfd43b573;else{val+=z7dfd43b573*(0x72d+2601-0x10fb);
z3ed798b443|=val<<zf96b1ad12e;zf96b1ad12e+=(val&8191)>(0x883+5973-0x1f80)?
(0x3c9+8128-0x237c):(0xd61+1622-0x13a9);do{out.push_back(char(z3ed798b443));
z3ed798b443>>=(0x1823+1681-0x1eac);zf96b1ad12e-=(0x2042+1308-0x2556);}while(
zf96b1ad12e>(0x1425+4521-0x25c7));val=-(0x662+7615-0x2420);}}if(val!=-
(0xaf5+2951-0x167b))out.push_back(char(z3ed798b443|val<<zf96b1ad12e));return 
true;}inline bool z1f8366599f(std::string&out,const unsigned char*ze0282b3cc4,
size_t z0079b2cf4f){if(z0079b2cf4f%(0x1846+73-0x188b)){return false;}const char 
zdcad3a5975[(0x18c5+2174-0x20ed)]="\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39"
"\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a"
"\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74"
"\x75\x76\x77\x78\x79\x7a\x41\x42\x43\x44"
"\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e"
"\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58"
"\x59\x5a\x2e\x2d\x3a\x2b\x3d\x5e\x21\x2f"
"\x2a\x3f\x26\x3c\x3e\x28\x29\x5b\x5d\x7b""\x7d\x40\x25\x24\x23";out.resize(
z0079b2cf4f*(0xac5+6979-0x2603)/(0x18a9+1244-0x1d81));for(size_t zd52250c935=
(0x1fc1+1165-0x244e);zd52250c935<z0079b2cf4f*(0x15a1+2294-0x1e92)/
(0x11f6+2340-0x1b16);ze0282b3cc4+=(0xa7d+3747-0x191c)){unsigned value=(
ze0282b3cc4[(0x7ff+503-0x9f6)]<<(0x1465+4323-0x2530))|(ze0282b3cc4[
(0x1760+1907-0x1ed2)]<<(0xa61+4954-0x1dab))|(ze0282b3cc4[(0x1334+119-0x13a9)]<<
(0x509+366-0x66f))|ze0282b3cc4[(0x441+866-0x7a0)];out[zd52250c935++]=zdcad3a5975
[(value/52200625)%(0xee5+5692-0x24cc)];out[zd52250c935++]=zdcad3a5975[(value/
614125)%(0xabd+719-0xd37)];out[zd52250c935++]=zdcad3a5975[(value/7225)%
(0x11c2+5359-0x265c)];out[zd52250c935++]=zdcad3a5975[(value/(0x46f+6760-0x1e82))
%(0x91+4433-0x118d)];out[zd52250c935++]=zdcad3a5975[value%(0x447+7303-0x2079)];}
return true;}inline bool za40e0fd1ab(std::string&out,const unsigned char*
zb5c7fce85a,size_t ze36fdaab35){if(ze36fdaab35%(0x16bb+3987-0x2649)){return 
false;}const unsigned char zeb48e39678[(0x4fd+7323-0x2118)]={
(0x10e4+4953-0x243d),(0xcac+2555-0x16a7),(0x10f0+1615-0x173f),
(0x1b08+1962-0x22b2),(0x1711+963-0x1ad4),(0x13a8+3759-0x2257),
(0x1262+489-0x144b),(0x729+3219-0x13bc),(0x800+3041-0x13e1),(0x3d8+1303-0x8ef),
(0x1894+1788-0x1f90),(0xa7+9507-0x25ca),(0x23a+1378-0x79c),(0x1053+1412-0x15d7),
(0x1379+299-0x14a4),(0x65+1366-0x5bb),(0x18e8+48-0x1918),(0x321+6663-0x1d28),
(0x555+5401-0x1a6e),(0x1b92+1316-0x20b6),(0x878+1947-0x1013),(0x2e4+5658-0x18fe)
,(0xe5f+3296-0x1b3f),(0x2e3+634-0x55d),(0x160b+835-0x194e),(0x3d8+4395-0x1503),
(0xd25+1476-0x12e9),(0xd30+5486-0x229e),(0x7c+6692-0x1aa0),(0x170+8819-0x23e3),
(0xb34+4979-0x1ea7),(0xdd+9309-0x253a),(0x15b+5315-0x161e),(0x2a8+6457-0x1b9d),
(0x939+551-0xb60),(0xbb1+6921-0x2666),(0x17a9+3814-0x263c),(0x37f+5237-0x17a2),
(0xff0+5365-0x249d),(0x6af+6182-0x1ed5),(0x7d+119-0xa9),(0x26a+5770-0x18a8),
(0xd6d+5313-0x21e8),(0xb11+495-0xcbf),(0xb9c+2174-0x141a),(0xee9+4391-0x1fd1),
(0x151f+294-0x1607),(0x2ea+5758-0x1923),(0xec4+5025-0x2265),(0x2246+840-0x258d),
(0x7b2+6340-0x2074),(0x2a1+4800-0x155e),(0x28+9622-0x25ba),(0x96a+3132-0x15a1),
(0x549+190-0x601),(0x1d1a+1160-0x219b),(0x407+6888-0x1ee7),(0x60f+2719-0x10a5),
(0xc65+340-0xd79),(0x17c5+2554-0x21bf),(0x590+7688-0x234f),(0xea9+1943-0x15fe),
(0xec7+886-0x11f3),(0xed9+1994-0x165c),(0xf1+1997-0x86d),(0x10f+1342-0x629),
(0x1498+3829-0x2368),(0x177c+2460-0x20f2),(0x853+7012-0x2390),
(0x4a1+7588-0x221d),(0x1231+3744-0x20a8),(0x17c9+2719-0x223e),
(0x2119+335-0x223d),(0x109+1941-0x872),(0x9a0+49-0x9a4),(0x51+8011-0x1f6e),
(0x250+8790-0x2477),(0x1a2c+2382-0x234a),(0xae0+2399-0x140e),(0xe31+2069-0x1614)
,(0x874+7416-0x2539),(0x14b6+3626-0x22ac),(0x815+7145-0x23c9),
(0x20c2+1420-0x2618),(0x5bd+628-0x7fa),(0x450+8944-0x2708),(0x395+931-0x6ff),
(0x13ef+1591-0x19ec),(0x1590+4428-0x26a1),(0x10b2+238-0x1164),
(0x1e26+1830-0x250f),(0x2d4+5217-0x16e8),(0x157+5978-0x18b1),
(0x14d3+2175-0x1d04),(0xffa+4139-0x1fe2),(0x93+9805-0x26e0),(0xaf9+770-0xdfb),
(0xafb+509-0xcee),(0x336+1641-0x994),(0xe97+3383-0x1bc2),(0x4bd+4544-0x1670),
(0x21b5+1381-0x270c),(0x1ae8+2826-0x25e3),(0x43a+6216-0x1c72),(0x470+2663-0xec6)
,(0xcdc+3203-0x194d),(0xfb0+1287-0x14a4),(0x4b1+5303-0x1954),
(0x1b19+2956-0x2690),(0xc32+4297-0x1ce5),(0x402+8893-0x26a8),(0x41d+3863-0x131c)
,(0x1500+4503-0x267e),(0x2d7+1156-0x741),(0x3e6+4071-0x13b2),(0x7f8+5865-0x1ec5)
,(0x24c9+230-0x2592),(0x4d5+6696-0x1edf),(0x3e7+1984-0xb88),(0x843+4621-0x1a30),
(0x678+7398-0x233d),(0x516+7737-0x232d),(0x9cb+2850-0x14ca),(0x817+6226-0x201a),
(0x1890+3549-0x266d),(0xbd+689-0x31e),(0x1a51+2842-0x256b),(0x407+445-0x5c4),};
out.resize(ze36fdaab35*(0xca+4074-0x10b0)/(0x659+1838-0xd82));for(size_t 
zd52250c935=(0xcf1+5701-0x2336);zd52250c935<ze36fdaab35*(0xc55+5079-0x2028)/
(0x896+4749-0x1b1e);zb5c7fce85a+=(0x138+7606-0x1ee9)){unsigned value=zeb48e39678
[zb5c7fce85a[(0x541+6034-0x1cd3)]]*52200625+zeb48e39678[zb5c7fce85a[
(0x90c+5488-0x1e7b)]]*614125+zeb48e39678[zb5c7fce85a[(0xe47+5864-0x252d)]]*
(0x25e9+2370-0x12f2)+zeb48e39678[zb5c7fce85a[(0x1d2+4781-0x147c)]]*
(0x596+3726-0x13cf)+zeb48e39678[zb5c7fce85a[(0x97+7830-0x1f29)]];out[zd52250c935
++]=(value>>(0x82d+5320-0x1cdd))&(0xeb1+1748-0x1486);out[zd52250c935++]=(value>>
(0x1140+1572-0x1754))&(0x474+6986-0x1ebf);out[zd52250c935++]=(value>>
(0x15d0+668-0x1864))&(0x19cb+1774-0x1fba);out[zd52250c935++]=(value>>
(0x31+4295-0x10f8))&(0x263+2648-0xbbc);}return true;}template<>bool base<
(0x767+4146-0x1744)>::encode(std::string&out,const std::string&zbf5ea762f6){std
::string z363f519a84=std::string((const char*)&zbf5ea762f6[(0x46f+6009-0x1be8)],
zbf5ea762f6.size())+'\1'+std::string("\0\0\0\0",(0x1a62+874-0x1dc8)-(zbf5ea762f6
.size()+(0x1aaa+1065-0x1ed2))%(0x303+2393-0xc58));return z1f8366599f(out,(const 
unsigned char*)z363f519a84.c_str(),z363f519a84.size());}template<>bool base<
(0xd1d+450-0xe8a)>::decode(std::string&out,const std::string&zbd375644ca){if(!
za40e0fd1ab(out,(const unsigned char*)&zbd375644ca[(0x1e1+1014-0x5d7)],
zbd375644ca.size())){return false;}else{while(out.size()&&*out.rbegin()=='\0')
out.resize(out.size()-(0xa+7123-0x1bdc));if(out.size()&&*out.rbegin()=='\1')out.
resize(out.size()-(0x973+1824-0x1092));return true;}}template<>bool base<
(0xe75+5205-0x228a)>::encode(std::string&out,const std::string&text){const std::
string zcfba936831=
"\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a"
"\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a"
"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x2b\x2f";unsigned char const*
z4951c5b9ff=(unsigned char const*)text.c_str();unsigned int z531dba9dd3=(
unsigned int)text.size();unsigned int zdae0797406=(0xf3+7844-0x1f97);unsigned 
int z211d9af9f8=(0x644+4461-0x17b1);unsigned char z9d084370b8[
(0x1086+754-0x1375)];unsigned char z8c85bdbb51[(0x694+481-0x871)];out.clear();
while(z531dba9dd3--){z9d084370b8[zdae0797406++]=*(z4951c5b9ff++);if(zdae0797406
==(0x73b+990-0xb16)){z8c85bdbb51[(0xd28+6413-0x2635)]=(z9d084370b8[
(0xf00+3856-0x1e10)]&(0x60b+5409-0x1a30))>>(0xbf6+4736-0x1e74);z8c85bdbb51[
(0x1cb9+2302-0x25b6)]=((z9d084370b8[(0x151+6995-0x1ca4)]&(0x1a9c+138-0x1b23))<<
(0x14ad+3453-0x2226))+((z9d084370b8[(0x60d+2058-0xe16)]&(0xe4b+4799-0x201a))>>
(0xacd+1552-0x10d9));z8c85bdbb51[(0x194f+629-0x1bc2)]=((z9d084370b8[
(0xb55+6038-0x22ea)]&(0x1381+3154-0x1fc4))<<(0xe41+4435-0x1f92))+((z9d084370b8[
(0x75+7718-0x1e99)]&(0x16c+5987-0x180f))>>(0xec4+4978-0x2230));z8c85bdbb51[
(0x221d+271-0x2329)]=z9d084370b8[(0x115f+1453-0x170a)]&(0x257+1214-0x6d6);for(
zdae0797406=(0x869+4688-0x1ab9);(zdae0797406<(0x9b+632-0x30f));zdae0797406++)out
+=zcfba936831[z8c85bdbb51[zdae0797406]];zdae0797406=(0x1667+1182-0x1b05);}}if(
zdae0797406){for(z211d9af9f8=zdae0797406;z211d9af9f8<(0xec5+1081-0x12fb);
z211d9af9f8++)z9d084370b8[z211d9af9f8]='\0';z8c85bdbb51[(0x1031+5167-0x2460)]=(
z9d084370b8[(0xd0b+6130-0x24fd)]&(0xe7b+4830-0x205d))>>(0x12e0+4396-0x240a);
z8c85bdbb51[(0x136+3354-0xe4f)]=((z9d084370b8[(0x159+4572-0x1335)]&
(0x7b3+459-0x97b))<<(0x820+1328-0xd4c))+((z9d084370b8[(0x662+356-0x7c5)]&
(0xa9b+3838-0x18a9))>>(0x1a43+1886-0x219d));z8c85bdbb51[(0x7c2+2692-0x1244)]=((
z9d084370b8[(0x470+165-0x514)]&(0x7f9+2351-0x1119))<<(0x821+6195-0x2052))+((
z9d084370b8[(0x130d+491-0x14f6)]&(0x7b9+5552-0x1ca9))>>(0x1733+3103-0x234c));
z8c85bdbb51[(0x53f+7946-0x2446)]=z9d084370b8[(0x1207+519-0x140c)]&
(0xd4b+3448-0x1a84);for(z211d9af9f8=(0x13d7+4391-0x24fe);(z211d9af9f8<
zdae0797406+(0xbf+6676-0x1ad2));z211d9af9f8++)out+=zcfba936831[z8c85bdbb51[
z211d9af9f8]];while((zdae0797406++<(0x1dab+1685-0x243d)))out+=
((char)(0x8f7+73-0x903));}return true;}template<>bool base<(0x4f0+5338-0x198a)>
::decode(std::string&out,const std::string&zee23aa4c23){const std::string 
zcfba936831=
"\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a"
"\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a"
"\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x2b\x2f";unsigned int z531dba9dd3=(
unsigned int)zee23aa4c23.size();unsigned int zdae0797406=(0x8d7+4156-0x1913);
unsigned int z211d9af9f8=(0x11c4+4707-0x2427);unsigned int z9be4561871=
(0x5bf+7132-0x219b);unsigned char z8c85bdbb51[(0x245a+359-0x25bd)],z9d084370b8[
(0x1754+4023-0x2708)];out.clear();while(z531dba9dd3--&&(zee23aa4c23[z9be4561871]
!=((char)(0xc75+188-0xcf4)))&&(isalnum(zee23aa4c23[z9be4561871])||zee23aa4c23[
z9be4561871]==((char)(0x18c0+3135-0x24d4))||zee23aa4c23[z9be4561871]==
((char)(0x21c9+900-0x251e)))){z8c85bdbb51[zdae0797406++]=zee23aa4c23[z9be4561871
];z9be4561871++;if(zdae0797406==(0xcac+5387-0x21b3)){for(zdae0797406=
(0x10d8+1552-0x16e8);zdae0797406<(0x2169+852-0x24b9);zdae0797406++)z8c85bdbb51[
zdae0797406]=zcfba936831.find(z8c85bdbb51[zdae0797406]);z9d084370b8[
(0x1c+2315-0x927)]=(z8c85bdbb51[(0x3ea+3063-0xfe1)]<<(0x676+4064-0x1654))+((
z8c85bdbb51[(0x7b8+2251-0x1082)]&(0x1811+3665-0x2632))>>(0x105d+2116-0x189d));
z9d084370b8[(0x4a0+1959-0xc46)]=((z8c85bdbb51[(0x192d+1201-0x1ddd)]&
(0x727+4675-0x195b))<<(0x404+279-0x517))+((z8c85bdbb51[(0xf4a+2828-0x1a54)]&
(0x4f7+1790-0xbb9))>>(0x1579+442-0x1731));z9d084370b8[(0x42b+3092-0x103d)]=((
z8c85bdbb51[(0x430+2001-0xbff)]&(0x1022+5010-0x23b1))<<(0xf3+9119-0x248c))+
z8c85bdbb51[(0x5b1+1226-0xa78)];for(zdae0797406=(0xfb+1744-0x7cb);(zdae0797406<
(0xe0f+2571-0x1817));zdae0797406++)out+=z9d084370b8[zdae0797406];zdae0797406=
(0x434+7459-0x2157);}}if(zdae0797406){for(z211d9af9f8=zdae0797406;z211d9af9f8<
(0x30d+3932-0x1265);z211d9af9f8++)z8c85bdbb51[z211d9af9f8]=(0x1834+655-0x1ac3);
for(z211d9af9f8=(0x324+8396-0x23f0);z211d9af9f8<(0xb02+4267-0x1ba9);z211d9af9f8
++)z8c85bdbb51[z211d9af9f8]=zcfba936831.find(z8c85bdbb51[z211d9af9f8]);
z9d084370b8[(0x580+7675-0x237b)]=(z8c85bdbb51[(0x1d4+3605-0xfe9)]<<
(0xa91+1309-0xfac))+((z8c85bdbb51[(0x1812+2211-0x20b4)]&(0x159d+3719-0x23f4))>>
(0xf48+3546-0x1d1e));z9d084370b8[(0x3f6+4567-0x15cc)]=((z8c85bdbb51[
(0x619+1126-0xa7e)]&(0x38c+9016-0x26b5))<<(0x235c+262-0x245e))+((z8c85bdbb51[
(0x1083+207-0x1150)]&(0x1f1a+1527-0x24d5))>>(0x1f60+1460-0x2512));z9d084370b8[
(0x736+1279-0xc33)]=((z8c85bdbb51[(0xab6+1938-0x1246)]&(0x9b0+6158-0x21bb))<<
(0x15d+2768-0xc27))+z8c85bdbb51[(0x760+63-0x79c)];for(z211d9af9f8=
(0x5e0+7807-0x245f);(z211d9af9f8<zdae0797406-(0xdcc+2748-0x1887));z211d9af9f8++)
out+=z9d084370b8[z211d9af9f8];}return true;}template<>std::string base<
(0x373+8633-0x24ec)>::encode(const std::string&binary){std::string out;return 
base<(0x96c+3806-0x180a)>::encode(out,binary)?out:std::string();}template<>std::
string base<(0x1830+1347-0x1d33)>::decode(const std::string&text){std::string 
out;return base<(0x646+2780-0x10e2)>::decode(out,text)?out:std::string();}
template<>std::string base<(0x7f2+4778-0x1a47)>::encode(const std::string&binary
){std::string out;return base<(0x2c3+5027-0x1611)>::encode(out,binary)?out:std::
string();}template<>std::string base<(0x748+6639-0x20e2)>::decode(const std::
string&text){std::string out;return base<(0x58b+6260-0x1daa)>::decode(out,text)?
out:std::string();}template<>std::string base<(0xa5d+5357-0x1eef)>::encode(const
 std::string&binary){std::string out;return base<(0xea3+3354-0x1b62)>::encode(
out,binary)?out:std::string();}template<>std::string base<(0xb32+5706-0x2121)>::
decode(const std::string&text){std::string out;return base<(0x10b2+2374-0x199d)>
::decode(out,text)?out:std::string();}}
#ifdef zff1105f98d
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define zae0afa5725(...) if(printf("\x2d\x2d\x2d\x2d\x2d\x2d\x20" __VA_ARGS__),\
puts(""),true)
#define z5ba78c48b0(...)  (errno=(0x5b7+1527-0xbae),++z41867ba6a6,z02d0b70ef5+=!\
(ok=!!(__VA_ARGS__))),printf(\
"\x5b\x25\x73\x5d\x20\x25\x64\x20\x25\x73\x20\x28\x25\x73\x29" "\n",ok?\
"\x20\x4f\x4b\x20":"\x46\x41\x49\x4c",__LINE__,#__VA_ARGS__,strerror(errno))
unsigned z41867ba6a6=(0x669+1896-0xdd1),z02d0b70ef5=(0x105a+1371-0x15b5),ok=
atexit([]{zae0afa5725("\x73\x75\x6d\x6d\x61\x72\x79"){printf(
"\x5b\x25\x73\x5d\x20\x25\x64\x20\x74\x65\x73\x74\x73\x20\x3d\x20\x25\x64\x20\x70\x61\x73\x73\x65\x64\x20\x2b\x20\x25\x64\x20\x65\x72\x72\x6f\x72\x73" "\n"
,z02d0b70ef5?"\x46\x41\x49\x4c":"\x20\x4f\x4b\x20",z41867ba6a6,z41867ba6a6-
z02d0b70ef5,z02d0b70ef5);}});
#include <cassert>
#include <iostream>
void z666082a493(const std::string&binary){bool z29a76792ef=[&binary]{for(const 
unsigned char&z15040abd5b:binary)if(z15040abd5b>(0x20e6+219-0x2142))return
(0x11ad+4091-0x21a8);return(0x116a+651-0x13f4);}();size_t z2241155e3e=
(0xe33+2231-0x16ea),z6ab5ecf29a=(0x154+3924-0x10a8),z942c8e88e6=
(0x5cc+3700-0x1440),z106c03427a=binary.size();{std::string z6277004ac0=
z51bcce0411::encode(binary);std::string ze065b09a62=z51bcce0411::decode(
z6277004ac0);z2241155e3e=z6277004ac0.size();z5ba78c48b0(ze065b09a62==binary);}{
std::string z633e91a2b2=zbb52cbc145::encode(binary);std::string z805991c71b=
zbb52cbc145::decode(z633e91a2b2);z6ab5ecf29a=z633e91a2b2.size();z5ba78c48b0(
z805991c71b==binary);z5ba78c48b0(z633e91a2b2.find_first_of(
((char)(0x78f+3091-0x1380)))==std::string::npos);z5ba78c48b0(z633e91a2b2.
find_first_of('\\')==std::string::npos);}{std::string zc7dfdfe198=z299a2ced8b::
encode(binary);std::string z12d1c16050=z299a2ced8b::decode(zc7dfdfe198);
z942c8e88e6=zc7dfdfe198.size();z5ba78c48b0(z12d1c16050==binary);z5ba78c48b0(
zc7dfdfe198.find_first_of(((char)(0x243+1725-0x8c4)))==std::string::npos);
z5ba78c48b0(zc7dfdfe198.find_first_of(((char)(0x2c1+1325-0x7b0)))==std::string::
npos);z5ba78c48b0(zc7dfdfe198.find_first_of(((char)(0x2d6+5467-0x180f)))==std::
string::npos);std::string z1eb3ad92de="\x20" "\r\n\f\t\v\n"+zc7dfdfe198+
"\x20" "\r\n\f\t\v\n";z5ba78c48b0(binary==z299a2ced8b::decode(z1eb3ad92de));std
::string z70bc39fa92=zc7dfdfe198;z70bc39fa92.insert(z70bc39fa92.size()/
(0x269+3559-0x104e),"\x20" "\r\n\f\t\v\n");z5ba78c48b0(binary==z299a2ced8b::
decode(z70bc39fa92));std::string z725ed990ad=zc7dfdfe198.substr(
(0xbd1+4056-0x1ba9),zc7dfdfe198.size()/(0x83d+5944-0x1f73))+
"\r\n\r\n\t\t" "\x20\x20"+zc7dfdfe198.substr(zc7dfdfe198.size()/
(0x217+5781-0x18aa));z5ba78c48b0(binary==z299a2ced8b::decode(z725ed990ad));}
z5ba78c48b0(z106c03427a<=z942c8e88e6);z5ba78c48b0(z942c8e88e6<=z6ab5ecf29a);
z5ba78c48b0(z6ab5ecf29a-(0x640+6473-0x1f85)<=z2241155e3e);auto zccceff81bf=[&
z106c03427a](size_t z741d1b3ca1)->size_t{return((z741d1b3ca1*(0xfcb+558-0x1195)/
z106c03427a)-(0x4bf+1500-0xa37));};std::cout<<
"\n" "\x72\x65\x73\x75\x6c\x74\x73\x3a\x20"<<(z29a76792ef?std::string()+'\"'+
binary+'\"':"\x28\x68\x69\x64\x64\x65\x6e\x20\x74\x65\x78\x74\x29")<<'\n';std::
cout<<"\t" "\x62\x69\x6e\x61\x72\x79\x3a\x20"<<zccceff81bf(z106c03427a)<<
"\x25\x20\x6f\x76\x65\x72\x68\x65\x61\x64\x20\x28\x74\x6f\x74\x61\x6c\x3a\x20"<<
z106c03427a<<"\x20\x62\x79\x74\x65\x73\x29" "\n";std::cout<<
"\t" "\x62\x61\x73\x65\x36\x34\x3a\x20"<<zccceff81bf(z2241155e3e)<<
"\x25\x20\x6f\x76\x65\x72\x68\x65\x61\x64\x20\x28\x74\x6f\x74\x61\x6c\x3a\x20"<<
z2241155e3e<<"\x20\x62\x79\x74\x65\x73\x29" "\n";std::cout<<
"\t" "\x62\x61\x73\x65\x38\x35\x3a\x20"<<zccceff81bf(z6ab5ecf29a)<<
"\x25\x20\x6f\x76\x65\x72\x68\x65\x61\x64\x20\x28\x74\x6f\x74\x61\x6c\x3a\x20"<<
z6ab5ecf29a<<"\x20\x62\x79\x74\x65\x73\x29" "\n";std::cout<<
"\t" "\x62\x61\x73\x65\x39\x31\x3a\x20"<<zccceff81bf(z942c8e88e6)<<
"\x25\x20\x6f\x76\x65\x72\x68\x65\x61\x64\x20\x28\x74\x6f\x74\x61\x6c\x3a\x20"<<
z942c8e88e6<<"\x20\x62\x79\x74\x65\x73\x29" "\n\n";}void z11536067dc(const std::
string&binary){{std::string z6277004ac0,ze065b09a62;z5ba78c48b0(z51bcce0411::
encode(z6277004ac0,binary));z5ba78c48b0(z51bcce0411::decode(ze065b09a62,
z6277004ac0));z5ba78c48b0(ze065b09a62==binary);}{std::string z633e91a2b2,
z805991c71b;z5ba78c48b0(zbb52cbc145::encode(z633e91a2b2,binary));z5ba78c48b0(
zbb52cbc145::decode(z805991c71b,z633e91a2b2));z5ba78c48b0(z805991c71b==binary);}
{std::string zc7dfdfe198,z12d1c16050;z5ba78c48b0(z299a2ced8b::encode(zc7dfdfe198
,binary));z5ba78c48b0(z299a2ced8b::decode(z12d1c16050,zc7dfdfe198));z5ba78c48b0(
z12d1c16050==binary);}}int main(){std::string zc8840a3b1a=z51bcce0411::encode(
"\x48\x65\x6c\x6c\x6f\x20\x77\x6f\x72\x6c\x64\x20\x66\x72\x6f\x6d\x20\x42\x41\x53\x45\x36\x34\x21\x20" "\x1"
);std::string zf70e723a20=z51bcce0411::zbc7b5dd088(zc8840a3b1a);std::cout<<
zf70e723a20<<"\x20\x3c\x2d\x3e\x20"<<zc8840a3b1a<<std::endl;std::string 
z44e6103122=zbb52cbc145::encode(
"\x48\x65\x6c\x6c\x6f\x20\x77\x6f\x72\x6c\x64\x20\x66\x72\x6f\x6d\x20\x42\x41\x53\x45\x38\x35\x21\x20" "\x1"
);std::string z717c6a933a=zbb52cbc145::decode(z44e6103122);std::cout<<
z717c6a933a<<"\x20\x3c\x2d\x3e\x20"<<z44e6103122<<std::endl;std::string 
z6bf1cfc59d=z299a2ced8b::z826d3c3166(
"\x48\x65\x6c\x6c\x6f\x20\x77\x6f\x72\x6c\x64\x20\x66\x72\x6f\x6d\x20\x42\x41\x53\x45\x39\x31\x21\x20" "\x1"
);std::string z7d76a937f8=z299a2ced8b::decode(z6bf1cfc59d);std::cout<<
z7d76a937f8<<"\x20\x3c\x2d\x3e\x20"<<z6bf1cfc59d<<std::endl<<std::endl;
z666082a493(
"\x4d\x61\x6e\x20\x69\x73\x20\x64\x69\x73\x74\x69\x6e\x67\x75\x69\x73\x68\x65\x64\x2c\x20\x6e\x6f\x74\x20\x6f\x6e\x6c\x79\x20\x62\x79\x20\x68\x69\x73\x20\x72\x65\x61\x73\x6f\x6e\x2c\x20\x62\x75\x74\x20\x62\x79\x20\x74\x68\x69\x73\x20\x73\x69\x6e\x67\x75\x6c\x61\x72\x20\x70\x61\x73\x73\x69\x6f\x6e\x20\x66\x72\x6f\x6d" "\n"
"\x6f\x74\x68\x65\x72\x20\x61\x6e\x69\x6d\x61\x6c\x73\x2c\x20\x77\x68\x69\x63\x68\x20\x69\x73\x20\x61\x20\x6c\x75\x73\x74\x20\x6f\x66\x20\x74\x68\x65\x20\x6d\x69\x6e\x64\x2c\x20\x74\x68\x61\x74\x20\x62\x79\x20\x61\x20\x70\x65\x72\x73\x65\x76\x65\x72\x61\x6e\x63\x65\x20\x6f\x66\x20\x64\x65\x6c\x69\x67\x68\x74\x20\x69\x6e\x20\x74\x68\x65\x20\x63\x6f\x6e\x74\x69\x6e\x75\x65\x64" "\n"
"\x61\x6e\x64\x20\x69\x6e\x64\x65\x66\x61\x74\x69\x67\x61\x62\x6c\x65\x20\x67\x65\x6e\x65\x72\x61\x74\x69\x6f\x6e\x20\x6f\x66\x20\x6b\x6e\x6f\x77\x6c\x65\x64\x67\x65\x2c\x20\x65\x78\x63\x65\x65\x64\x73\x20\x74\x68\x65\x20\x73\x68\x6f\x72\x74\x20\x76\x65\x68\x65\x6d\x65\x6e\x63\x65\x20\x6f\x66\x20\x61\x6e\x79\x20\x63\x61\x72\x6e\x61\x6c\x20\x70\x6c\x65\x61\x73\x75\x72\x65\x2e"
);z666082a493("\x68\x65\x6c\x6c\x6f\x20\x77\x6f\x72\x6c\x64\x20" "\x1\x2");
z666082a493(std::string("abc\x1\x2\0",(0x27b+7326-0x1f13)));std::string 
z97677caadf;z97677caadf.reserve((0xa5d+3956-0x18d1)*(0xc43+1203-0xff6)*
(0xdc6+3862-0x1bdc));for(int zdae0797406=(0x36c+7874-0x212e);--zdae0797406>=
(0x1c89+1396-0x21fd);){for(int z211d9af9f8=(0x3c5+6482-0x1c17);--z211d9af9f8>=
(0x5a3+6167-0x1dba);){for(int k=(0x7d7+4589-0x18c4);--k>=(0x90d+636-0xb89);){
z97677caadf+=char(zdae0797406)+char(z211d9af9f8)+char(k);}}}z11536067dc(
z97677caadf);std::cout<<"\x41\x6c\x6c\x20\x6f\x6b\x2e"<<std::endl;}
#endif
#ifdef z2d89d9b183
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
int main(int z8659e37a83,const char**zcb6ff7ab5e){if(z8659e37a83==
(0x121+9616-0x26ad)){std::ifstream z561bb26d6d(zcb6ff7ab5e[(0x10ed+1489-0x16bb)]
,std::ios::binary);std::stringstream z6c73fb9766;z6c73fb9766<<z561bb26d6d.rdbuf(
);bool ze81a30cb5d=zcb6ff7ab5e[(0x50b+3160-0x1162)][(0xc5d+1561-0x1276)]==
((char)(0x2a5+7568-0x1fd0)),dec=zcb6ff7ab5e[(0x137f+2648-0x1dd6)][
(0x1604+911-0x1993)]==((char)(0x35d+1976-0xab1));bool zcc90c00e38=zcb6ff7ab5e[
(0x43a+5114-0x1832)][(0x80c+5796-0x1eb0)]==((char)(0x5c+8121-0x1fdf)),
zce24880f05=zcb6ff7ab5e[(0x1708+2186-0x1f90)][(0x13c5+1994-0x1b8f)]==
((char)(0x1614+944-0x198c)),z42572c5fff=zcb6ff7ab5e[(0x1489+2805-0x1f7c)][
(0xa8c+5898-0x2196)]==((char)(0x1ef+7898-0x2090));if(z42572c5fff&&ze81a30cb5d)
return std::cout<<z299a2ced8b::z826d3c3166(z6c73fb9766.str()),(z561bb26d6d.good(
)?(0x1266+1062-0x168c):(0x13fd+4289-0x24bd));else if(z42572c5fff&&dec)return std
::cout<<z299a2ced8b::decode(z6c73fb9766.str()),(z561bb26d6d.good()?
(0x8b7+5982-0x2015):(0xa3d+3489-0x17dd));else if(zce24880f05&&ze81a30cb5d)return
 std::cout<<zbb52cbc145::encode(z6c73fb9766.str()),(z561bb26d6d.good()?
(0xbf7+1220-0x10bb):(0x112b+234-0x1214));else if(zce24880f05&&dec)return std::
cout<<zbb52cbc145::decode(z6c73fb9766.str()),(z561bb26d6d.good()?
(0x173d+1245-0x1c1a):(0xc24+3295-0x1902));else if(zcc90c00e38&&ze81a30cb5d)
return std::cout<<z51bcce0411::encode(z6c73fb9766.str()),(z561bb26d6d.good()?
(0xd56+3040-0x1936):(0x1059+5387-0x2563));else if(zcc90c00e38&&dec)return std::
cout<<z51bcce0411::zbc7b5dd088(z6c73fb9766.str()),(z561bb26d6d.good()?
(0x43b+1603-0xa7e):(0x5bd+7214-0x21ea));}std::cout<<zcb6ff7ab5e[
(0x150+6830-0x1bfe)]<<
"\x20\x5b\x65\x7c\x64\x5d\x20\x5b\x36\x34\x7c\x38\x35\x7c\x39\x31\x5d\x20\x66\x69\x6c\x65"
<<std::endl;return(0x14af+591-0x16fd);}
#endif
