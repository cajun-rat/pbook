#include "network.h"
#include "client.h"
#include "server.h"

using namespace std;
using namespace boost::asio::ip;

int main() {
	mockinternet net;
	mockinternetconnection servernet(net, address::from_string("192.168.0.1"));
	mockinternetconnection client1net(net, address::from_string("192.168.0.2"));
	mockinternetconnection client2net(net, address::from_string("192.168.0.3"));

	server s(servernet);
	client c1(client1net, s.pk());
	client c2(client2net, s.pk());

	contact contact1 {c1.key().pk(), "Client 1"};
	c2.sendinstantmessage(contact1, "I'm bored hmu");

	cout << "Client 1:" << c1 <<"\n";

	return 0;
}
