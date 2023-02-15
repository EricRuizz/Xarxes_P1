#pragma once 
#include <SFML\Network.hpp>
#include <string>

class TCPSocketManager
{
    sf::TcpSocket socket;
    sf::TcpListener dispatcher;

public:
    sf::Socket::Status Listen(unsigned short port, sf::IpAddress ip);
    void Send(sf::Packet& packet, std::string* mssg);
    void Receive(sf::Packet*& packet, std::string* mssg);
    sf::Socket::Status Connect(unsigned short port, sf::IpAddress ip);
    void Disconnect();
    unsigned short GetLocalPort();
};
