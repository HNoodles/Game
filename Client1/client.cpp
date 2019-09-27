#include <zmq.hpp>
#include <string>
#include <iostream>

constexpr auto CLIENT_NAME = "A";

using namespace std;
using namespace zmq;

string s_recv(socket_t& socket);
void s_send(socket_t& socket, const string& string);

int main()
{
	//  Prepare our context and socket
	context_t context(1);

	socket_t sender(context, ZMQ_REQ);
	sender.connect("tcp://localhost:5555");
	cout << "Connecting to server on port 5555..." << endl;

	socket_t subscriber(context, ZMQ_SUB);
	subscriber.connect("tcp://localhost:5556");
	subscriber.setsockopt(ZMQ_SUBSCRIBE, "C", strlen("C"));
	cout << "Subscribing to server on port 5556..." << endl;

	// send name to connect to server
	s_send(sender, CLIENT_NAME);
	string response = s_recv(sender);

	// keep printing out published messages
	while (true)
	{
		cout << s_recv(subscriber);
	}

	return 0;
}

//  Receive 0MQ string from socket and convert into string
static string s_recv(socket_t& socket)
{
	message_t message;
	socket.recv(message);

	return string(static_cast<char*>(message.data()), message.size());
}

//  Convert string to 0MQ string and send to socket
static void s_send(socket_t& socket, const string& string)
{
	message_t message(string.size());
	memcpy(message.data(), string.data(), string.size());

	socket.send(message, send_flags::none);
}