/* Network.h - Networking abstraction.

This provides an abstraction over the UDP/IP networking, and allows mock
network implementations to be used for NAT traversal testing. */

#include <boost/asio/ip/address.hpp>
#include <set>
#include <string>

using namespace std;
using namespace boost::asio::ip;

/* A network endpoint maps one-to-one with a public/private key pair. */
class network_endpoint
{
	public:
	virtual void rxmsg(string msg, address sender) = 0;
	virtual string pk() = 0;
	virtual string sk() = 0;
};

/* A network abstracts the IP/UDP transport layer, and provides the ability
to send a datagram securely to a given public key. It includes encryption and
and ARP */

class network
{
	public:
		virtual void sendmsg(string msg, string pk) = 0;
		virtual void addmessagehandler(network_endpoint* handler) = 0;
		virtual void removemessagehandler(network_endpoint* handler) = 0;
};

class mocknetwork 
{
	public:
		set<network_endpoint*> m_handlers;
};

class mocknetworkendpoint : public network
{
	public:
		mocknetworkendpoint(mocknetwork& net, address clientip);
		virtual void sendmsg(string msg, string pk);
		virtual void addmessagehandler(network_endpoint* handler);
		virtual void removemessagehandler(network_endpoint* handler);	
	private:
		mocknetwork& m_network;
		address m_clientip;
};

