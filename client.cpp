#include <string>
#include "client.h"
#include "messages.pb.h"

client::client(network &net) : m_network(net) {
	string ser;
	net.addmessagehandler(this); 
	client_to_server hellomsg;
	hello *h = hellomsg.mutable_hello();
	h->set_publickey(pk());
	h->set_nickname("phil");
	hellomsg.SerializeToString(&ser);
	net.sendmsg(ser, "todoserverpk");
}

client::~client() {
	m_network.removemessagehandler(this);
}

void client::rxmsg(string msg, address sender) {}

