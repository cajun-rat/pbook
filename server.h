#ifndef __SERVER_H
#define __SERVER_H

#include <map>
#include "crypto.h"
#include <boost/signals2/signal.hpp>

class server
{
	public:
		server(udp_connection &net);
	private:
		udp_connection& m_network;
		map<string, address> m_lastknownaddress;
		void handle_msg(shared_ptr<udp_datagram> msg);
		boost::signals2::scoped_connection m_handle_msg_connection;
};

#endif
