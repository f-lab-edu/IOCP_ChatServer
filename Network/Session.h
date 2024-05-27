#pragma once
#include "IocpObject.h"
#include "ioEvent.h"
#include "Buffer.h"

class Packet;

class Session : public IocpObject
{
public:
	Session(HANDLE iocpHandle);
	~Session();
public:
	virtual void OnExecute(IoEvent* event, int SizeOfBytes) override;

public:
	void RegisterConnect();
	void RegisterDisconnect();
	void RegisterSend();
	void RegisterRecv();

	void CompletedConnect();
	void CompletedSend(int sizeOfBytes);
	void CompletedRecv(int sizeOfBytes);
	void CompletedDisconnect();

	void Connect(std::string ip, int port);

	virtual void Send(std::shared_ptr<Packet> p);
	virtual void DoDisconnect();
private:
	int OnRecv();

	virtual void OnConnected() {};
	virtual void OnSend(int sendSize) {};
	virtual void OnDisconnect() {};
	
	virtual void OnAssemblePacket(Packet* packet) { }
public:
	SOCKET GetSocket() { return _socket; }
	Buffer* GetRecvBuffer() { return &_recvBuffer; }

	char _ip[INET_ADDRSTRLEN];
	int _port = 0;
private:
	
	HANDLE _iocpHandle;
	SOCKET _socket = INVALID_SOCKET;

	IoEvent _connectEvent;
	IoEvent _recvEvent;
	IoEvent _disconnectEvent;

	SendEvent _sendEvent;
	std::atomic<bool> _isSendRegister;
	Concurrency::concurrent_queue<std::shared_ptr<Packet>> _sendRegisteredPacket;
	std::vector<std::shared_ptr<Packet>> _sendCompletePacket;
	std::mutex _sendLock;

	Buffer _recvBuffer;

	std::atomic<bool> _isDisconnect;
};
