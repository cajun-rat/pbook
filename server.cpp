#include <iostream>

#include "network.h"
#include "server.h"
#include "messages.pb.h"


using namespace std;

server::server(udp_connection &net, keypair key) 
	: m_network(net),
	m_key(key),
	m_handle_msg_connection(net.udp_rx.connect(bind(&server::handle_msg,this, _1))) {
}

void server::handle_msg(shared_ptr<udp_datagram> datagram) {
	cout << "Got message in server" << endl;
	tuple<string,publickey> msg;
	decryptresult res = m_key->decrypt(datagram->data, msg);
	if (res == decryptresult::Success) {
		cout << "Got message from " << get<1>(msg) << "\n";
		// TODO
	} else {
		cout << "decrypt failed in server:" << res << "\n";
	}
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
