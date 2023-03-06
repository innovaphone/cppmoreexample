/*---------------------------------------------------------------------------*/
/* cipher.h                                                                  */
/* copyright (c) innovaphone 2016                                            */
/*                                                                           */
/*---------------------------------------------------------------------------*/

NAMESPACE_BEGIN

#define DES_DATA_SIZE       8
#define DES_KEY_SIZE        7

enum cipher_t {
    CIPHER_UNDEFINED,
    CIPHER_DES,
    CIPHER_AES,
    CIPHER_RC4
};

class Cipher {
protected:
    bool encrypt;
    cipher_t type;
#if defined ARMV7_CRYPT   
    __attribute__((aligned(16))) ulong64 encrypt_ctx[213], decrypt_ctx[213]; // max context size is aesbs_ctx , 13 * (8 * 16) + 32 + 4 bytes
#else
        ulong64 encrypt_ctx[129], decrypt_ctx[129];
#endif
public:
    Cipher();
    void Init(cipher_t type, const byte * key, int keyLen, bool encrypt = true, bool makekey = true);
    void Reset();

    void Block(const byte * in, byte * out);
    void Crypt(const byte * in, byte * out, int len);
    void CtrCrypt(const byte * in, byte * out, int len, byte * iv);
    void CfbCrypt(const byte * in, byte * out, int len, byte * iv);
    void CbcCrypt(const byte * in, byte * out, int len, byte * iv);
    int CtsCrypt(const byte * in, byte * out, int len, byte * iv);
    bool GcmCrypt(const byte * aad, int aad_len, const byte * in, byte * out, int len, byte * iv, byte * tag);
};

NAMESPACE_END
