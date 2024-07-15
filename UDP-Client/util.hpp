#include <SFML/Network.hpp>
#include <functional>

#define PORT 4300

using peer = std::pair<sf::IpAddress, unsigned short>;//STILL CODE IS NEVER CALLED
using message = std::pair<std::string, peer>;

void net_run(std::function<sf::Socket::Status(void)> f, std::string error_message);

