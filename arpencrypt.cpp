#include "network.h"
#include "messages.pb.h"
#include "crypto.h"
#include <iostream>
#include <cassert>

using namespace std;

networkarpencryptor::networkarpencryptor(udp_connection &net, shared_ptr<keypair> user) :
	m_net(net),
	m_userkey(user) {
}

void networkarpencryptor::send_pbook_message(shared_ptr<pbook_message> message) {
	assert(message->destination.get() != nullptr);
	message->sender = m_userkey;
	string plaintext;
	message->data.SerializeToString(&plaintext);
	string ciphertext = message->sender->encrypt(message->destination, plaintext);

	peer_location& peer = m_arpentries[message->destination];
	peer.send_message(ciphertext);
}

void peer_location::send_message(string msg) {
	/* TODO */
	cout << "Needs arp:" << m_needs_arp << endl; 
	if (m_needs_arp) {
		// enqueue the message, we will send it when the arp response arrives
		m_messages_queued.push_back(msg);
		// Check for arp timeout
		// send an arp message
	} else {
		// TODO: 
	}
}

