#include "network.h"
#include "messages.pb.h"
#include "crypto.h"

using namespace std;

networkarpencryptor::networkarpencryptor(udp_connection &net) :
	m_net(net) {
	/* TODO: send hello message */	
}

void send_pbook_message(shared_ptr<pbook_message> message) {
	

}

