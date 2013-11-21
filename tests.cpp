#include "network.h"
#include "client.h"
#include "server.h"

#define BOOST_TEST_MODULE simple_tests
#include <boost/test/unit_test.hpp>

struct msg_grabber {
	string msg;
	void operator()(const string m) {
		msg = m;
	}
};

BOOST_AUTO_TEST_CASE (simple_im) {
	mock_pbook_hub hub;
	keypair c1key;
	mock_pbook_connection c1net(hub, c1key.pk());
	keypair c2key;
	mock_pbook_connection c2net(hub, c2key.pk());

	keypair s; /* unused */ 
	client c1(c1net, s.pk());
	client c2(c2net, s.pk());
	msg_grabber lastmsg;
	scoped_connection c1c(c1.onmsg.connect(std::ref(lastmsg)));
	contact contact1 {c1key.pk(), "Client 1"};
	c2.sendinstantmessage(contact1, "I'm bored hmu");
	BOOST_CHECK_EQUAL(lastmsg.msg, "I'm bored hmu");
}

