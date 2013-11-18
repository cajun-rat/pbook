#ifndef __SERVER_H
#define __SERVER_H

#include <map>
#include "crypto.h"

class server : public network_endpoint
{
	public:
		server(network &net);
		~server();
		virtual void rxmsg(string msg, address sender);
	private:
		network& m_network;
		map<string, address> m_lastknownaddress;
};

#endif
