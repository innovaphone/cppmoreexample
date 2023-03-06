/*---------------------------------------------------------------------------*/
/* base64.h                                                                  */
/* copyright (c) innovaphone 2002, 2007                                      */
/*                                                                           */
/* BASE64 encoding/decoding                                                  */
/*---------------------------------------------------------------------------*/

#ifndef _BASE64_H_
#define _BASE64_H_

NAMESPACE_BEGIN

unsigned decode_base64_bin(const char * in, byte * out, unsigned size_of_out);
dword decode_base64url_bin(const char * in, byte * out, dword size_of_out);
dword decode_base64(const char * in, byte * out, int n);
dword decode_base64url(const char * in, byte * out, dword size_of_in);
dword encode_base64(const byte * in, char * out, int len);
dword encode_base64url(const byte * in, char * out, dword len);
dword base64url_to_base64(const char* in, char* out, dword in_len);
dword base64_to_base64url(const char* in, char* out, dword in_len);

NAMESPACE_END

#endif /* _BASE64_H_ */
