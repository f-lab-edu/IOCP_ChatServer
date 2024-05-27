#pragma once

class ServerService;
class Session; 
class Listener : public IocpObject
{
public:
	Listener(ServerService* service);
	~Listener() = default;
public:
	virtual void OnExecute(IoEvent* event, int SizeOfBytes) override;

	void StartAccept(int maxAccept);
	void RegisterAccept(AcceptEvent* ioEvent);
	void CompleteAccept(AcceptEvent* acceptEvent);
	HANDLE GetHandle() override { return reinterpret_cast<HANDLE>(_listenSocket); }

private:
	ServerService* _service;
	SOCKET _listenSocket = INVALID_SOCKET;
	std::vector<IoEvent*> _acceptEvents;

	
};

				