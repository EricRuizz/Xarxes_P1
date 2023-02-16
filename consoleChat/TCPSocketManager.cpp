#include "TCPSocketManager.h"
#include <iostream>

sf::Socket::Status TCPSocketManager::Listen(unsigned short port, sf::IpAddress ip)
{
    sf::Socket::Status status = dispatcher.listen(port, ip);
    if (status != sf::Socket::Done)
    {
        //No se puede vincular al puerto 5000
        std::cout << "Error al escuchar el puerto " + port << std::endl;
        return status;
    }

    sf::TcpSocket incoming;
    //Al conectarse un cliente, el socket incoming pasa a ser el que utilizan este cliente y el servidor para comunicarse en exclusiva
    if (dispatcher.accept(incoming) != sf::Socket::Done)
    {
        //Error al aceptar conexión
        std::cout << "Error al aceptar conexión" << std::endl;
    }

    return status;
}

void TCPSocketManager::Send(sf::Packet& packet, std::string* mssg)
{
    packet << mssg;
    sf::Socket::Status status = socket.send(packet);
    std::cout << "Message sent: " << mssg << std::endl;
    if (status != sf::Socket::Done)
    {
        // Error when sending data
        std::cout << "Error sending message" << std::endl;
    }
    packet.clear();
}

void TCPSocketManager::Receive(sf::Packet& packet, std::string* mssg)
{
    //sf::Packet received_packet;
    sf::TcpSocket incoming;
    incoming.receive(packet);
    packet >> *mssg;

    // Se procesaelmensaje
    if(mssg->size() > 0)
    {
        if (*mssg == "exit") 
        {
            // Manages the desconection
            Disconnect();
        }
        std::cout << "Received message: " << *mssg << std::endl;
        packet.clear();
    }

    // DesconectarTCP Listener
    dispatcher.close();
}

sf::Socket::Status TCPSocketManager::Connect(unsigned short port, sf::IpAddress ip)
{
    sf::Socket::Status status = socket.connect(ip, port, sf::seconds(5.f));
    if (status != sf::Socket::Done)
    {
        //No se ha podido conectar
        std::cout << "Error al conectarse con el servidor" << std::endl;
    }

    return status;
}

void TCPSocketManager::Disconnect()
{
    dispatcher.close();
    socket.disconnect();
}
