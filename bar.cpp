#include "network.h"
#include "client.h"
#include "server.h"
#include "crypto.h"

using namespace std;
using namespace boost::asio::ip;
using namespace boost::signals2;

void m(const string msg) {
	cout << "GOT MSG:" << msg << endl;
}
const uint8_t shared_sk[] = {
	165, 126, 124, 193, 224, 246, 154, 237, 46, 127, 10, 54, 3, 216, 1, 49,
	47, 22, 119, 114, 244, 40, 219, 26, 66, 25, 84, 151, 163, 110, 140, 134};

int main() {
	uint8_t *pka = (uint8_t*) malloc(crypto_box_PUBLICKEYBYTES+1);
	uint8_t *ska = (uint8_t*) malloc(crypto_box_SECRETKEYBYTES+1);

	uint8_t *pk = pka + 1;
	uint8_t *sk = ska + 1;
	crypto_box_keypair(pk, sk);
	vector<uint8_t> firstmsg(crypto_box_ZEROBYTES);
	firstmsg.push_back(5);
	vector<uint8_t> ciphertext(firstmsg.size());
	vector<uint8_t> nonce(crypto_box_NONCEBYTES);
	
	crypto_box(ciphertext.data(), firstmsg.data(), firstmsg.size(), nonce.data(), pk ,sk); 
	/*	
	mock_pbook_hub hub;
	keypair c1key;
	mock_pbook_connection c1net(hub, c1key.pk());
	keypair c2key;
	mock_pbook_connection c2net(hub, c2key.pk());

	keypair s;  
	client c1(c1net, s.pk());
	client c2(c2net, s.pk());
	scoped_connection c1c(c1.onmsg.connect(m));
	contact contact1 {c1key.pk(), "Client 1"};
	c2.sendinstantmessage(contact1, "I'm bored hmu");
	cout << "Client 1:" << c1 <<"\n";
	
	publickey k1 = s.pk();
	publickey k2 = s.pk();
	cout << "Compare:" << (k1 < k2) << endl;
	*/
	return 0;
}
