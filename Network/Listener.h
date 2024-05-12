#pragma once

class Session; 
class Listener : public IocpObject
{
public:
	Listener(HANDLE iocpHandle, function<shared_ptr<Session>(void)> sessionCreateFunc);
	~Listener() = default;
public:
	virtual void OnExecute(IoEvent* event, int SizeOfBytes) override;

	void StartAccept(int maxAccept);
	void RegisterAccept(AcceptEvent* ioEvent);
	void CompleteAccept(AcceptEvent* acceptEvent);

	
private:
	HANDLE _iocpHandle;

	SOCKET _listenSocket = INVALID_SOCKET;
	std::vector<IoEvent*> _acceptEvents;

	function<shared_ptr<Session>(void)> _sessionCreateFunc;
};

				