#ifndef __CLIENT_H
#define __CLIENT_H
#include <vector>
#include <string>

#include "network.h"

class contact
{
	string pk;
	string nickname;
};


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

#endif
