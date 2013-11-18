#ifndef __CRYPTO_H
#define __CRYPTO_H

#include <crypto_box.h>

class publickey {
	public:
		publickey();
	
	uint8_t key[crypto_box_PUBLICKEYBYTES];
	friend ostream& operator<<(ostream& os, const publickey& pk);
};

class secretkey {
	public:
		/* create a keypair */
		secretkey(publickey &pk);
		string decrypt(string ciphertext);
};


#endif
