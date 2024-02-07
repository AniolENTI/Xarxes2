#pragma once
#include <iostream>
#include <mutex>
#include <SFML/Network.hpp>
#include <vector>

class Chat
{
private:
	Chat();

	sf::IpAddress _serverAddress;
	std::vector<std::string> _messages;
	std::mutex _messagesMutex;

	void ShowMessage(std::string message);
	void ShowWarning(std::string message);
	void ShowError(std::string message);
	void ListenClientsConnections(unsigned short port);

public:
	static Chat* Server(unsigned short port);
	static Chat* Client(std::string ip, unsigned short port);
};

