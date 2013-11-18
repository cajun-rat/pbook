#include "crypto.h"
#include <string.h>
#include <algorithm>

publickey::publickey(const unsigned char *pk) {
	memcpy(m_key, pk, crypto_box_PUBLICKEYBYTES);
}

publickey::publickey(const string &bytes) {
	if (bytes.length() != crypto_box_PUBLICKEYBYTES) {
		throw bad_publickey();
	}
	std::copy(bytes.begin(), bytes.end(), m_key);
}

bool operator==(const publickey& lhs, const publickey& rhs) {
	return std::equal(lhs.m_key, lhs.m_key+crypto_box_PUBLICKEYBYTES, rhs.m_key);
}

keypair::keypair() {
	unsigned char pk[crypto_box_PUBLICKEYBYTES];
	crypto_box_keypair(pk, m_key);
	m_publickey = make_shared<publickey>(pk);
}

ostream& operator<<(ostream& os, const publickey& pk) {
	os << "pk {" << (int) pk.m_key[0] << ", " << (int) pk.m_key[1] << ", " << (int) pk.m_key[2] <<"}";
	return os;
}

