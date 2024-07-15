#include "receiver.hpp"
#include "util.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <iostream>
#include <sstream>
#include <thread>
#include <tuple>


Receiver::Receiver(std::shared_ptr<sf::UdpSocket> s,
        Queue<message> &q):
    socket_(s),
    queue_(q)
{
}

void Receiver::recv_loop()
{
	//sf::IpAddress address;
	sf::IpAddress socket;
    char buffer[256];
	{
		std::stringstream ss;
		//ss << socket_->getRemoteAddress() << ":" << socket_->getRemotePort() << std::endl;
		std::cout << ss.str();
	}
	while (true)
	{
		std::memset(buffer, 0, 256);
        std::size_t received;
		/*auto recive = [&] {return socket_->receive(buffer, 256, received); };
		net_run(recive, "recived");
		{
			std::stringstream ss;
			ss << "received: \"" << buffer << "\", " << received << "bytes." << std::endl;
			std::cout << ss.str();
		}
		queue_.push(std::string(buffer));*/
    
    unsigned short port;
    
        
		// TODO: receive a message into buffer
		// Note same as in server - VS doesn't like projects sharing directories
		// END TODO: receive a message into buffer
        // Note that the queue of messages, now takes note of the peer
        // (address, port).
        //queue_.push(std::make_pair(std::string(buffer), std::make_pair(address, port)));
    }
}

