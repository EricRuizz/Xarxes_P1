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
void GetLineFromCin(std::string* mssg) 
{
	while (applicationRunning) 
	{
		std::string line;
		std::getline(std::cin, line);
		mssg->assign(line);
	}
}

void OpenReceiveThread(TCPSocketManager* _tcpSocketManager, sf::Packet _packet, std::string* _mssg)
{
	while (applicationRunning)
	{
		_tcpSocketManager->Receive(_packet, _mssg);
	}
}

void main() 
{
	int server_mode;
	std::string mode_str;
	std::cout << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
	server_mode = std::stoi(mode_str);

	TCPSocketManager tcpSocketManager;


	if (server_mode == 1) 
	{
		std::cout << "Server mode running" << std::endl;
		// server connect
		tcpSocketManager.Listen(PORT, IP);

		sf::Packet inPacket, outPacket;
		std::string sendMessage, receiveMessage;

		// Logic for receiving
		std::thread tcpScoketReceive(OpenReceiveThread, &tcpSocketManager, inPacket, &receiveMessage);
		tcpScoketReceive.detach();

		std::thread getLines(GetLineFromCin, &sendMessage);
		getLines.detach();

		std::cout << "Client connected" << std::endl;
		while (applicationRunning) 
		{
			// Logic for sending
			if (sendMessage.size() > 0) 
			{
				if (sendMessage == "exit") 
				{
					// Desconection
					applicationRunning = false;
					sendMessage.clear();
					break;
				}
				else 
				{
					tcpSocketManager.Send(outPacket, &sendMessage);
					sendMessage.clear();
				}
			}
		}

		// When the application loop is broken, we have to release resources.
		tcpSocketManager.Disconnect();
		tcpScoketReceive.join();
		getLines.join();
	}
	else if (server_mode == 2) {

		std::cout << "Client mode running" << std::endl;
		// client connect
		sf::Socket::Status status = tcpSocketManager.Connect(PORT, IP);

		sf::Packet inPacket, outPacket;
		std::string sendMessage, receiveMessage;

		// Logic for receiving
		std::thread tcpSocketReceive(OpenReceiveThread, &tcpSocketManager, inPacket, &receiveMessage);
		tcpSocketReceive.detach();

		std::thread getLines(GetLineFromCin, &sendMessage);
		getLines.detach();

		switch (status)
		{
		case sf::Socket::Done:
			std::cout << "DONE" << std::endl;

			while (applicationRunning)
			{
				if (sendMessage.size() > 0)
				{
					if (sendMessage == "exit")
					{
						// Desconection
						applicationRunning = false;
						sendMessage.clear();
						break;
					}
					else
					{
						tcpSocketManager.Send(outPacket, &sendMessage);
						sendMessage.clear();
					}
				}
			}
			break;

		case sf::Socket::Disconnected:
		case sf::Socket::Error:
		default:
			applicationRunning = false;
			break;
		}
		tcpSocketManager.Disconnect();
		tcpSocketReceive.join();
		getLines.join();
	}

	tcpSocketManager.Disconnect();
}