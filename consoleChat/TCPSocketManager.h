#pragma once 
#include <SFML\Network.hpp>
#include <string>
#include <list>

class TCPSocketManager
{
    std::list<sf::TcpSocket*> sockets;
    sf::TcpListener listener;
    sf::SocketSelector selector;

public:
    enum MessageTypes
    {
        LOGIN,
        MESSAGE,
        DISCONNECT,
        COUNT
    };

    sf::Socket::Status Listen(unsigned short port, sf::IpAddress ip);
    void ServerSend(std::string mssg);
    void ClientSend(std::string mssg);
    void ServerReceive(std::string* mssg);
    void ClientReceive(std::string* mssg);
    sf::Socket::Status Connect(unsigned short port, sf::IpAddress ip);
    void Disconnect();
    void AddListener(unsigned short port);
};
