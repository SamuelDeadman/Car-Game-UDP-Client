#include "tcp_client.h"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <tuple>
#include <sstream>
#include <stdio.h>
#include <tuple>
#include <string>


tcp_client::tcp_client(std::string ip)
{
	socket.connect(ip, 60001);
	socket.setBlocking(false);
}

void tcp_client::tcp_message(std::string message)
{
	// Send a message to the connected host
	//std::string message = "HELLO sam new client made";
	sf::Packet packet;
	packet << message;//inbuild size work

	socket.send(packet);
	// Receive an answer from the server
	char buffer[1024];
	std::size_t received = 0;
	//socket.receive(buffer, sizeof(buffer), received);
	//std::cout << "The server said: " << buffer << std::endl;
}

void tcp_client::tcp_recive()
{
	sf::Packet packet;
	sf::Socket::Status status;
	status = socket.receive(packet);
	if (status == sf::Socket::Done)
	{
		std::string mess;
		packet >> mess;
		std::cout << "tcp client recived: " << mess << std::endl;
	}
}
