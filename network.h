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

struct udp_datagram
{
	string data;
	address sender;
	address destination;
};

class udp_connection
{
	public:
		virtual void send_udp(shared_ptr<udp_datagram> datagram) = 0;
		signal<void (shared_ptr<udp_datagram>)> udp_rx;
};

class pbook_message
{
	public:
		any_message data;
		shared_ptr<keypair> sender;
		publickey destination;
};

class pbook_connection
{
	public:
		virtual void send_pbook_message(shared_ptr<pbook_message> message) = 0;
		signal<void (shared_ptr<pbook_message>)> pbook_message_rx;
};

/* An entry in the ARP cache. Will include expiry times */
struct arpentry {
	address addr;
};

class networkarpencryptor : pbook_connection
{
	public:
		networkarpencryptor(udp_connection &net);
		virtual void send_pbook_message(shared_ptr<pbook_message> message);
	private:
		udp_connection &m_net;
		list<shared_ptr<pbook_message> > m_awaitingarp;
		map<publickey, arpentry> m_arpentries;
};	

/* For testing. Could also do NAT simulation */
class mock_internet_endpoint;

class mock_internet 
{
	friend class mock_internet_endpoint;
	private:
		set<mock_internet_endpoint*> connections;
};

class mock_internet_endpoint : public udp_connection 
{
	public:
		mock_internet_endpoint(mock_internet &net, address ip);
		~mock_internet_endpoint();
		virtual void send_udp(shared_ptr<udp_datagram> datagram);
	private:
		mock_internet& m_net;
		address m_ip;
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
		mock_pbook_connection(mock_pbook_hub &hub, publickey user);
		~mock_pbook_connection();
		virtual void send_pbook_message(shared_ptr<pbook_message> message);
		publickey user;
	private:
		mock_pbook_hub& m_hub;
};

#endif
