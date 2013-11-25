#include "network.h"
#include "client.h"
#include "server.h"

using namespace std;
using namespace boost::asio::ip;

void m(const string msg) {
	cout << "GOT MSG:" << msg << endl;
}

int main() {
	mock_pbook_hub hub;
	keypair c1key;
	mock_pbook_connection c1net(hub, c1key.pk());
	keypair c2key;
	mock_pbook_connection c2net(hub, c2key.pk());

	keypair s; /* unused */ 
	client c1(c1net, s.pk());
	client c2(c2net, s.pk());
	scoped_connection c1c(c1.onmsg.connect(m));
	contact contact1 {c1key.pk(), "Client 1"};
	c2.sendinstantmessage(contact1, "I'm bored hmu");
	cout << "Client 1:" << c1 <<"\n";
	
	publickey k1 = s.pk();
	publickey k2 = s.pk();
	cout << "Compare:" << (k1 < k2) << endl;
	return 0;
}
