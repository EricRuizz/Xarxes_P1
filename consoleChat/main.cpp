#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include <string>
#include <thread>
#include "TCPSocketManager.h"

const unsigned short PORT = 5000;
const sf::IpAddress IP = "127.0.0.1";
bool applicationRunning = false;

// Function to read from console (adapted for threads)
void GetLineFromCin_t(std::string* mssg) {
	while (true) {
		std::string line;
		std::getline(std::cin, line);
		mssg->assign(line);
	}
}

void OpenReceiveThread(TCPSocketManager tcpSocketManager, sf::Packet*& packet, std::string* mssg)
{
	while (applicationRunning)
	{
		tcpSocketManager.Receive(packet, mssg);
	}
}

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

		sf::Packet inPacket, outPacket;
		std::string sendMessage, receiveMessage;

		std::thread tcpScoketReceive(OpenReceiveThread, tcpSocketManager, inPacket, &receiveMessage);
		tcpScoketReceive.detach();

		std::cout << "Client connected";
		while (applicationRunning) {
			// Logic for receiving
			if (receiveMessage.size() > 0) {
				if (receiveMessage == "exit") {
					applicationRunning = false;
					// Manages the desconection 
					break;
				}
				std::cout << receiveMessage << std::endl;;
				receiveMessage.clear();
			}
			// Logic for sending
			// std::cout << "In applicacion loop" << std::endl;
			if (sendMessage.size() > 0) {
				if (sendMessage == "exit") {
					// Desconection
					applicationRunning = false;
					sendMessage.clear();
					break;
				}
				else {
					tcpSocketManager.Send(outPacket, &sendMessage);
					sendMessage.clear();
				}
			}
		}

		// When the application loop is broken, we have to release resources.
		tcpSocketManager.Disconnect();
		tcpScoketReceive.join();
		//send thread join

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

		sf::Packet inPacket, outPacket;
		std::string sendMessage, receiveMessage;

		switch (status)
		{
		case sf::Socket::Done:
			std::cout << "DONE" << std::endl;

			// Can now send/reveice mssgs
			//packet << GetInput();

			tcpSocketManager.Send(outPacket, &sendMessage);
			break;

		case sf::Socket::NotReady:
			break;
		case sf::Socket::Partial:
			break;
		case sf::Socket::Disconnected:
		case sf::Socket::Error:
		default:
			applicationRunning = false;
			std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAH" << std::endl;
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

	tcpSocketManager.Disconnect();
}