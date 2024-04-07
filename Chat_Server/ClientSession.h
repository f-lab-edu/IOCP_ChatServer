#pragma once
class Room;

struct UserInfo
{
	int userId;
	string nickName;
};

class ClientSession : public Session
{
public:
	ClientSession(HANDLE iocp);

private:
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	void OnAssemblePacket(Packet* packet) override;

public:
	UserInfo _userInfo;
};

