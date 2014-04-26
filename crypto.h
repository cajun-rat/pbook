#ifndef __CRYPTO_H
#define __CRYPTO_H

#include <crypto_box.h>
#include <exception>
#include <memory>
#include <ostream>
#include <vector>

using namespace std;

class bad_publickey : public exception {
	virtual const char* what() const throw() {
		return "Invalid public key";
	}
};

class keypairdata;

class publickeydata {
	public:
		publickeydata(const unsigned char* pk);
		publickeydata(const string &pk);
		publickeydata(const vector<uint8_t>::iterator pk);
		string bytes() const { return string((char *)m_key, crypto_box_PUBLICKEYBYTES); }
	private:
		// crypto_box will read off the end of the input see bug.cpp
		unsigned char m_key[crypto_box_PUBLICKEYBYTES+7];
	friend ostream& operator<<(ostream& os, const publickeydata& pk);
	friend bool operator==(const publickeydata& lhs, const publickeydata& rhs);
	friend bool operator<(const publickeydata& lhs, const publickeydata& rhs);
	friend class keypairdata;
};

typedef shared_ptr<publickeydata> publickey;
bool operator<(const publickey& lhs, const publickey& rhs);
bool operator==(const publickey& lhs, const publickey& rhs);
bool operator!=(const publickey& lhs, const publickey& rhs);
ostream& operator<<(ostream& os, const publickey& pk);

/* The result of keypairdata::decrypt */
enum decryptresult { 
	/* Decryption was successfull, you are safe to use the result */
	Success = 0, 
	/* The message was too short */
	TooShort,
	/* The message failed to decrypt */
	DecryptFailed
};

/* A public/private key pair */
class keypairdata {
	public:
		/* Create a random key pair (for testing) */
		keypairdata();
		decryptresult decrypt(const string& ciphertext, tuple<string,publickey> &result);
		tuple<string,publickey> decrypt(const string &ciphertext);
		string encrypt(publickey const destination, const string &plaintext);
		const publickey pk() const { return m_publickey; }
	private:
		// crypto_box will read off the end of the input see bug.cpp
		uint8_t m_key[crypto_box_SECRETKEYBYTES+7];
		publickey m_publickey;
};

typedef shared_ptr<keypairdata> keypair;

#endif
