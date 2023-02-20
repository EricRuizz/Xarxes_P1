#pragma once
#include <SFML\Network.hpp>
#include <iostream> 
#include <string>
#include <thread>
#include "TCPSocketManager.h"

const unsigned short PORT = 4999;
const sf::IpAddress IP = "127.0.0.1";
bool applicationRunning = true;

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
		_tcpSocketManager->Receive(_mssg);
	}
}

bool SendLogic(TCPSocketManager* tcpSocketManager, std::string* message)
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
			tcpSocketManager->Send(*message);
			message->clear();
		}
	}

	return true;
}

void Server()
{
	std::cout << "Server mode running" << std::endl;

	TCPSocketManager tcpSocketManager;
	// server connect
	if (tcpSocketManager.Listen(PORT, IP) != sf::Socket::Status::Done)
	{
		std::cout << "Error connecting" << std::endl;
		return;
	}

	//sf::Packet inPacket, outPacket;
	std::string sendMessage, receiveMessage;

	// Logic for receiving
	std::thread tcpScoketReceive(OpenReceiveThread, &tcpSocketManager, &receiveMessage);
	tcpScoketReceive.detach();

	std::thread getLines(GetLineFromCin, &sendMessage);
	getLines.detach();

	std::cout << "Client connected" << std::endl;
	while (applicationRunning)
	{
		// Logic for sending
		if (SendLogic(&tcpSocketManager, &sendMessage) != true)
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
	// client connect
	sf::Socket::Status status = tcpSocketManager.Connect(PORT, IP);

	//sf::Packet inPacket, outPacket;
	std::string sendMessage, receiveMessage;

	// Logic for receiving
	std::thread tcpSocketReceive(OpenReceiveThread, &tcpSocketManager, &receiveMessage);
	tcpSocketReceive.detach();

	std::thread getLines(GetLineFromCin, &sendMessage);
	getLines.detach();

	while (applicationRunning)
	{
		if (status != sf::Socket::Done)
		{
			applicationRunning = false;
			break;
		}

		// Logic for sending
		if (SendLogic(&tcpSocketManager, &sendMessage) != true)
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