#pragma once
struct UserInfo
{
	int userId;
	string nickName;
};

class ClientSession : public Session
{
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	virtual void OnAssemblePacket(Packet& packet);
};

