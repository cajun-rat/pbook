#include <string>
#include "client.h"
#include "messages.pb.h"

client::client(pbook_connection &net, shared_ptr<publickey>) :
	m_network(net),
	m_signalconnection(
		net.pbook_message_rx.connect(
			boost::bind(&client::rxmsg, this, _1))) {
	shared_ptr<pbook_message> msg = make_shared<pbook_message>();
	hello *h = msg->data.mutable_hello();
	h->set_nickname("phil");
	m_network.send_pbook_message(msg);
}

void client::sendinstantmessage(contact c, string text) {
	shared_ptr<pbook_message> msg = make_shared<pbook_message>();
	instantmessage *im = msg->data.mutable_instantmessage();
	im->set_message(text);
	msg->destination = c.pk;
	m_network.send_pbook_message(msg);
}

void client::rxmsg(shared_ptr<pbook_message> msg) {
	cout << "client: got msg " << msg->data.ShortDebugString() << "\n";
	if (msg->data.has_instantmessage()) {
		onmsg(msg->data.instantmessage().message());
	}
}

ostream& operator<<(ostream& os, const client&) {
	os << "client()";
	return os;
}

