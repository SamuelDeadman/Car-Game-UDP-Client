#pragma once
#include <SFML/Network.hpp>
class UdpClient
{
public:
	UdpClient();//constrcutor
	int UDP_Send(float x, float y, float z);
	int UDP_Recive(float *x, float *y, float *z, unsigned int* IP4, unsigned short* port);
	std::shared_ptr<sf::UdpSocket> socket_ptr; // smart pointer destroyed/deallocated when last object is destroyed;
	void UDP_get();
	std::string UDP_serverip;
private:
	
};

