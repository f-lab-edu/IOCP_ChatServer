#pragma once
#include <string>
class ServerSession : public Session
{
public:
	ServerSession(HANDLE iocp);
	
private:
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	void OnAssemblePacket(Packet* packet) override;
public:
	string nickName;
	
};

