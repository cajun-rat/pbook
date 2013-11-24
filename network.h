#ifndef __NETWORK_H
#define __NETWORK_H
/* Network.h - Networking abstraction.

This provides an abstraction over the UDP/IP networking, and allows mock
network implementations to be used for NAT traversal testing. */

#include <boost/asio/ip/address.hpp>
#include <boost/signals2/signal.hpp>
#include <set>
#include <string>
#include "crypto.h"
#include "messages.pb.h"

using namespace std;
using namespace boost::asio::ip;
using namespace boost::signals2;

class udp_datagram
{
	string data;
	address sender;
	address destination;
};

class udp_connection
{
	virtual void send_udp(shared_ptr<udp_datagram> datagram) = 0;
	signal<void (shared_ptr<udp_datagram>)> udp_rx;
};

class pbook_message
{
	public:
		any_message data;
		shared_ptr<keypair> sender;
		shared_ptr<publickey> destination;
};

class pbook_connection
{
	public:
		virtual void send_pbook_message(shared_ptr<pbook_message> message) = 0;
		signal<void (shared_ptr<pbook_message>)> pbook_message_rx;
};

class networkarpencryptor : pbook_connection
{
	public:
		networkarpencryptor(udp_connection &net);
		virtual void send_pbook_message(shared_ptr<pbook_message> message);
	private:
		udp_connection &m_net;
};	

/* For testing. Could also do NAT simulation */
class mockinternetendpoint;

class mockinternet 
{
	set<mockinternetendpoint*> connections;
};

class mockinternetendpoint : public udp_connection 
{
	mockinternetendpoint(mockinternet &net, address ip);
	void send_udp(shared_ptr<udp_datagram> datagram);
};

class mock_pbook_connection;
class mock_pbook_hub
{
	friend class mock_pbook_connection;
	private:
		set<mock_pbook_connection*>	connections;
		
};

class mock_pbook_connection : public pbook_connection
{
	public:
		mock_pbook_connection(mock_pbook_hub &hub, shared_ptr<publickey> user);
		~mock_pbook_connection();
		virtual void send_pbook_message(shared_ptr<pbook_message> message);
		shared_ptr<publickey> user;
	private:
		mock_pbook_hub& m_hub;
};

#endif
