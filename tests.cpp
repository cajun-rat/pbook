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
	mockinternet net;
	mockinternetconnection servernet(net, address::from_string("192.168.0.1"));
	mockinternetconnection client1net(net, address::from_string("192.168.0.2"));
	mockinternetconnection client2net(net, address::from_string("192.168.0.3"));

	server s(servernet);
	client c1(client1net, s.pk());
	client c2(client2net, s.pk());
	msg_grabber lastmsg;
	scoped_connection c1c(c1.onmsg.connect(std::ref(lastmsg)));
	contact contact1 {c1.key().pk(), "Client 1"};
	c2.sendinstantmessage(contact1, "I'm bored hmu");
	BOOST_CHECK_EQUAL(lastmsg.msg, "I'm bored hmu");
}
