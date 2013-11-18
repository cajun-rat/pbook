#ifndef __CLIENT_H
#define __CLIENT_H
#include <vector>
#include <string>

#include "network.h"
#include "crypto.h"

struct contact
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
		void sendinstantmessage(contact c, string message);
		/* network_endpoint */
		virtual string pk() { return "todoclientpk"; }
		virtual string sk() { return "todoclientsk"; }
		virtual void rxmsg(string msg, address sender);
		friend ostream& operator<<(ostream& os, const client& c);
	private:
		network& m_network;
		string m_pk;
		string m_sk; 
};

#endif
