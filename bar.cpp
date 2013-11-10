#include <iostream>
#include <fstream>
#include <list>
#include <memory>
#include <vector>
#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/asio/ip/address.hpp>

#include "messages.pb.h"
#include "network.h"

using namespace boost;
using namespace std;
using namespace boost::asio::ip;

class contact
{
	string pk;
	string nickname;
};

class server : public network_endpoint
{
	public:
		server(network &net);
		~server();
		virtual string pk() { return "todoserverpk"; }
		virtual string sk() { return "todoserversk"; }
		virtual void rxmsg(string msg, address sender);
	private:
		network& m_network;
		map<string, address> m_lastknownaddress;
};

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
		cout << "server: got hello from " << h.publickey() << " ip " << sender<< "\n";
		m_lastknownaddress[h.publickey()] = sender;
	}
	if (msg.has_whereis()) {
		whereis w(msg.whereis());
		cout << "server: got whois from " << w.tell() << " tell " << w.who() << "\n";
	}
}

class client : public network_endpoint
{
	public:
		client(network &net);
		~client();
		void add_contact(contact c);
		vector<contact> contacts();
		virtual string pk() { return "todoclientpk"; }
		virtual string sk() { return "todoclientsk"; }
		virtual void rxmsg(string msg, address sender);
	private:
		network& m_network;
};

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

int main() {
	mocknetwork net;
	mocknetworkendpoint servernet(net, address::from_string("192.168.0.1"));
	mocknetworkendpoint clientnet(net, address::from_string("192.168.0.2"));

	server s(servernet);
	client c(clientnet);		
	return 0;
}
