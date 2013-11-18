#ifndef __CRYPTO_H
#define __CRYPTO_H

#include <crypto_box.h>
#include <ostream>
#include <memory>
#include <exception>

using namespace std;

class bad_publickey : public exception {
	virtual const char* what() const throw() {
		return "Invalid public key";
	}
};


class publickey {
	public:
		publickey(const unsigned char* pk);
		publickey(const string &pk);
		string bytes() const { return string((char *)m_key, crypto_box_PUBLICKEYBYTES); }
	private:
		unsigned char m_key[crypto_box_PUBLICKEYBYTES];
	friend ostream& operator<<(ostream& os, const publickey& pk);
	friend bool operator==(const publickey& lhs, const publickey& rhs);
};

/* A public/private key pair */
class keypair {
	public:
		/* Create a random key pair (for testing) */
		keypair();
		string decrypt(publickey const &sender, const string &ciphertext);
		string encrypt(publickey const &destination, const string &plaintext);
		const shared_ptr<publickey> pk() const { return m_publickey; }
	private:
		uint8_t m_key[crypto_box_SECRETKEYBYTES];
		shared_ptr<publickey> m_publickey;
};


#endif
