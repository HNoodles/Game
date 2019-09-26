//
//  Hello World client in C++
//  Connects REQ socket to tcp://localhost:5555
//  Sends "Hello" to server, expects "World" back
//
#include <zmq.hpp>
#include <string>
#include <iostream>

constexpr auto CLIENT_NAME = "C";

using namespace std;
using namespace zmq;

string s_recv(socket_t& socket);
void s_send(socket_t& socket, const string& string);

int main()
{
	//  Prepare our context and socket
	context_t context(1);
	socket_t socket(context, ZMQ_REQ);

	cout << "Connecting to server¡­" << endl;
	socket.connect("tcp://localhost:5555");

	// keep sending and recieving messages
	while (true)
	{
		s_send(socket, CLIENT_NAME);
		cout << s_recv(socket);

		// rest for 5 seconds before next iteration
		Sleep(5000);
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

	socket.send(message, send_flags::dontwait);
}