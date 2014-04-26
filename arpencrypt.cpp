#include "network.h"
#include "messages.pb.h"
#include "crypto.h"
#include <iostream>
#include <cassert>

using namespace std;

networkarpencryptor::networkarpencryptor(udp_connection &net, keypair user,
		publickey arpkey, address arpaddress, int arpport) :
	m_net(net),
	m_userkey(user),
	m_arpkey(arpkey),
	m_arpaddress(arpaddress),
	m_arpport(arpport) {
}

void networkarpencryptor::send_pbook_message(shared_ptr<pbook_message> message) {
	assert(message->destination.get() != nullptr);
	message->sender = m_userkey;
	string plaintext;
	message->data.SerializeToString(&plaintext);
	string ciphertext = message->sender->encrypt(message->destination, plaintext);

	peer_location& peer = m_arpentries[message->destination];
	if (peer.got_address()) {
		cout << "Already have address for peer" << endl;
		shared_ptr<udp_datagram> packet = make_shared<udp_datagram>();
		packet->data = ciphertext;
		packet->destination = peer.addr;
		m_net.send_udp(packet);
	} else {
		cout << "Performing Arp" << endl;
		peer.messages_queued.push_back(ciphertext);
		// Send arp
		any_message arpmsg;
		whereis *arprequest = arpmsg.mutable_whereis();
		arprequest->set_who(message->destination->bytes()); 
		string plaintext;
		arpmsg.SerializeToString(&plaintext);
		string ciphertext(message->sender->encrypt(m_arpkey, plaintext));
		shared_ptr<udp_datagram> packet = make_shared<udp_datagram>();
		packet->data = ciphertext;
		packet->destination = m_arpaddress;
		m_net.send_udp(packet);
	}
}

