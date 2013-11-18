#ifndef __CLIENT_H
#define __CLIENT_H
#include <vector>
#include <string>

#include "network.h"
#include "crypto.h"

struct contact
{
	shared_ptr<publickey> pk;
	string nickname;
};


class client : public network_endpoint
{
	public:
		client(network &net, shared_ptr<publickey> serverkey);
		~client();
		void add_contact(contact c);
		vector<contact> contacts();
		void sendinstantmessage(contact c, string message);
		/* network_endpoint */
		virtual void rxmsg(string msg, address sender);
		friend ostream& operator<<(ostream& os, const client& c);
	private:
		network& m_network;
};

#endif
