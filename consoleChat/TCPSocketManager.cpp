#include "TCPSocketManager.h"
#include <iostream>

sf::Socket::Status TCPSocketManager::Listen(unsigned short port, sf::IpAddress ip)
{
    sf::Socket::Status status = dispatcher.listen(5000, "127.0.0.1");
    if (status != sf::Socket::Done)
    {
        //No se puede vincular al puerto 5000
        std::cout << "Error al escuchar el puerto 5000";
        return status;
    }

    sf::TcpSocket incoming;
    //Al conectarse un cliente, el socket incoming pasa a ser el que utilizan este cliente y el servidor para comunicarse en exclusiva
    if (dispatcher.accept(incoming) != sf::Socket::Done)
    {
        //Error al aceptar conexión
        std::cout << "Error al aceptar conexión";
    }

    return status;
}

sf::Socket::Status TCPSocketManager::Send(sf::Packet& packet, unsigned short port, sf::IpAddress ip)
{
    // Envíode paquetes
    //sf::Packet packet;
    //std::string message = "hola";
    //packet << message;

    // Rellenardata
    sf::Socket::Status status = sock.send(packet);
    if(status != sf::Socket::Done)
    {
        // Ha fallado el envío de datos
        std::cout << "Error al conectarse con el servidor";
    }
    sock.disconnect();

    return status;
}

sf::Socket::Status TCPSocketManager::Receive(sf::Packet*& packet, sf::IpAddress& ip, unsigned short& port)
{
    sf::Packet received_packet;
    std::string message;
    sf::TcpSocket incoming;
    incoming.receive(received_packet);
    received_packet >> message;

    // Se procesaelmensaje
    if(message.size() > 0)
    {
        std::cout << "Se ha recibidoelmensaje:" << message;
        message.clear();
    }

    // DesconectarTCP Listener
    dispatcher.close();

    return sf::Socket::Status();
}

sf::Socket::Status TCPSocketManager::Connect(unsigned short port, sf::IpAddress ip)
{
    sf::Socket::Status status = sock.connect(ip, port, sf::seconds(5.f));
    if (status != sf::Socket::Done)
    {
        //No se ha podido conectar
        std::cout << "Error al conectarsecon elservidor";
    }

    return status;
}
