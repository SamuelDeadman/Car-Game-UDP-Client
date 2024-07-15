#include <string>
#include <SFML/Network.hpp>
#pragma once
class tcp_client
{
public:
	tcp_client(std::string ip);
	void tcp_message(std::string message);
	void tcp_recive();
private:
	sf::TcpSocket socket;
};

