#ifndef crypto_scalarmult_H
#define crypto_scalarmult_H

#include "crypto_scalarmult_curve25519.h"

#define crypto_scalarmult crypto_scalarmult_curve25519
#define crypto_scalarmult_base crypto_scalarmult_curve25519_base
#define crypto_scalarmult_BYTES crypto_scalarmult_curve25519_BYTES
#define crypto_scalarmult_SCALARBYTES crypto_scalarmult_curve25519_SCALARBYTES
#define crypto_scalarmult_PRIMITIVE "curve25519"
#define crypto_scalarmult_IMPLEMENTATION crypto_scalarmult_curve25519_IMPLEMENTATION
#define crypto_scalarmult_VERSION crypto_scalarmult_curve25519_VERSION

#endif
