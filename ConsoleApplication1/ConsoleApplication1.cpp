// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include "ConsoleControl.h"
#include "Chat.h"
#include "ConnectionAbstraction/SocketManager.h"
#include "WindowsHelper/Window.h"

void RunClient();
void RunServer();
void RunWindows();

unsigned short port = 3000;

enum PackagesIds: Packet::PacketKey { Message = 0 };

int main()
{
    std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

    char mode = ' ';

	RunWindows();

	do
	{
		mode = ConsoleControl::WaitForReadNextChar();
	} while (mode != 'C' && mode != 'c' && mode != 'S' && mode != 's');

	switch (mode)
	{
	case 'C':
	case 'c':
	{
		RunClient();
		break;
	}
	case 'S':
	case 's':
	{
		RunServer();
		break;
	}
	default:
	{
		break;
	}
	}

	while (true)
	{

	}
}

void RunClient() 
{
	std::cout << "Client";

	std::cout << std::endl << "Set Server IP --> ";

	std::string ip;
	std::getline(std::cin, ip);

	//Chat* chat = Chat::Client(ip, port);

	SocketManager* SM = new SocketManager([](TcpSocket* socket)
		{
			std::cout << std::endl << "Socket connected" << socket->getRemoteAddress().toString();

			socket->Subscribe(Message, [socket](Packet packet)
				{
					std::string message;
					packet >> message;
					std::cout << std::endl << "New Message: " << message;

				});

			socket->SubscribeOnDisconnect([](TcpSocket* socket)
				{
					std::cout << std::endl << "Socket Disconnected" << socket->getRemoteAddress().toString();
				});

			std::string message = "Soc el client";
			Packet packet;
			packet << message;

			socket->Send(Message, packet);
		});

	if (SM->ConnectToServer(ip, port))
	{
		SM->StartLoop();
	}
}

void RunServer() 
{
	std::cout << "Server";

	//Chat* chat = Chat::Server(port);
	
	SocketManager* SM = new SocketManager([](TcpSocket* socket)
		{
			std::cout << std::endl << "Socket connected" << socket->getRemoteAddress().toString();

			socket->Subscribe(Message, [socket](Packet packet)
				{
					std::string message;
					packet >> message;
					std::cout << std::endl << "New Message: " << message;

					std::string response = "Soc el server";
					Packet responsePacket;
					responsePacket << response;
					socket->Send(Message, responsePacket);

				});

			socket->SubscribeOnDisconnect([](TcpSocket* socket)
				{
					std::cout << std::endl << "Socket Disconnected" << socket->getRemoteAddress().toString();
				});
		});

	if (SM->StartListener(port))
	{
		sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
		std::cout << "Listening on IP: " << ipAddress.toString();
		SM->StartLoop();
	}
}

void RunWindows()
{
	Window window;

	Button* button = new Button(50, 20, "Peces/QG.png");
	button->onClick = []()
	{
		std::cout << std::endl << "Long Live the Queen";
	};

	window.AddButton(button);
	window.RunWindowLoop();
}