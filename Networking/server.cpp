#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include <thread>

using namespace std;
using namespace zmq;


typedef struct {
	string name;
	int iteration;
} Client;

void publisherHandler(socket_t* publisher, vector<Client>* clients);
string s_recv(socket_t& socket);
void s_send(socket_t& socket, const string& string);
string Message(int num, int iter);

int main() 
{
	//  Prepare our context and socket
	context_t context(1);

	socket_t receiver(context, ZMQ_REP);
	receiver.bind("tcp://*:5555");
	cout << "Receiver started, listening for clients on port 5555..." << endl;

	socket_t publisher(context, ZMQ_PUB);
	publisher.bind("tcp://*:5556");
	cout << "Publisher started, publishing messages on port 5556..." << endl;

	vector<Client> clients;

	// create new thread to handle publishing
	thread newThread(publisherHandler, &publisher, &clients);
	newThread.detach();

	while (true)
	{
		// listen from clients
		string client_name = s_recv(receiver);

		// new client
		Client client = { client_name, 1 };
		clients.emplace_back(client);
		cout << "New client " + client_name << endl;

		// send a response to fulfill a come and go
		s_send(receiver, "success");
	}

	return 0;
}

void publisherHandler(socket_t* publisher, vector<Client>* clients)
{
	// keep publishing current state of connection
	while (true)
	{
		// publish current message
		string message = "";
		for (size_t i = 0; i < clients->size(); i++)
		{
			message += Message(i + 1, (*clients)[i].iteration++);
		}
		s_send(*publisher, message + "\n");
		// cout << message << endl;

		// rest for 5 seconds
		Sleep(5000);
	}
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

string Message(int num, int iter) 
{ 
	return "Client " + to_string(num) + ": Iteration " + to_string(iter) + "\n";
}