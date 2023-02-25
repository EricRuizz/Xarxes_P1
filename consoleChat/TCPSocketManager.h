#pragma once 
#include <SFML\Network.hpp>
#include <string>
#include <list>
#include <iostream>

class TCPSocketManager
{
    std::list<sf::TcpSocket*> sockets;
    sf::TcpListener listener;
    sf::SocketSelector selector;
    std::vector<std::string> usernames;

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
    void ClientSend(sf::Packet infoPack);
    void ServerReceive(sf::Packet receivedPacket);
    void ClientReceive(std::string* mssg);
    sf::Socket::Status Connect(unsigned short port, sf::IpAddress ip);
    void Disconnect();
    void AddListener(unsigned short port);
};
