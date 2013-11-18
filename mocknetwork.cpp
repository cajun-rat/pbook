#include "network.h"

mockinternetconnection::mockinternetconnection(mockinternet& net, address clientip)
	: m_network(net), m_clientip(clientip) { }

void mockinternetconnection::sendmsg(string msg, shared_ptr<publickey> pk) {
	int found = 0;
	set<network_endpoint*> handlers = m_network.m_handlers;
	for (set<network_endpoint*>::iterator h = handlers.begin(); h != handlers.end(); ++h) {
		network_endpoint* mh = *h;
		if (mh->key().pk() == pk) {
			found++;
			mh->rxmsg(msg, m_clientip);
		} 
	}
	cout << "mockinternet: sent to " << found << " people\n";
}

void mockinternetconnection::addmessagehandler(network_endpoint* handler) {
	m_network.m_handlers.insert(handler);
}

void mockinternetconnection::removemessagehandler(network_endpoint* handler) {
	int rmd = m_network.m_handlers.erase(handler);
	if (rmd != 1) cout << "Warning nothing removed\n";
}


