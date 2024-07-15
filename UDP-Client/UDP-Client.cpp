#include "receiver.hpp"
#include "util.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <tuple>
#include <sstream>
#include <stdio.h>
#include <tuple>

#define BUFFER_SIZE (256) // SET BUFFER SIZE AT 256 

int UDP_Client() //legacy UDP version not used by game
{
	sf::IpAddress local(127, 0, 0, 1); //send to IP
	sf::Socket::Status status; // the status of the socket

	std::shared_ptr<sf::UdpSocket> socket_ptr = std::make_shared<sf::UdpSocket>(); //shared pointer as we can't share sockets - "smart pointer" CLASS THIS !!!!!!!!!!!!!!!!
	//auto connect = [&] {return socket->connect(sf::IpAddress::getLocalAddress(), PORT); };
	//net_run(connect, "Connect");
	std::cout << "Socket created";
	//Queue<std::string> queue;
	//Receiver r{ socket, queue };
	//std::thread{ &Receiver::recv_loop, &r}.detach();
	// Sending a "register" message

	//std::string s;
	//std::cout << "Please type in your user name: "; 
	//std::cin >> s;
	// TODO: send the registration message consisting of the user name.
	// END TODO: send the registration message consisting of the user name.

	char buffer[BUFFER_SIZE]; //GAME DATA !
	memcpy(buffer, "HELLO", 6);

	//socket_ptr->send();

	status = socket_ptr->send(buffer, sizeof(buffer), local, 50001);
	if (status != sf::Socket::Done)
	{
		std::cerr << "ERROR UDP-Client socket sending ERROR Line 41\n";
		return 2;
	}

	//Queue<message> queue;
	//Receiver r{ socket_ptr, queue };
	// TODO: launch a receiver thread
	// END TODO: launch a receiver thread
	while (true)
	{
		//s = "";
		//std::getline(std::cin, s);
		//std::cout << "Sending: \"" << s << "\"" << std::endl;
		// TODO: send the message to the server
		// END TODO: send the message to the server
	}
	return 0;
}
