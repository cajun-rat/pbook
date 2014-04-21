#include "network.h"
#include "client.h"
#include "server.h"

#define BOOST_TEST_MODULE simple_tests
#include <boost/test/unit_test.hpp>
#include <boost/asio/ip/address.hpp>

using namespace boost::asio::ip;
using namespace boost::signals2;
struct msg_grabber {
	string msg;
	void operator()(const string m) {
		msg = m;
	}
};

BOOST_AUTO_TEST_CASE (crypto) {
	keypairdata sender;
	keypairdata receiver;

	string msg = "hello";

	string ciphertext = sender.encrypt(receiver.pk(), msg);
	tuple<string,publickey> x = receiver.decrypt(ciphertext);

	BOOST_CHECK_EQUAL(get<0>(x), msg);
}

BOOST_AUTO_TEST_CASE (simple_im) {
	mock_pbook_hub hub;
	keypair c1key = make_shared<keypairdata>();
	mock_pbook_connection c1net(hub, c1key->pk());
	keypair c2key = make_shared<keypairdata>();
	mock_pbook_connection c2net(hub, c2key->pk());

	keypair s = make_shared<keypairdata>(); 
	client c1(c1net, s->pk());
	client c2(c2net, s->pk());
	msg_grabber lastmsg;
	scoped_connection c1c(c1.onmsg.connect(std::ref(lastmsg)));
	contact contact1 {c1key->pk(), "Client 1"};
	c2.sendinstantmessage(contact1, "I'm bored hmu");
	BOOST_CHECK_EQUAL(lastmsg.msg, "I'm bored hmu");
}

BOOST_AUTO_TEST_CASE (internet_im) {
	mock_internet hub;
	mock_internet_endpoint serverep(hub, address::from_string("192.168.0.1"));
	mock_internet_endpoint c1ep(hub, address::from_string("192.168.0.2"));
	mock_internet_endpoint c2ep(hub, address::from_string("192.168.0.3"));
	keypair skey = make_shared<keypairdata>();
	server s(serverep, skey);
	keypair c1key = make_shared<keypairdata>();
	keypair c2key = make_shared<keypairdata>();
	cout << "server is " << *skey->pk() << endl;
	cout << "client 1 is " << *c1key->pk() << endl;
	cout << "client 2 is " << *c2key->pk() << endl;
	networkarpencryptor c1net(c1ep, c1key, skey->pk(), address::from_string("192.168.0.1"), 1234);
	networkarpencryptor c2net(c2ep, c2key, skey->pk(), address::from_string("192.168.0.1"), 1234);

	client c1(c1net, skey->pk());
	client c2(c2net, skey->pk());

	msg_grabber lastmsg;
	scoped_connection c1c(c1.onmsg.connect(std::ref(lastmsg)));
	contact contact1 {c1key->pk(), "Client 1"};
	// If we send an IM to someone...
	c2.sendinstantmessage(contact1, "I'm bored hmu");
	// ... they receive it
	BOOST_CHECK_EQUAL(lastmsg.msg, "I'm bored hmu");
}

