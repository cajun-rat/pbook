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
//using namespace boost::signals2;

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
		boost::signals2::signal<void (shared_ptr<udp_datagram>)> udp_rx;
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
		boost::signals2::signal<void (shared_ptr<pbook_message>)> pbook_message_rx;
};
class peer_location;
class networkarpencryptor : public pbook_connection
{
	public:
		// net - The upstream connection
		// user - The key to encrypt/decrypt communication
		// arp* - The location of the arp server 
		networkarpencryptor(udp_connection &net, shared_ptr<keypair> user,
			publickey arpkey, address arpaddress, int arpport);
		virtual void send_pbook_message(shared_ptr<pbook_message> message);
	private:
		udp_connection &m_net;
		map<publickey, peer_location> m_arpentries;
		shared_ptr<keypair> m_userkey;
		publickey m_arpkey;
		address m_arpaddress;
		int m_arpport;
};	

/* An entry in the ARP cache. Will include expiry times */
class peer_location {
	public:
		peer_location() : m_got_address(false) {}
		bool got_address() { return m_got_address;}
		address addr;
		int port;
		vector<string> messages_queued;
	private:
		bool m_got_address;
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
