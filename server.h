#ifndef __SERVER_H
#define __SERVER_H

#include <map>

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


#endif
