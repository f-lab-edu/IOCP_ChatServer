#pragma once

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
	void CompletedSend(int thread_id,int sizeOfBytes);
	void CompletedRecv(int sizeOfBytes);
	void CompletedDisconnect();

	void Connect(std::string ip, int port);
	void Send(Packet* p);
	void SendByCopy(Buffer* packetBuffer);
	virtual void DoDisconnect();
private:
	int OnRecv();

	virtual void OnConnected() {};
	virtual void OnSend(int sendSize) {};
	virtual void OnDisconnect() {};
	
	virtual void OnAssemblePacket(Packet* packet) {};
public:
	SOCKET GetSocket() { return _socket; }
	Buffer* GetRecvBuffer() { return &_recvBuffer; }

	char _ip[INET_ADDRSTRLEN];
	int _port = 0;
public:
	Buffer* GetSendBuffer();
private:
	
	HANDLE _iocpHandle;
	SOCKET _socket = INVALID_SOCKET;

	IoEvent _connectEvent;
	IoEvent _recvEvent;
	IoEvent _disconnectEvent;

	SendEvent _sendEvent;
	atomic<bool> _isSendRegister;
	concurrent_queue<Packet*> _sendRegisteredPacket;
	vector<Packet*> _sendCompletePacket;
	mutex _sendLock;

	Buffer* _sendBuffer;
	Buffer _recvBuffer;
};

