#include "UdpClient.h"
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

UdpClient::UdpClient()
{
	socket_ptr = std::make_shared<sf::UdpSocket>();//shared pointer as we can't share sockets - "smart pointer" CLASS THIS !!!!!!!!!!!!!!!! 
	socket_ptr->setBlocking(false); //blocking false, on recive now its not blocking :)
	//because this is not blocking thread is not needed
	//blocking will allow for updates to be missed but also not slow down the game waiting for input
	//if blocking causes an issue use a "std::thread"
	std::cout << "Socket created";

	UDP_get();
}



int UdpClient::UDP_Send(float x, float y, float z)
{
	
	//sf::IpAddress local(127, 0, 0, 1); //send to IP
	sf::IpAddress local(UDP_serverip); //send to IP
	sf::Socket::Status status; // the status of the socket
	
	char buffer[BUFFER_SIZE]; //GAME DATA !
	std::memset(buffer, 0, BUFFER_SIZE);
	memcpy(buffer, "HELLO", 6); //HELLO is example data and used to verify that the data has been send and recvied corretly 
	memcpy(buffer + 6, &x, sizeof(x));
	memcpy(buffer + 6 + sizeof(x), &y, sizeof(y));
	memcpy(buffer + 6 + sizeof(x) + sizeof(y), &z, sizeof(z)); //sender code
	//socket_ptr->send();

	status = socket_ptr->send(buffer, sizeof(buffer), local, 50001);
	if (status != sf::Socket::Done)
	{
		std::cerr << "ERROR UDP-Client socket sending ERROR Line 56\n";
		return 2;
	}
	//std::cout << "sent out: \"" << x << "\"" << y << "\"" << std::endl; // X Y debug out
	return 0;
}

int UdpClient::UDP_Recive(float* x, float* y, float* z, unsigned int *IP4, unsigned short *port)
{
	sf::Socket::Status status;
	const std::size_t bufferSize = 256;
	char buffer[bufferSize];
	std::size_t received;
	sf::IpAddress Serveraddress;
	unsigned short Serverport;

	status = socket_ptr->receive((void*)buffer, bufferSize, received, Serveraddress, Serverport); //in send package address and port in front
	if ((status != sf::Socket::Done) && (status != sf::Socket::Disconnected) && (status != sf::Socket::NotReady)) // only exit on unexpected error
	{
		std::cout << "reciver loop exited " << status << std::endl;
		return -1; // ERROR !
	}
	if (status == sf::Socket::Done) // only set / update if recived
	{
		float t = 0;//temp padding for the mem copy
		int sizet = sizeof(t);
		memcpy(x, buffer + 6, sizet); // thsi unpacks and sets the values by what has been recived 
		memcpy(y, buffer + 6 + sizet, sizet);
		memcpy(z, buffer + 6 + sizet + sizet, sizet); 
		memcpy(IP4, buffer + 6 + sizet + sizet + sizet, 4);
		memcpy(port, buffer + 6 + sizet + sizet + sizet + 4, 2); //reciver code USE PORT AT KEY IN STORAGE LAYER
		return 0; //success !
	}
	
	return 1; // nothing recived
}

void UdpClient::UDP_get()
{
	std::string s; //string to coantain user input of there ip
	std::cout << "type in server IP address, use 127.0.0.1 for local host: ";
	std::cin >> s;
	if (s == "") // this does not work for some reaon ?
	{
		s = "127.0.0.1";
	}

	UDP_serverip = s;
}

