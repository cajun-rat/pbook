#include <string>
#include "client.h"
#include "messages.pb.h"

client::client(network &net, shared_ptr<publickey> serverkey) : m_network(net) {
	net.addmessagehandler(this); 
	client_to_server hellomsg;
	hello *h = hellomsg.mutable_hello();
	h->set_publickey(key().pk()->bytes());
	h->set_nickname("phil");

	string ser;
	hellomsg.SerializeToString(&ser);
	net.sendmsg(ser, serverkey);
}

client::~client() {
	m_network.removemessagehandler(this);
}

void client::sendinstantmessage(contact c, string text) {
	anything_to_client m;
	instantmessage *im = m.mutable_instantmessage();
	im->set_message(text);

	string ser;
	m.SerializeToString(&ser);
	m_network.sendmsg(ser, c.pk);
}


void client::rxmsg(string msgdata, address) {
	anything_to_client m;
	m.ParseFromString(msgdata);
	cout << "client: got msg " << m.ShortDebugString() << "\n";
	if (m.has_instantmessage()) {
		cout << "im: " << m.instantmessage().message() << "\n";
	}
}

ostream& operator<<(ostream& os, const client& c) {
	os << "client(" << c.pk() << ")";
	return os;
}

