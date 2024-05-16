#pragma once
#include <string>
class ServerSession : public Session
{
public:
	void SetTestFlag(bool flag) { isTestMode = flag; }
private:
	bool isTestMode;
public:
	ServerSession(HANDLE iocp);
public:
	void DoDisconnect() override;

private:
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	void OnAssemblePacket(Packet* packet) override;

public:
	DWORD WINAPI ChattingLogic();
public:
	string nickName;


};

