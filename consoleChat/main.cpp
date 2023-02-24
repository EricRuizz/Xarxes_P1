#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include <string>
#include <thread>
#include "TCPSocketManager.h"

const unsigned short PORT = 5000;
const sf::IpAddress IP = "127.0.0.1";
bool applicationRunning = true;

enum Mode
{
	SERVER,
	CLIENT,
	COUNT
};

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

void OpenReceiveThread(TCPSocketManager* _tcpSocketManager, std::string* _mssg)
{
	while (applicationRunning)
	{
		_tcpSocketManager->ClientReceive(_mssg);
	}
}

void OpenListener(TCPSocketManager* _tcpSocketManager)
{
	_tcpSocketManager->AddListener(PORT);

	while (applicationRunning)
	{
		_tcpSocketManager->Listen(PORT, IP);
	}
}

bool SendLogic(TCPSocketManager* tcpSocketManager, Mode mode, std::string* message) // Change message for packet
{
	if (message->size() > 0)
	{
		if (*message == "exit")
		{
			// Desconection
			applicationRunning = false;
			message->clear();
			return false;
		}
		else
		{
			switch (mode)
			{
			case SERVER:
				tcpSocketManager->ServerSend(*message);
				break;
			case CLIENT:
				tcpSocketManager->ClientSend(*message);
				break;
			default:
				break;
			}
			message->clear();
		}
	}

	return true;
}

void Server()
{
	std::cout << "Server mode running" << std::endl;

	std::vector<std::string> usernames;
	TCPSocketManager tcpSocketManager;

	//sf::Packet inPacket, outPacket;
	std::string sendMessage, receiveMessage;

	// Logic for receiving
	std::thread tcpScoketListen(OpenListener, &tcpSocketManager);
	tcpScoketListen.detach();

	std::thread getLines(GetLineFromCin, &sendMessage);
	getLines.detach();

	while (applicationRunning)
	{
		// Logic for sending
		if (SendLogic(&tcpSocketManager, Mode::SERVER, &sendMessage) != true)
		{
			break;
		}
	}

	tcpSocketManager.Disconnect();
}

void Client()
{
	std::cout << "Client mode running" << std::endl;
	
	TCPSocketManager tcpSocketManager;
	std::string username;

	// client connect
	sf::Socket::Status status = tcpSocketManager.Connect(PORT, IP);

	//sf::Packet inPacket, outPacket;
	std::string sendMessage, receiveMessage;

	std::cout << "EEE" << std::endl;
	// Logic for receiving
	std::thread tcpSocketReceive(OpenReceiveThread, &tcpSocketManager, &receiveMessage);
	tcpSocketReceive.detach();

	std::cout << "AAA" << std::endl;
	std::thread getLines(GetLineFromCin, &sendMessage);
	getLines.detach();

	std::cout << "Input your username:" << std::endl;

	while (applicationRunning)
	{
		if (status != sf::Socket::Done)
		{
			applicationRunning = false;
			break;
		}

		// Logic for sending
		if (SendLogic(&tcpSocketManager, Mode::CLIENT, &sendMessage) != true)
		{
			break;
		}
	}

	tcpSocketManager.Disconnect();
}

void main()
{
	int server_mode;
	std::string mode_str;
	std::cout << "Select a mode: (1) server, (2) cliente" << std::endl;
	std::cin >> mode_str;
	server_mode = std::stoi(mode_str);

	if (server_mode == 1) 
	{
		Server();
	}
	else if (server_mode == 2)
	{
		Client();
	}
}