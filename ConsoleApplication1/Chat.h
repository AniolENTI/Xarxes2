#pragma once
#include <iostream>
#include <mutex>
#include <SFML/Network.hpp>
#include <vector>
#include <list>

class Chat
{
private:
	Chat();

	bool _isServer = false;
	std::mutex _isServerMutex;

	std::list<sf::TcpSocket*> _sockets;
	std::mutex _socketsMutex;

	sf::IpAddress _serverAddress;
	std::vector<std::string> _messages;
	std::mutex _messagesMutex;

	void ShowMessage(std::string message);
	void ShowWarning(std::string message);
	void ShowError(std::string message);
	void ListenClientsConnections(unsigned short port);
	void ConnectToServer(std::string ip, unsigned short port);
	void OnClientEnter(sf::TcpSocket* client);
	void ListenMessages(sf::TcpSocket* socket);
	void ListenKeyboardToSendMessage();
	void SendMessage(std::string message);

public:
	static Chat* Server(unsigned short port);
	static Chat* Client(std::string ip, unsigned short port);
};

