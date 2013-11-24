#include "network.h"

mock_internet_endpoint::mock_internet_endpoint(mock_internet &net, address ip) :
	m_net(net),
	m_ip(ip) {
	m_net.connections.insert(this);
}

mock_internet_endpoint::~mock_internet_endpoint() {
	m_net.connections.erase(this);
}

void mock_internet_endpoint::send_udp(shared_ptr<udp_datagram> datagram) {
	set<mock_internet_endpoint*> &conns = m_net.connections;

	int found = 0;
	for (set<mock_internet_endpoint*>::iterator h = conns.begin(); h!= conns.end(); ++h) {
		mock_internet_endpoint *c = *h;
		if (c->m_ip == datagram->destination) {
			found++;
			c->udp_rx(datagram);
		}
	}
	cout << "mock_internet_endpoint sent to " << found << " people" << endl;
}


mock_pbook_connection::mock_pbook_connection(mock_pbook_hub &hub, shared_ptr<publickey> user_) :
	user(user_), m_hub(hub) {
	m_hub.connections.insert(this);
}

mock_pbook_connection::~mock_pbook_connection() {
	m_hub.connections.erase(this);
}

void mock_pbook_connection::send_pbook_message(shared_ptr<pbook_message> message) {
	set<mock_pbook_connection*> &conns = m_hub.connections;

	int found = 0;
	for (set<mock_pbook_connection*>::iterator h = conns.begin(); h!= conns.end(); ++h) {
		mock_pbook_connection *c = *h;
		if (c->user == message->destination) {
			found++;
			c->pbook_message_rx(message);
		}
	}
	cout << "mock_pbook_connection sent to " << found << " people" << endl;
}

