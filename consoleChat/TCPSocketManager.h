#pragma once 
#include <SFML\Network.hpp>
#include <string>

class TCPSocketManager
{
    sf::TcpSocket socket;
    sf::TcpListener listener;

public:
    sf::Socket::Status Listen(unsigned short port, sf::IpAddress ip);
    void Send(std::string mssg);
    void Receive(std::string* mssg);
    sf::Socket::Status Connect(unsigned short port, sf::IpAddress ip);
    void Disconnect();
    unsigned short GetLocalPort();
};
