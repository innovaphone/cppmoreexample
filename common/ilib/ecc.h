/*---------------------------------------------------------------------------*/
/* ecc.h                                                                     */
/* Copyright (c) innovaphone 2015                                            */
/*                                                                           */
/* Elliptic curve cryptography                                               */
/* - Elliptic curve Diffie-Hellman                                           */
/* - Elliptic curve digital signature algorithm                              */
/*---------------------------------------------------------------------------*/

NAMESPACE_BEGIN

enum elliptic_curve_t {
    EC_UNDEFINED = 0,
    EC_SECP256R1 = 0x0017
};

class ecc {
public:
    static bool elliptic_curve_supported(elliptic_curve_t curve);
    static size_t elliptic_curve_size(elliptic_curve_t curve);
    static size_t signature_size(elliptic_curve_t curve);
    static size_t pkix_signature_size(elliptic_curve_t curve);
    static bool make_key(byte * public_key, byte * private_key, elliptic_curve_t curve);
    static bool shared_secret(byte * shared_secret, const byte * public_key, const byte * private_key, elliptic_curve_t curve);
    static bool sign(byte * signature, const byte * hash, size_t hash_len, const byte * private_key, elliptic_curve_t curve);
    static bool verify(const byte * signature, const byte * hash, size_t hash_len, const byte * public_key, elliptic_curve_t curve);
    static bool pkix_sign(byte * signature, const byte * hash, size_t hash_len, const byte * private_key, elliptic_curve_t curve);
    static bool pkix_verify(const byte * signature, size_t signature_len, const byte * hash, size_t hash_len, const byte * public_key, elliptic_curve_t curve);
};

NAMESPACE_END
