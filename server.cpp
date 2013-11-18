#include <iostream>

#include "network.h"
#include "server.h"
#include "messages.pb.h"


using namespace std;

server::server(network &net) : m_network(net) {
	m_network.addmessagehandler(this);
}

server::~server() {
	m_network.removemessagehandler(this);
}

void server::rxmsg(string msgdata, address sender) {
	client_to_server msg;
	msg.ParseFromString(msgdata);
	cout << "server: got msg " << msg.ShortDebugString() << "\n";
	if (msg.has_hello()) {
		hello h(msg.hello()); 
		cout << "server: got hello from " << h.publickey() 
									<< " ip " << sender<< "\n";
		m_lastknownaddress[h.publickey()] = sender;
	}
	if (msg.has_whereis()) {
		whereis w(msg.whereis());
		cout << "server: got whois from " << w.tell() << 
									" tell " << w.who() << "\n";
	}
}



