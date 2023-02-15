#pragma once 
#include <SFML\Network.hpp>
#include <string>

class TCPSocketManager
{
    sf::TcpSocket sock;
    sf::TcpListener dispatcher;

public:
    sf::Socket::Status Listen(unsigned short port, sf::IpAddress ip);
    sf::Socket::Status Send(sf::Packet& packet, unsigned short port, sf::IpAddress ip);
    sf::Socket::Status Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port);
    sf::Socket::Status Connect(unsigned short port, sf::IpAddress ip);
    void Disconnect();
    unsigned short GetLocalPort();
};
