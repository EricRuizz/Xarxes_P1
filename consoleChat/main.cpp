#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include <string>
#include "TCPSocketManager.h"

const unsigned short PORT = 5000;
const sf::IpAddress IP = "127.0.0.1";

std::string GetInput();

void main() {
	int server_mode;
	std::string mode_str;
	std::cout << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
	server_mode = std::stoi(mode_str);

	TCPSocketManager tcpSocketManager;

	if (server_mode == 1) {
		std::cout << "Server mode running";
		// server connect
		tcpSocketManager.Listen(PORT, IP);

		while (true)
		{
			std::cout << "Client connected";
			// LOGIC
		}

		/*sf::TcpListener dispatcher;
		sf::Socket::Status status = dispatcher.listen(5000, "127.0.0.1");
		if (status != sf::Socket::Done)
		{
			//No se puede vincular al puerto 5000
			std::cout << "Error al escuchar el puerto 5000";
		}

		sf::TcpSocket incoming;
		//Al conectarse un cliente, el socket incoming pasa a ser el que utilizan este cliente y el servidor para comunicarse en exclusiva
		if (dispatcher.accept(incoming) != sf::Socket::Done)
		{
			//Error al aceptar conexión
			std::cout << "Error al aceptar conexión";
		}*/
	}
	else if (server_mode == 2) {

		std::cout << "Client mode running";
		// client connect
		sf::Socket::Status status = tcpSocketManager.Connect(PORT, IP);
		sf::Packet packet;

		switch (status)
		{
		case sf::Socket::Done:
			std::cout << "DONE" << std::endl;

			// Can now send/reveice mssgs
			//packet << GetInput();

			tcpSocketManager.Send(packet, PORT, IP);
			break;

		case sf::Socket::NotReady:
			break;
		case sf::Socket::Partial:
			break;
		case sf::Socket::Disconnected:
		case sf::Socket::Error:
		default:
			std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAH" << std::endl;
			//tcpSocketManager.Disconnect();
			break;
		}
		
		/*
		sf::TcpSocket socket;
		sf::Socket::Status status = socket.connect("127.0.0.1", 5000, sf::seconds(5.f));
		if (status != sf::Socket::Done)
		{
			//No se ha podido conectar
			std::cout << "Error al conectarsecon elservidor";
		}*/
	}
}

std::string GetInput()
{
	std::string mssg;

	std::cin >> mssg;

	return mssg;
}