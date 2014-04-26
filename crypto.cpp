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

publickeydata::publickeydata(const vector<uint8_t>::iterator pk) {
	std::copy(pk, pk + crypto_box_PUBLICKEYBYTES, m_key);
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

ostream& operator<<(ostream& os, const publickeydata& pk) {
	os << "pk {" << (int) pk.m_key[0] << ", " << (int) pk.m_key[1] << " ... " << (int) pk.m_key[31] <<"}";
	return os;
}


bool operator==(const publickey& lhs, const publickey& rhs) {
	return *lhs == *rhs;
}

bool operator!=(const publickey& lhs, const publickey& rhs) {
	return !(lhs == rhs);
}

ostream& operator<<(ostream& os, const publickey& pk) {
	os << *pk;
	return os;
}


const uint8_t shared_pk[] = {
	183, 186, 46, 122, 249, 181, 36, 160, 4, 111, 202, 243, 236, 138, 194, 99,
	137, 222, 6, 187, 195, 57, 72, 66, 78, 119, 122, 122, 31, 100, 245, 44};

const uint8_t shared_sk[] = {
	165, 126, 124, 193, 224, 246, 154, 237, 46, 127, 10, 54, 3, 216, 1, 49,
	47, 22, 119, 114, 244, 40, 219, 26, 66, 25, 84, 151, 163, 110, 140, 134};


keypairdata::keypairdata() {
	unsigned char pk[crypto_box_PUBLICKEYBYTES];
	crypto_box_keypair(pk, m_key);
	m_publickey = make_shared<publickeydata>(pk);
}

/* The format is 
   Nonce1, {PK_sender, Nonce2}|dest   , {msg}|dest
                              |public        |sender */
/* TODO: use the nonces for freshness */
string keypairdata::encrypt(publickey const destination, const string &plaintext) const {
	assert(destination.get() != nullptr);
	string res;
	string zeropadding(crypto_box_ZEROBYTES, '\0');
	uint8_t nonce1[crypto_box_NONCEBYTES], nonce2[crypto_box_NONCEBYTES];
	randombytes(nonce1, crypto_box_NONCEBYTES);
	randombytes(nonce2, crypto_box_NONCEBYTES);

	res.append((char*) nonce1, crypto_box_NONCEBYTES);

	vector<uint8_t> firstmsg(crypto_box_ZEROBYTES);
	unsigned char* senderpk = pk()->m_key;
	std::copy(senderpk, senderpk + crypto_box_PUBLICKEYBYTES, back_inserter(firstmsg));
	std::copy(nonce2, nonce2 + crypto_box_NONCEBYTES, back_inserter(firstmsg));
	vector<uint8_t> ciphertext(firstmsg.size());
	//cout << "encrypting to " << *destination << endl;
	//uint8_t *pk = &destination->m_key[0];
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

decryptresult keypairdata::decrypt(const string& ciphertext, tuple<string,publickey> &result) const {
	if (ciphertext.size() < crypto_box_NONCEBYTES) {
		cout << "ciphertext is only " << ciphertext.size() << " bytes\n";
		return decryptresult::TooShort;
	}
	string::const_iterator i = ciphertext.begin();
	// Read out the nonce
	uint8_t nonce1[crypto_box_NONCEBYTES];
	std::copy(i, i + crypto_box_NONCEBYTES, nonce1);
	int firstmsgsize = crypto_box_PUBLICKEYBYTES + crypto_box_NONCEBYTES + crypto_box_ZEROBYTES - crypto_box_BOXZEROBYTES;
	i+= crypto_box_NONCEBYTES;
	// Read the first message
	if (ciphertext.end() - i < firstmsgsize) {
		return decryptresult::TooShort;
	}
	vector<uint8_t> firstciphertext(crypto_box_BOXZEROBYTES);
	std::copy(i, i + firstmsgsize, back_inserter(firstciphertext));
	i += firstmsgsize;
	vector<uint8_t> firstmsg(firstciphertext.size());
	int err = crypto_box_open(firstmsg.data(), firstciphertext.data(), firstciphertext.size(), nonce1, 
		m_publickey->m_key, shared_sk);
	//cout << "crypto_box_open result #1:" << err << "\n";  
	if (err != 0) {
		return decryptresult::DecryptFailed;
	}
	publickey sender = make_shared<publickeydata>(firstmsg.begin() + crypto_box_ZEROBYTES);
	//cout << "sender PK:" << *sender << "\n";	
	uint8_t nonce2[crypto_box_NONCEBYTES];
	std::copy(firstmsg.begin() + crypto_box_ZEROBYTES + crypto_box_PUBLICKEYBYTES,
		firstmsg.end(), nonce2);
	// Read the second message
	vector<uint8_t> secondciphertext(crypto_box_BOXZEROBYTES);
	std::copy(i, ciphertext.end(), back_inserter(secondciphertext));
	vector<uint8_t> secondmsg(secondciphertext.size());
	int err2 = crypto_box_open(secondmsg.data(), secondciphertext.data(), secondciphertext.size(), nonce2, sender->m_key, m_key);
	//cout << "crypto_box_open result #2 " << err2 << "\n";
	if (err2 != 0) {
		return decryptresult::DecryptFailed;
	}
	string msg;
	std::copy(secondmsg.begin() + crypto_box_ZEROBYTES, secondmsg.end(), back_inserter(msg));
	get<0>(result) = msg;
	get<1>(result) = sender;
	return decryptresult::Success;
} 

