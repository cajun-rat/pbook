#ifndef __SERVER_H
#define __SERVER_H

#include <map>
#include "crypto.h"

class server
{
	public:
		server(udp_connection &net);
	private:
		udp_connection& m_network;
		map<string, address> m_lastknownaddress;
};

#endif
