#ifndef crypto_auth_hmacsha256_H
#define crypto_auth_hmacsha256_H

#define crypto_auth_hmacsha256_ref_BYTES 32
#define crypto_auth_hmacsha256_ref_KEYBYTES 32

#ifdef __cplusplus
#include <string>
extern std::string crypto_auth_hmacsha256_ref(const std::string &,const std::string &);
extern void crypto_auth_hmacsha256_ref_verify(const std::string &,const std::string &,const std::string &);
extern "C" {
#endif
extern int crypto_auth_hmacsha256_ref(unsigned char *,const unsigned char *,unsigned long long,const unsigned char *);
extern int crypto_auth_hmacsha256_ref_verify(const unsigned char *,const unsigned char *,unsigned long long,const unsigned char *);
#ifdef __cplusplus
}
#endif

#define crypto_auth_hmacsha256 crypto_auth_hmacsha256_ref
#define crypto_auth_hmacsha256_verify crypto_auth_hmacsha256_ref_verify
#define crypto_auth_hmacsha256_BYTES crypto_auth_hmacsha256_ref_BYTES
#define crypto_auth_hmacsha256_KEYBYTES crypto_auth_hmacsha256_ref_KEYBYTES
#define crypto_auth_hmacsha256_IMPLEMENTATION "ref"
#ifndef crypto_auth_hmacsha256_ref_VERSION
#define crypto_auth_hmacsha256_ref_VERSION "-"
#endif
#define crypto_auth_hmacsha256_VERSION crypto_auth_hmacsha256_ref_VERSION

#endif
