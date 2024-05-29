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
	ClientSession() = default;
	~ClientSession() override = default;

	HANDLE GetHandle() override { return reinterpret_cast<HANDLE>(GetSocket()); }
private:
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	virtual void OnAssemblePacket(Packet* packet) override;

public:
	UserInfo _userInfo;

};

