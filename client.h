#ifndef __CLIENT_H
#define __CLIENT_H
#include <vector>
#include <string>
#include <boost/signals2/signal.hpp>
#include "network.h"
#include "crypto.h"

struct contact
{
	contact(publickey pk, string nickname);
	publickey pk;
	string nickname;
};

/* The top-level interface to pbook, to be used by a GUI */
class client
{
	public:
		client(pbook_connection &net, publickey serverkey);
		void add_contact(contact c);
		vector<contact> contacts();
		void rxmsg(shared_ptr<pbook_message> msg);
		void sendinstantmessage(contact c, string message);
		friend ostream& operator<<(ostream& os, const client& c);
		boost::signals2::signal<void (const string&)> onmsg;
	private:
		pbook_connection& m_network;
		publickey m_serverkey;
		boost::signals2::scoped_connection m_signalconnection;
};

#endif
