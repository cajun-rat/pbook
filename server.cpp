#include <iostream>

#include "network.h"
#include "server.h"
#include "messages.pb.h"


using namespace std;

server::server(udp_connection &net) : m_network(net) {
}

/*
void server::rxmsg(string msgdata, address sender) {
	client_to_server msg;
	msg.ParseFromString(msgdata);
	cout << "server: got msg " << msg.ShortDebugString() << "\n";
	if (msg.has_hello()) {
		hello h(msg.hello()); 
		publickey pk(h.publickey());
		cout << "server: got hello from " << pk
									<< " ip " << sender<< "\n";
		m_lastknownaddress[h.publickey()] = sender;
	}
	if (msg.has_whereis()) {
		whereis w(msg.whereis());
		cout << "server: got whois from " << w.tell() << 
									" tell " << w.who() << "\n";
	}
}


*/
