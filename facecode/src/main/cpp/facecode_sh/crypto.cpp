
#include "crypto.h"
#include "tools.h"
std::string fc::za2341d21ce::zbd0a8b580a(const std::string&zca6df2d267,const std
::string&z4c85584d74){const char*k=z4c85584d74.c_str();std::string z127251fe38=
zca6df2d267;for(unsigned int zdae0797406=(0xa79+6630-0x245f);zdae0797406<
zca6df2d267.size();zdae0797406++){z127251fe38[zdae0797406]=zca6df2d267[
zdae0797406]^k[zdae0797406%(z4c85584d74.size()/sizeof(char))];}return 
z127251fe38;}std::string fc::za2341d21ce::za12655acbc(const std::string&str){
CryptoPP::SHA3_256 hash;CryptoPP::byte z7dfd43b573[CryptoPP::SHA3_256::
DIGESTSIZE];hash.CalculateDigest(z7dfd43b573,(CryptoPP::byte*)str.c_str(),str.
length());unsigned char z8502b221f2[CryptoPP::SHA3_256::DIGESTSIZE];std::copy(&
z7dfd43b573[(0x50c+684-0x7b8)],&z7dfd43b573[CryptoPP::SHA3_256::DIGESTSIZE],&
z8502b221f2[(0x14bf+2827-0x1fca)]);return std::string(reinterpret_cast<const 
char*>(z8502b221f2),CryptoPP::SHA3_256::DIGESTSIZE);}void fc::za2341d21ce::
RSA_generate_KeyPair_(unsigned int z5cb8b6d0f7,CryptoPP::RSA::PublicKey*
z495689be55,CryptoPP::RSA::PrivateKey*zca5d41add1){CryptoPP::
AutoSeededRandomPool z5726cc0629;CryptoPP::InvertibleRSAFunction z6cdf04f9f2;
z6cdf04f9f2.GenerateRandomWithKeySize(z5726cc0629,z5cb8b6d0f7);*z495689be55=
CryptoPP::RSA::PublicKey(z6cdf04f9f2);*zca5d41add1=CryptoPP::RSA::PrivateKey(
z6cdf04f9f2);}std::string fc::za2341d21ce::z4b285ff686(const CryptoPP::RSA::
PublicKey&z495689be55,const std::string&message){CryptoPP::AutoSeededRandomPool 
z2687d1390a;std::string zd70abecf74;CryptoPP::RSAES_OAEP_SHA_Encryptor 
z5543adec15(z495689be55);assert((0x131c+2328-0x1c34)!=z5543adec15.
FixedMaxPlaintextLength());assert(message.size()<=z5543adec15.
FixedMaxPlaintextLength());size_t zf3cf2af54e=z5543adec15.CiphertextLength(
message.size());assert((0x12a0+3478-0x2036)!=zf3cf2af54e);CryptoPP::SecByteBlock
 z453e234eb2(zf3cf2af54e);z5543adec15.Encrypt(z2687d1390a,(const CryptoPP::byte*
)message.data(),message.size(),z453e234eb2);return std::string(z453e234eb2.begin
(),z453e234eb2.end());}bool fc::za2341d21ce::z665dc26c9f(const CryptoPP::RSA::
PrivateKey&zca5d41add1,const std::string&zd70abecf74,std::string*message){
CryptoPP::AutoSeededRandomPool z2687d1390a;CryptoPP::RSAES_OAEP_SHA_Decryptor 
z1204db5d3e(zca5d41add1);assert((0x2d1+7563-0x205c)!=z1204db5d3e.
FixedCiphertextLength());assert(zd70abecf74.size()<=z1204db5d3e.
FixedCiphertextLength());size_t z382db59d05=z1204db5d3e.MaxPlaintextLength(
zd70abecf74.size());assert((0x230+3543-0x1007)!=z382db59d05);CryptoPP::
SecByteBlock z6ee58c62f9(z382db59d05);CryptoPP::DecodingResult result;try{result
=z1204db5d3e.Decrypt(z2687d1390a,(const CryptoPP::byte*)zd70abecf74.data(),
zd70abecf74.size(),z6ee58c62f9);}catch(...){return false;}if(!result.
isValidCoding)return false;size_t s=std::min(z382db59d05,result.messageLength);*
message=std::string(z6ee58c62f9.begin(),z6ee58c62f9.begin()+s);return true;}fc::
za2341d21ce::z92f64bfcf0 fc::za2341d21ce::zea1bd5af49(const std::string&
z4c85584d74,const std::string&message){z92f64bfcf0 zb271657760;std::string 
z2da3cc0b24;CryptoPP::AutoSeededRandomPool z2687d1390a;CryptoPP::SecByteBlock 
zb03064e0ff((0xba9+4703-0x1df8));z2687d1390a.GenerateBlock(zb03064e0ff,
zb03064e0ff.size());CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption z2684009c11((
CryptoPP::byte*)z4c85584d74.c_str(),(0x11bb+2552-0x1b93),zb03064e0ff);size_t 
z3e84a0e2ec=message.length();CryptoPP::byte zf5654cd684[z3e84a0e2ec];memcpy(
zf5654cd684,message.data(),z3e84a0e2ec);CryptoPP::byte z0b72e72e63[z3e84a0e2ec];
memset(z0b72e72e63,(0x1beb+2653-0x2648),sizeof(z0b72e72e63));z2684009c11.
ProcessData(z0b72e72e63,zf5654cd684,z3e84a0e2ec);unsigned char z0c1e8655f8[
(0x2aa+6346-0x1b64)];std::copy(zb03064e0ff.begin(),zb03064e0ff.end(),&
z0c1e8655f8[(0xc26+2792-0x170e)]);zb271657760.zb03064e0ff=std::string(
reinterpret_cast<const char*>(z0c1e8655f8),(0x1bf2+1001-0x1fcb));zb271657760.
zd70abecf74=std::string(reinterpret_cast<const char*>(z0b72e72e63),z3e84a0e2ec);
return zb271657760;}std::string fc::za2341d21ce::z32d07e8312(const std::string&
z4c85584d74,const std::string zb03064e0ff,const std::string&zd70abecf74){
CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption za7c5831d6a((CryptoPP::byte*)
z4c85584d74.c_str(),(0x11ca+3945-0x2113),(CryptoPP::byte*)zb03064e0ff.c_str());
size_t z3e84a0e2ec=zd70abecf74.length();CryptoPP::byte zf5654cd684[z3e84a0e2ec];
memset(zf5654cd684,(0x800+3828-0x16f4),sizeof(zf5654cd684));CryptoPP::byte 
z0b72e72e63[zd70abecf74.length()];memcpy(z0b72e72e63,zd70abecf74.data(),
zd70abecf74.length());za7c5831d6a.ProcessData(zf5654cd684,z0b72e72e63,
z3e84a0e2ec);return std::string(reinterpret_cast<const char*>(zf5654cd684),
z3e84a0e2ec);}std::string fc::za2341d21ce::z32d07e8312(const std::string&
z4c85584d74,const z92f64bfcf0&zb271657760){return z32d07e8312(z4c85584d74,
zb271657760.zb03064e0ff,zb271657760.zd70abecf74);}std::string fc::za2341d21ce::
z8c6b4183f9(const CryptoPP::RSA::PrivateKey&zca5d41add1,const std::string&
z8893c89d2b){CryptoPP::AutoSeededRandomPool z2687d1390a;CryptoPP::RSASS<CryptoPP
::PSS,CryptoPP::Whirlpool>::Signer zfd1e6c07f2(zca5d41add1);size_t length=
zfd1e6c07f2.MaxSignatureLength();CryptoPP::SecByteBlock z1b036b7299(length);
zfd1e6c07f2.SignMessage(z2687d1390a,(const CryptoPP::byte*)z8893c89d2b.data(),
z8893c89d2b.size(),z1b036b7299);return std::string(z1b036b7299.begin(),
z1b036b7299.end());}bool fc::za2341d21ce::ze3763691e2(const CryptoPP::RSA::
PublicKey&z495689be55,const std::string&z8893c89d2b,const std::string&
z014d1f4106){std::string zd70abecf74;CryptoPP::RSASS<CryptoPP::PSS,CryptoPP::
Whirlpool>::Verifier z33497f0ad1(z495689be55);return z33497f0ad1.VerifyMessage((
const CryptoPP::byte*)z8893c89d2b.data(),z8893c89d2b.size(),(const CryptoPP::
byte*)z014d1f4106.data(),z014d1f4106.size());}bool fc::za2341d21ce::zccd5fa9fae(
const CryptoPP::RSA::PublicKey&z495689be55){CryptoPP::AutoSeededRandomPool 
z5726cc0629;return z495689be55.Validate(z5726cc0629,(0x1050+5184-0x248d));}bool 
fc::za2341d21ce::zccd5fa9fae(const CryptoPP::RSA::PrivateKey&zca5d41add1){
CryptoPP::AutoSeededRandomPool z5726cc0629;return zca5d41add1.Validate(
z5726cc0629,(0xb4c+2342-0x146f));}bool fc::za2341d21ce::zb081cc2488(const 
CryptoPP::RSA::PublicKey&z495689be55,const CryptoPP::RSA::PublicKey&ze38905fc66)
{if(z495689be55.GetModulus()!=ze38905fc66.GetModulus()||z495689be55.
GetPublicExponent()!=ze38905fc66.GetPublicExponent())return false;return true;}
bool fc::za2341d21ce::zb081cc2488(const CryptoPP::RSA::PrivateKey&zca5d41add1,
const CryptoPP::RSA::PrivateKey&zeedc8f2883){if(zca5d41add1.GetModulus()!=
zeedc8f2883.GetModulus()||zca5d41add1.GetPublicExponent()!=zeedc8f2883.
GetPublicExponent()||zca5d41add1.GetPrivateExponent()!=zeedc8f2883.
GetPrivateExponent())return false;return true;}void fc::za2341d21ce::za3286fe3a5
(const std::string&z3740df047c,const CryptoPP::BufferedTransformation&
zd06bda30a2){CryptoPP::FileSink z8bb9d3d64d(z3740df047c.c_str());zd06bda30a2.
CopyTo(z8bb9d3d64d);z8bb9d3d64d.MessageEnd();}void fc::za2341d21ce::zd13d360aff(
const std::string&z3740df047c,CryptoPP::BufferedTransformation*zd06bda30a2){
CryptoPP::FileSource z8bb9d3d64d(z3740df047c.c_str(),true);z8bb9d3d64d.
TransferTo(*zd06bda30a2);zd06bda30a2->MessageEnd();}void fc::za2341d21ce::
z57b6949d5b(const std::string&z3740df047c,const CryptoPP::RSA::PrivateKey&
z4c85584d74){CryptoPP::ByteQueue z3ed798b443;z4c85584d74.DEREncodePrivateKey(
z3ed798b443);za3286fe3a5(z3740df047c,z3ed798b443);}void fc::za2341d21ce::
z4f0087eb11(const std::string&z3740df047c,const CryptoPP::RSA::PublicKey&
z4c85584d74){CryptoPP::ByteQueue z3ed798b443;z4c85584d74.DEREncodePublicKey(
z3ed798b443);za3286fe3a5(z3740df047c,z3ed798b443);}void fc::za2341d21ce::
z4bf875fc88(const std::string&z3740df047c,CryptoPP::RSA::PrivateKey*z4c85584d74)
{CryptoPP::ByteQueue z3ed798b443;zd13d360aff(z3740df047c,&z3ed798b443);
z4c85584d74->BERDecodePrivateKey(z3ed798b443,false,z3ed798b443.MaxRetrievable())
;}void fc::za2341d21ce::zccb0e2e79f(const std::string&z3740df047c,CryptoPP::RSA
::PublicKey*z4c85584d74){CryptoPP::ByteQueue z3ed798b443;zd13d360aff(z3740df047c
,&z3ed798b443);z4c85584d74->BERDecodePublicKey(z3ed798b443,false,z3ed798b443.
MaxRetrievable());}void fc::za2341d21ce::z4bf875fc88(const std::vector<char>&
data,CryptoPP::RSA::PrivateKey*z4c85584d74){CryptoPP::ByteQueue z3ed798b443;
z3ed798b443.Put((const CryptoPP::byte*)data.data(),data.size());z4c85584d74->
BERDecodePrivateKey(z3ed798b443,false,z3ed798b443.MaxRetrievable());}void fc::
za2341d21ce::zccb0e2e79f(const std::vector<char>&data,CryptoPP::RSA::PublicKey*
z4c85584d74){CryptoPP::ByteQueue z3ed798b443;z3ed798b443.Put((const CryptoPP::
byte*)data.data(),data.size());z4c85584d74->BERDecodePublicKey(z3ed798b443,false
,z3ed798b443.MaxRetrievable());}void fc::za2341d21ce::z1514a6d90a(const CryptoPP
::RSA::PrivateKey&zca5d41add1,std::string*z59876a71bc){CryptoPP::ByteQueue 
z3ed798b443;zca5d41add1.Save(z3ed798b443);size_t length=static_cast<size_t>(
z3ed798b443.CurrentSize());std::unique_ptr<char[]>z4179cb615e(new char[length]);
z3ed798b443.Get((CryptoPP::byte*)z4179cb615e.get(),length);*z59876a71bc=std::
string((const char*)z4179cb615e.get(),length);}void fc::za2341d21ce::z0d79a0bf63
(const std::string&z59876a71bc,CryptoPP::RSA::PrivateKey*zca5d41add1){CryptoPP::
ByteQueue z3ed798b443;z3ed798b443.Put((const CryptoPP::byte*)z59876a71bc.data(),
z59876a71bc.size());zca5d41add1->Load(z3ed798b443);}void fc::za2341d21ce::
zf2dd96ae58(const CryptoPP::RSA::PublicKey&z495689be55,std::string*z54a517a822){
CryptoPP::ByteQueue z3ed798b443;z495689be55.Save(z3ed798b443);size_t length=
static_cast<size_t>(z3ed798b443.CurrentSize());std::unique_ptr<char[]>
z4179cb615e(new char[length]);z3ed798b443.Get((CryptoPP::byte*)z4179cb615e.get()
,length);*z54a517a822=std::string((const char*)z4179cb615e.get(),length);}void 
fc::za2341d21ce::z312278d7a1(const std::string&z54a517a822,CryptoPP::RSA::
PublicKey*z495689be55){CryptoPP::ByteQueue z3ed798b443;z3ed798b443.Put((const 
CryptoPP::byte*)z54a517a822.data(),z54a517a822.size());z495689be55->Load(
z3ed798b443);}void fc::za2341d21ce::z7767b68524(const std::string&z3740df047c,
const CryptoPP::RSA::PrivateKey&z4c85584d74){CryptoPP::ByteQueue z3ed798b443;
z4c85584d74.Save(z3ed798b443);za3286fe3a5(z3740df047c,z3ed798b443);}void fc::
za2341d21ce::zba8e8150ca(const std::string&z3740df047c,CryptoPP::RSA::PrivateKey
*z4c85584d74){CryptoPP::ByteQueue z3ed798b443;zd13d360aff(z3740df047c,&
z3ed798b443);z4c85584d74->Load(z3ed798b443);}void fc::za2341d21ce::zba8e8150ca(
const std::vector<char>&data,CryptoPP::RSA::PrivateKey*z4c85584d74){CryptoPP::
ByteQueue z3ed798b443;z3ed798b443.Put((const CryptoPP::byte*)data.data(),data.
size());z4c85584d74->Load(z3ed798b443);}void fc::za2341d21ce::zec9d2d26dc(const 
std::string&z3740df047c,const CryptoPP::RSA::PublicKey&z4c85584d74){CryptoPP::
ByteQueue z3ed798b443;z4c85584d74.Save(z3ed798b443);za3286fe3a5(z3740df047c,
z3ed798b443);}void fc::za2341d21ce::z2830f89e95(const std::string&z3740df047c,
CryptoPP::RSA::PublicKey*z4c85584d74){CryptoPP::ByteQueue z3ed798b443;
zd13d360aff(z3740df047c,&z3ed798b443);z4c85584d74->Load(z3ed798b443);}void fc::
za2341d21ce::z2830f89e95(const std::vector<char>&data,CryptoPP::RSA::PublicKey*
z4c85584d74){CryptoPP::ByteQueue z3ed798b443;z3ed798b443.Put((const CryptoPP::
byte*)data.data(),data.size());z4c85584d74->Load(z3ed798b443);}void fc::
za2341d21ce::z61678a7373(const std::string&z3740df047c,const CryptoPP::RSA::
PublicKey&z4c85584d74){CryptoPP::FileSink zd1f914774f(z3740df047c.c_str());
PEM_Save(zd1f914774f,z4c85584d74);}void fc::za2341d21ce::zbb31811941(const std::
string&z3740df047c,CryptoPP::RSA::PublicKey*z4c85584d74){CryptoPP::FileSource 
zd1f914774f(z3740df047c.c_str(),true);PEM_Load(zd1f914774f,*z4c85584d74);}void 
fc::za2341d21ce::zac58a82c01(const std::string&z3740df047c,const CryptoPP::RSA::
PrivateKey&z4c85584d74,const std::string&z044dea4665){CryptoPP::FileSink 
zd1f914774f(z3740df047c.c_str());if(z044dea4665.empty())PEM_Save(zd1f914774f,
z4c85584d74);else{CryptoPP::AutoSeededRandomPool z919e589f1c;PEM_Save(
zd1f914774f,z4c85584d74,z919e589f1c,
"\x41\x45\x53\x2d\x32\x35\x36\x2d\x43\x42\x43",z044dea4665.c_str(),z044dea4665.
size());}}void fc::za2341d21ce::zfc21ba32c5(const std::string&z3740df047c,
CryptoPP::RSA::PrivateKey*z4c85584d74,const std::string&z044dea4665){CryptoPP::
FileSource zd1f914774f(z3740df047c.c_str(),true);if(z044dea4665.empty())PEM_Load
(zd1f914774f,*z4c85584d74);else PEM_Load(zd1f914774f,*z4c85584d74,z044dea4665.
c_str(),z044dea4665.size());}bool fc::za2341d21ce::z83cca138dc(int z7d11053ea7,
int zb6b50d757b,const char*z3740df047c,const char*seed){if(zb6b50d757b<
(0x231+3581-0x102d)||zb6b50d757b>(0xad1+7680-0x24e9))return false;CryptoPP::
RandomPool z2687d1390a;z2687d1390a.IncorporateEntropy((CryptoPP::byte*)seed,
strlen(seed));CryptoPP::ChannelSwitch*z418dbc072a=NULLPTR;CryptoPP::FileSource 
z771f6b504c(z3740df047c,false,new CryptoPP::SecretSharing(z2687d1390a,
z7d11053ea7,zb6b50d757b,z418dbc072a=new CryptoPP::ChannelSwitch));CryptoPP::
vector_member_ptrs<CryptoPP::FileSink>z00961ce4d5(zb6b50d757b);std::string 
za8c49e6d62;for(int zdae0797406=(0x1869+1932-0x1ff5);zdae0797406<zb6b50d757b;
zdae0797406++){char zc313dde62a[(0x10a3+4919-0x23d5)]="\x2e\x30\x30\x30";
zc313dde62a[(0xe41+1226-0x130a)]=((char)(0xbf5+36-0xbe9))+CryptoPP::byte(
zdae0797406/(0x103f+714-0x12a5));zc313dde62a[(0x12cf+3013-0x1e92)]=
((char)(0xdd0+1483-0x136b))+CryptoPP::byte((zdae0797406/(0x17e8+1556-0x1df2))%
(0x1122+4930-0x245a));zc313dde62a[(0x17e6+2536-0x21cb)]=
((char)(0x11aa+5097-0x2563))+CryptoPP::byte(zdae0797406%(0x9dc+455-0xb99));
z00961ce4d5[zdae0797406].reset(new CryptoPP::FileSink((std::string(z3740df047c)+
zc313dde62a).c_str()));za8c49e6d62=CryptoPP::WordToString<CryptoPP::word32>(
zdae0797406);z00961ce4d5[zdae0797406]->Put((const CryptoPP::byte*)za8c49e6d62.
data(),(0x1846+3535-0x2611));z418dbc072a->AddRoute(za8c49e6d62,*z00961ce4d5[
zdae0797406],CryptoPP::DEFAULT_CHANNEL);}z771f6b504c.PumpAll();return true;}bool
 fc::za2341d21ce::z87138f6ec7(int z7d11053ea7,const char*zdf981b3344,char*const*
ze62395cdeb){if(z7d11053ea7<(0x388+5871-0x1a76)||z7d11053ea7>(0xfd4+579-0xe2f))
return false;CryptoPP::SecretRecovery za699b0a0c6(z7d11053ea7,new CryptoPP::
FileSink(zdf981b3344));CryptoPP::vector_member_ptrs<CryptoPP::FileSource>
zcb4c952794(z7d11053ea7);CryptoPP::SecByteBlock za8c49e6d62((0x7fa+1145-0xc6f));
int zdae0797406;for(zdae0797406=(0xa41+4732-0x1cbd);zdae0797406<z7d11053ea7;
zdae0797406++){zcb4c952794[zdae0797406].reset(new CryptoPP::FileSource(
ze62395cdeb[zdae0797406],false));zcb4c952794[zdae0797406]->Pump(
(0x680+5514-0x1c06));zcb4c952794[zdae0797406]->Get(za8c49e6d62,
(0x812+6829-0x22bb));zcb4c952794[zdae0797406]->Attach(new CryptoPP::
ChannelSwitch(za699b0a0c6,std::string((char*)za8c49e6d62.begin(),
(0xb10+3860-0x1a20))));}while(zcb4c952794[(0x14db+2730-0x1f85)]->Pump(
(0x8b5+7694-0x25c3)))for(zdae0797406=(0x26b+7586-0x200c);zdae0797406<z7d11053ea7
;zdae0797406++)zcb4c952794[zdae0797406]->Pump((0x1933+3193-0x24ac));for(
zdae0797406=(0x1477+817-0x17a8);zdae0797406<z7d11053ea7;zdae0797406++)
zcb4c952794[zdae0797406]->PumpAll();return true;}bool fc::za2341d21ce::
z90976c7263(int z7d11053ea7,int zb6b50d757b,const char*z3740df047c){if(
z7d11053ea7<(0x57c+2186-0xe05)||z7d11053ea7>(0xfa0+4320-0x1c98))return false;
CryptoPP::ChannelSwitch*z418dbc072a=NULLPTR;CryptoPP::FileSource z771f6b504c(
z3740df047c,false,new CryptoPP::InformationDispersal(z7d11053ea7,zb6b50d757b,
z418dbc072a=new CryptoPP::ChannelSwitch));CryptoPP::vector_member_ptrs<CryptoPP
::FileSink>z00961ce4d5(zb6b50d757b);std::string za8c49e6d62;for(int zdae0797406=
(0x1b0+8756-0x23e4);zdae0797406<zb6b50d757b;zdae0797406++){char zc313dde62a[
(0xb3+3541-0xe83)]="\x2e\x30\x30\x30";zc313dde62a[(0x20d+8524-0x2358)]=
((char)(0x1810+273-0x18f1))+CryptoPP::byte(zdae0797406/(0x6ad+4440-0x17a1));
zc313dde62a[(0x11b9+1978-0x1971)]=((char)(0xf5f+1933-0x16bc))+CryptoPP::byte((
zdae0797406/(0x1b41+934-0x1edd))%(0x1cf+751-0x4b4));zc313dde62a[
(0x1140+4544-0x22fd)]=((char)(0x447+4592-0x1607))+CryptoPP::byte(zdae0797406%
(0x4e3+6572-0x1e85));z00961ce4d5[zdae0797406].reset(new CryptoPP::FileSink((std
::string(z3740df047c)+zc313dde62a).c_str()));za8c49e6d62=CryptoPP::WordToString<
CryptoPP::word32>(zdae0797406);z00961ce4d5[zdae0797406]->Put((const CryptoPP::
byte*)za8c49e6d62.data(),(0x1ad2+2111-0x230d));z418dbc072a->AddRoute(za8c49e6d62
,*z00961ce4d5[zdae0797406],CryptoPP::DEFAULT_CHANNEL);}z771f6b504c.PumpAll();
return true;}bool fc::za2341d21ce::z9166fd79df(int z7d11053ea7,const char*
zdf981b3344,char*const*ze62395cdeb){if(z7d11053ea7<(0x5a4+2069-0xdb8)||
z7d11053ea7>(0x1add+546-0x1917))return false;CryptoPP::InformationRecovery 
za699b0a0c6(z7d11053ea7,new CryptoPP::FileSink(zdf981b3344));CryptoPP::
vector_member_ptrs<CryptoPP::FileSource>zcb4c952794(z7d11053ea7);CryptoPP::
SecByteBlock za8c49e6d62((0x2c+1082-0x462));int zdae0797406;for(zdae0797406=
(0x4c2+3489-0x1263);zdae0797406<z7d11053ea7;zdae0797406++){zcb4c952794[
zdae0797406].reset(new CryptoPP::FileSource(ze62395cdeb[zdae0797406],false));
zcb4c952794[zdae0797406]->Pump((0x791+6854-0x2253));zcb4c952794[zdae0797406]->
Get(za8c49e6d62,(0x149f+3585-0x229c));zcb4c952794[zdae0797406]->Attach(new 
CryptoPP::ChannelSwitch(za699b0a0c6,std::string((char*)za8c49e6d62.begin(),
(0x45d+2016-0xc39))));}while(zcb4c952794[(0x63a+6572-0x1fe6)]->Pump(
(0x8bf+1953-0xf60)))for(zdae0797406=(0xdf+5365-0x15d3);zdae0797406<z7d11053ea7;
zdae0797406++)zcb4c952794[zdae0797406]->Pump((0x1c06+130-0x1b88));for(
zdae0797406=(0x1908+1044-0x1d1c);zdae0797406<z7d11053ea7;zdae0797406++)
zcb4c952794[zdae0797406]->PumpAll();return true;}
