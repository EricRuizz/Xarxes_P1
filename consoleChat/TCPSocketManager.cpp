#include "TCPSocketManager.h"
#include <iostream>

sf::Socket::Status TCPSocketManager::Listen(unsigned short port, sf::IpAddress ip)
{
    sf::Socket::Status status = listener.listen(port, ip);
    if (status != sf::Socket::Status::Done)
    {
        //No se puede vincular al puerto 5000
        std::cout << "Error al escuchar el puerto " + port << std::endl;
        return status;
    }

    //Al conectarse un cliente, el socket incoming pasa a ser el que utilizan este cliente y el servidor para comunicarse en exclusiva
    if (listener.accept(socket) != sf::Socket::Done)
    {
        //Error al aceptar conexión
        std::cout << "Error al aceptar conexión" << std::endl;
        return sf::Socket::Status::Error;
    }

    return status;
}

void TCPSocketManager::Send(std::string message)
{
    sf::Packet packet;
    packet << message;

    sf::Socket::Status status = socket.send(packet);
    std::cout << "Message sent: " << message << std::endl;
    if (status != sf::Socket::Status::Done)
    {
        // Error when sending data
        std::cout << "Error sending message" << std::endl;
        return;
    }

    packet.clear();
}

void TCPSocketManager::Receive(std::string* mssg)
{
    sf::Packet packet;

    sf::Socket::Status status = socket.receive(packet);
    if (status != sf::Socket::Status::Done)
    {
        std::cout << "Error receiving message" << std::endl;
        return;
    }

    std::string masaje;
    packet >> masaje;

    // Se procesaelmensaje
    if(masaje.size() > 0)
    {
        if (masaje == "exit")
        {
            // Manages the desconection
            Disconnect();
        }
        std::cout << "Received message: " << masaje << std::endl;
        mssg->assign(masaje);
    }
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
    listener.close();
    socket.disconnect();
}
