#include "SocketManager.h"

SocketManager::SocketManager(OnSocketConnected onSocketConnected)
{
	_onSocketConnected = onSocketConnected;
}

SocketManager::~SocketManager()
{
	delete _listener;

	for (TcpSocket* socket : _sockets)
	{
		delete socket;
	}
}

void SocketManager::StartLoop()
{
	_isRunningMutex.lock();

	if (_isRunning) //Prevent multiples loops
	{
		_isRunningMutex.unlock();
		return;
	}

	_isRunning = true;

	_isRunningMutex.unlock();

	std::thread* loopThread = new std::thread(&SocketManager::SelectorLoop, this);
	loopThread->detach();
}

bool SocketManager::StartListener(unsigned short port)
{
	_listenerMutex.lock();

	if (_listener != nullptr)
	{
		_listenerMutex.unlock();
		return false;
	}

	_listener = new TcpListener();
	if (!_listener->Listen(port))
	{
		delete _listener;
		_listenerMutex.unlock();
		return false;
	}

	_selector.Add(*_listener);

	_listenerMutex.unlock();
	return true;
}

bool SocketManager::ConnectToServer(std::string ip, unsigned short port)
{
	TcpSocket* socket = new TcpSocket();
	if (!socket->Connect(ip, port))
	{
		delete socket;
		return false;
	}

	AddSocket(socket);

	return true;
}

void SocketManager::SelectorLoop()
{
	_isRunningMutex.lock();
	bool isRunning = true;
	_isRunningMutex.unlock();

	while(isRunning) 
	{

		if (_selector.Wait())
		{
			CheckListener();

			CheckSockets();
		}

		_isRunningMutex.lock();
		isRunning = _isRunning;
		_isRunningMutex.unlock();

	}
}

void SocketManager::CheckListener()
{
	_listenerMutex.lock();

	if (_listener != nullptr && _selector.IsReady(*_listener))
	{
		TcpSocket* socket = new TcpSocket();
		if (_listener->Accept(*socket))
		{
			AddSocket(socket);
		}
		else
		{
			delete socket;
		}
	}

	_listenerMutex.unlock();
}

void SocketManager::CheckSockets()
{
	_socketsMutex.lock();

	for (TcpSocket* socket : _sockets)
	{
		if (_selector.IsReady(*socket))
		{
			socket->Receive();
		}
	}

	_socketsMutex.unlock();
}

void SocketManager::AddSocket(TcpSocket* socket)
{
	_socketsMutex.lock();

	_sockets.push_back(socket);
	_selector.Add(*socket);
	_onSocketConnected(socket);

	socket->SubscribeOnDisconnect([this](TcpSocket* socket) {
		RemoveSocketAsync(socket);
		});


	_socketsMutex.unlock();
}

void SocketManager::RemoveSocket(TcpSocket* socket)
{
	_selector.Remove(*socket);

	_socketsMutex.lock();

	_sockets.remove(socket);

	_socketsMutex.unlock();

	delete socket;
}

void SocketManager::RemoveSocketAsync(TcpSocket* socket)
{
	std::thread* removeSocketThread = new std::thread(&SocketManager::RemoveSocket, this, socket);
	removeSocketThread->detach();
}
