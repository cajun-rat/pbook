#ifndef __NETWORK_H
#define __NETWORK_H
/* Network.h - Networking abstraction.

This provides an abstraction over the UDP/IP networking, and allows mock
network implementations to be used for NAT traversal testing. */

#include <boost/asio/ip/address.hpp>
#include <set>
#include <string>
#include "crypto.h"

using namespace std;
using namespace boost::asio::ip;

/* A network endpoint maps one-to-one with a public/private key pair. 
Classes that want to talk to the network will inheirit from this. */

class network_endpoint
{
	public:
		virtual void rxmsg(string msg, address sender) = 0;
		virtual const keypair& key() const { return m_key; }
		virtual shared_ptr<publickey> pk() const { return key().pk(); }
	protected:
		keypair m_key;
};

/* A network abstracts the IP/UDP transport layer, and provides the ability
to send a datagram securely to a given public key. It includes encryption and
and ARP */

class network
{
	public:
		virtual void sendmsg(string msg, shared_ptr<publickey> destination) = 0;
		virtual void addmessagehandler(network_endpoint* handler) = 0;
		virtual void removemessagehandler(network_endpoint* handler) = 0;
};

/* mockinternet -> mockinternet 
mockinternetconnection -> mockinternetconnection
*/
class mockinternet 
{
	public:
		set<network_endpoint*> m_handlers;
};

class mockinternetconnection : public network
{
	public:
		mockinternetconnection(mockinternet& net, address clientip);
		virtual void sendmsg(string msg, shared_ptr<publickey> pk);
		virtual void addmessagehandler(network_endpoint* handler);
		virtual void removemessagehandler(network_endpoint* handler);	
	private:
		mockinternet& m_network;
		address m_clientip;
};

#endif
