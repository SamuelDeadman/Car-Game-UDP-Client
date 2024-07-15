#include "receiver.hpp"
#include "util.hpp"
#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <tuple>

int main()
{
    std::shared_ptr<sf::UdpSocket> socket_ptr = std::make_shared<sf::UdpSocket>();
    // Sending a "register" message
    std::string s;
    std::cout << "Please type in your user name: ";
    std::cin >> s;
    // TODO: send the registration message consisting of the user name.
    Queue<message> queue;
    Receiver r{socket_ptr, queue};
    // TODO: launch a receiver thread
    while (true)
    {
        s = "";
        std::getline(std::cin, s);
        std::cout << "Sending: \"" << s << "\"" << std::endl;
        // TODO: send the message to the server
    }
    return 0;
}
