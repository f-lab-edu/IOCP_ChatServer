#pragma once
class ServerSession : public Session
{
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	virtual void OnAssemblePacket(Packet& packet);
};

