#ifndef __CLIENT_H
#define __CLIENT_H
#include <vector>
#include <string>
#include <boost/signals2/signal.hpp>
#include "network.h"
#include "crypto.h"

using namespace boost::signals2;

struct contact
{
	shared_ptr<publickey> pk;
	string nickname;
};


class client
{
	public:
		client(pbook_connection &net, shared_ptr<publickey> serverkey);
		void add_contact(contact c);
		vector<contact> contacts();
		void rxmsg(shared_ptr<pbook_message> msg);
		void sendinstantmessage(contact c, string message);
		friend ostream& operator<<(ostream& os, const client& c);
		signal<void (const string&)> onmsg;
	private:
		pbook_connection& m_network;
		scoped_connection m_signalconnection;
};

#endif
