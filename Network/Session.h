#pragma once

class Session : public IocpObject
{
public:
	Session(HANDLE iocpHandle);
public:
	virtual void OnExecute(IoEvent* event, int SizeOfBytes) override;

public:
	bool RegisterConnect();
	bool RegisterDisconnect();
	void RegisterSend();
	void RegisterRecv();

	void CompletedConnect();
	void CompletedSend(int thread_id,int sizeOfBytes);
	void CompletedRecv(int sizeOfBytes);
	void CompletedDisconnect();
	
	void Connect(std::string ip, int port);
	void Send(Packet* p);
private:
	int OnRecv();

	virtual void OnConnected() {};
	virtual void OnSend(int sendSize) {};
	virtual void OnDisconnect() {};
	
	virtual void OnAssemblePacket(Packet* packet) {};
public:
	SOCKET GetSocket() { return _socket; }
	RingBuffer* GetRecvBuffer() { return &_recvBuffer; }
	char ip[INET_ADDRSTRLEN];
	int port = 0;
private:
	
	HANDLE _iocpHandle;
	SOCKET _socket = INVALID_SOCKET;

	IoEvent _connectEvent;
	IoEvent _recvEvent;

	SendEvent _sendEvent;
	bool _isSendRegister;
	queue<Packet*> _sendRegisteredPacket;
	mutex _sendLock;

	RingBuffer _recvBuffer;
};

