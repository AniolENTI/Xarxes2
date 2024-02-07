// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <conio.h>
#include <SFML/Network.hpp>

void RunClient();
void RunServer();

int main()
{
    std::cout << "Select" << std::endl << "Client -> C" << std::endl << "Server -> S" << std::endl;

    char mode = ' ';

	do
	{
		if (_kbhit())
		{
			mode = _getch();
		}
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

	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect("10.40.2.185", 3000);

	if (status != sf::Socket::Done)
	{
		std::cout << std::endl << "Error on Connect to Server";
		return;
	}

	while (true)
	{
		std::cout << std::endl << "Next Message: ";
		std::string message;
		std::getline(std::cin, message);

		char data[100];

		int stringSize = message.length();
		for (int i = 0; i < stringSize; i++)
		{
			char c = message[i];
			data[i] = c;
		}

		if (socket.send(data, 100) != sf::Socket::Done)
		{
			std::cout << std::endl << "Error on Sending Message";
		}
	}

}

void RunServer() 
{
	std::cout << "Server";

	sf::TcpListener listener;

	if (listener.listen(3000) != sf::Socket::Done)
	{
		std::cout << std::endl << "Error on Start Listener";
		return;
	}

	sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
	std::cout << std::endl << "Listenening on IP: " + ipAddress.toString();

	sf::TcpSocket client;

	if (listener.accept(client) != sf::Socket::Done)
	{
		std::cout << std::endl << "Error on Accept Client";
		return;
	}

	std::cout << std::endl << "Client Connected: " << client.getRemoteAddress().toString();

	while (true)
	{
		char data[100];
		std::size_t received;

		std::string message;

		if (client.receive(data, 100, received) != sf::Socket::Done)
		{
			std::cout << std::endl << "Error on Receive Message";
		}
		else
		{
			for (size_t i = 0; i < received; i++)
			{
				char c = data[i];
				message += c;
			}
			std::cout << std::endl << message;
		}
	}
}