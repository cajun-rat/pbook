#include "crypto.h"
#include <string.h>
#include <algorithm>
#include <iostream>
#include "randombytes.h"
#include <cassert>

publickeydata::publickeydata(const unsigned char *pk) {
	memcpy(m_key, pk, crypto_box_PUBLICKEYBYTES);
}

publickeydata::publickeydata(const string &bytes) {
	if (bytes.length() != crypto_box_PUBLICKEYBYTES) {
		throw bad_publickey();
	}
	std::copy(bytes.begin(), bytes.end(), m_key);
}

bool operator==(const publickeydata& lhs, const publickeydata& rhs) {
	return std::equal(lhs.m_key, lhs.m_key+crypto_box_PUBLICKEYBYTES, rhs.m_key);
}

bool operator<(const publickeydata& lhs, const publickeydata& rhs) {
	return memcmp(lhs.m_key, rhs.m_key, crypto_box_PUBLICKEYBYTES) < 0;
}

bool operator<(const publickey& lhs, const publickey& rhs) {
	return *lhs < *rhs;
}

const uint8_t shared_pk[] = {
	183, 186, 46, 122, 249, 181, 36, 160, 4, 111, 202, 243, 236, 138, 194, 99,
	137, 222, 6, 187, 195, 57, 72, 66, 78, 119, 122, 122, 31, 100, 245, 44};

const uint8_t shared_sk[] = {
	165, 126, 124, 193, 224, 246, 154, 237, 46, 127, 10, 54, 3, 216, 1, 49,
	47, 22, 119, 114, 244, 40, 219, 26, 66, 25, 84, 151, 163, 110, 140, 134};


keypair::keypair() {
	unsigned char pk[crypto_box_PUBLICKEYBYTES];
	crypto_box_keypair(pk, m_key);
	m_publickey = make_shared<publickeydata>(pk);
}

/* The format is 
   Nonce1, {PK_sender, Nonce2}|dest   , {msg}|dest
                              |public        |sender */
/* TODO: use the nonces for freshness */
string keypair::encrypt(publickey const destination, const string &plaintext) {
	assert(destination.get() != nullptr);
	assert(crypto_box_PUBLICKEYBYTES == 32);
	assert(crypto_box_SECRETKEYBYTES == 32);
	string res;
	string zeropadding(crypto_box_ZEROBYTES, '\0');
	uint8_t nonce1[crypto_box_NONCEBYTES], nonce2[crypto_box_NONCEBYTES];
	randombytes(nonce1, crypto_box_NONCEBYTES);
	randombytes(nonce2, crypto_box_NONCEBYTES);

	res.append((char*) nonce1, crypto_box_NONCEBYTES);

	vector<uint8_t> firstmsg(crypto_box_ZEROBYTES);
	std::copy(destination->m_key, destination->m_key + crypto_box_PUBLICKEYBYTES, back_inserter(firstmsg));
	std::copy(nonce2, nonce2 + crypto_box_NONCEBYTES, back_inserter(firstmsg));
	vector<uint8_t> ciphertext(firstmsg.size());
	//cout << "encrypting to " << *destination << endl;
	uint8_t *pk = &destination->m_key[0];
	//cout << "pk starts at " << std::hex << (uint64_t) pk << endl;
	//cout << "pk ends at " << std::hex << (uint64_t) &pk[31] << endl;
	int err = crypto_box(ciphertext.data(), firstmsg.data(), firstmsg.size(),  
		nonce1, &destination->m_key[0], shared_sk);
	if (err) {
		cout << "encryption error\n";
	}

	res.append((char*) ciphertext.data() + crypto_box_BOXZEROBYTES, firstmsg.size() - crypto_box_BOXZEROBYTES);

	int secondmsglen = plaintext.length() + crypto_box_ZEROBYTES;
	vector<uint8_t> secondplaintext(secondmsglen);
	std::copy(plaintext.begin(), plaintext.end(), secondplaintext.begin() + crypto_box_ZEROBYTES);
	vector<uint8_t> secondciphertext(secondmsglen);
	err = crypto_box(secondciphertext.data(), secondplaintext.data(), secondmsglen,
		nonce2, destination->m_key, m_key);

	if (err) {
		cout << "phase 2 encryption error\n";
	}

	res.append(secondciphertext.begin() + crypto_box_BOXZEROBYTES, secondciphertext.end());

	return res;
}

ostream& operator<<(ostream& os, const publickeydata& pk) {
	os << "pk {" << (int) pk.m_key[0] << ", " << (int) pk.m_key[1] << " ... " << (int) pk.m_key[31] <<"}";
	return os;
}

