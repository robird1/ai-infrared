#ifndef FC_H
#define FC_H

#include <memory>
#include <vector>
#include <string>

namespace fc
{
    enum class Type
    {
        INT, FLOAT, DOUBLE, LONG, CHAR, STRING
    };

    enum class Version
    {
        FC1a
    };

    enum class Device
    {
        ANY, UL100, iCatch4, RPI3BP
    };

    // Supported devices
    // UL100
    // iCatch4
    // ALL

    namespace utils
    {
        std::string hex_to_bin_str(const std::string &hex);
        std::string bin_str_to_hex(const std::string &bin_str);
        std::string hash(const std::string &s);
        std::string generate_bin_qr_svg(const std::vector<char> &bin_data, const int margin);
    }

    namespace crypto
    {
        class RSAPublicKey
        {
        public:
            RSAPublicKey();
            void save_X509(const std::string &filename);
            void load_X509(const std::string &filename);
            void load_X509(const std::vector<char> &data);
            void save_PEM(const std::string &filename);
            void load_PEM(const std::string &filename);
            void save_DER(const std::string &filename);
            void load_BER(const std::string &filename);
            void load_BER(const std::vector<char> &data);

            //void load_AESWB(const std::string &filename);

            bool validate();
            bool compare(const fc::crypto::RSAPublicKey &public_key);
            std::string to_string();
            void from_string(const std::string &s);
            void *getPtr() const;
            ~RSAPublicKey();

        private:
            class A;
            A *a;
        };


        class RSAPrivateKey
        {
        public:
            RSAPrivateKey();
            void save_PKCS8(const std::string &filename);
            void load_PKCS8(const std::string &filename);
            void load_PKCS8(const std::vector<char> &data);
            void save_PEM(const std::string &filename, const std::string &password = {});
            void load_PEM(const std::string &filename, const std::string &password = {});
            void save_DER(const std::string &filename);
            void load_BER(const std::string &filename);
            void load_BER(const std::vector<char> &data);

            bool validate();
            bool compare(const fc::crypto::RSAPrivateKey &private_key);
            std::string to_string();
            void from_string(const std::string &s);
            void *getPtr() const;
            ~RSAPrivateKey();

        private:
            class A;
            A *a;
        };

        // Constructor automatically creates the RSA 2048 bits keys
        class RSAKeyPair
        {
        public:
            RSAKeyPair();
            RSAPublicKey RSA_public_key;
            RSAPrivateKey RSA_private_key;
            ~RSAKeyPair();
        };
    }

    class FaceCodeEncoder
    {
    public:

        enum class MODE
        {
            RSA_NO, RSA_ENCRYPTION, RSA_SIGNATURE, RSA_ENCRYPTION_SIGNATURE
        };

        enum class QR_EC
        {
            L, M, Q, H
        };


        FaceCodeEncoder();

        //void init(const Version &v, const std::vector<float> &f, const bool compressed);
        void init(const Version &v, const Device &d, const std::vector<float> &f, const bool compressed);

        bool encode(std::vector<char> *facecode_binary, const fc::FaceCodeEncoder::MODE &mode,
                    const char header[2] = nullptr);

        bool set_public_RSA_encryption_key(const fc::crypto::RSAPublicKey &public_encryption_key);
        bool set_private_RSA_signature_key(const fc::crypto::RSAPrivateKey &private_signature_key);

        template<typename T>
        void push_data(T &a);
        void print_data();
        void to_base91(const std::vector<char> &facecode_binary, std::string *facecode_text);
        void to_base64(const std::vector<char> &facecode_binary, std::string *facecode_text);

        bool
        update_encryption_key(const std::vector<char> &facecode_binary, const fc::crypto::RSAPrivateKey &private_key,
                              const fc::crypto::RSAPublicKey &new_public_key, std::vector<char> *new_facecode_binary,
                              const char header[2] = nullptr);

        // Get
        std::string get_version() const;
        std::string get_device() const;
        std::string get_header() const;
        std::vector<float> get_code() const;
        int get_data_size() const;
        std::pair<fc::Type, void *> get_data(const int &idx);

        // QR Code
        void generate_QR_data(const std::vector<char> &facecode_binary, const fc::FaceCodeEncoder::QR_EC &ec, const int mask,
                              std::vector<unsigned char> *qr_code_data);
        void generate_QR_data(const std::string &facecode_text, const fc::FaceCodeEncoder::QR_EC &ec, const int mask,
                              std::vector<unsigned char> *qr_code_data);
        void generate_QR_svg(const std::vector<char> &facecode_binary, const fc::FaceCodeEncoder::QR_EC &ec, const int mask,
                             const int margin, std::string *qr_code_svg);
        void generate_QR_svg(const std::string &facecode_text, const fc::FaceCodeEncoder::QR_EC &ec, const int mask, const int margin,
                             std::string *qr_code_svg);
        // Experimental
        void generate_QR_data_color(const std::vector<char> &facecode_binary, const fc::FaceCodeEncoder::QR_EC &ec,
                                    std::vector<unsigned char> *qr_code_data);
        void generate_QR_data_color(const std::string &facecode_text, const fc::FaceCodeEncoder::QR_EC &ec,
                                    std::vector<unsigned char> *qr_code_data);
        ~FaceCodeEncoder();


    private:
        class F;

        std::unique_ptr<F> F_;
    };

    class FaceCodeDecoder
    {
    public:

        FaceCodeDecoder();

        bool set_private_RSA_decryption_key(const fc::crypto::RSAPrivateKey &private_decryption_key);
        bool set_public_RSA_signature_key(const fc::crypto::RSAPublicKey &public_signature_key);

        bool decode(const std::vector<char> &d);
        bool decode(const std::string &d);

        std::string get_version(const std::vector<char> &facecode) const;
        std::string get_version(const std::string &facecode) const;
        std::string get_device(const std::vector<char> &facecode) const;
        std::string get_device(const std::string &facecode) const;
        std::string get_header(const std::vector<char> &facecode) const;
        std::string get_header(const std::string &facecode) const;
        std::string get_type(const std::vector<char> &facecode) const;
        std::string get_type(const std::string &facecode) const;

        std::vector<float> get_code() const;
        int get_data_size() const;
        std::pair<fc::Type, void *> get_data(const int &idx);
        void print_data();

        ~FaceCodeDecoder();

    private:
        class F;
        std::unique_ptr<F> F_;
    };

}

#endif
