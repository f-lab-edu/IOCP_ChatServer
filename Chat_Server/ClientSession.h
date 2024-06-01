#pragma once
class Room;
class Player;

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

	virtual void OnAssemblePacket(std::shared_ptr<Packet> packet) override;

public:
	shared_ptr<Player> GetPlayer() { return _myPlayer; }
	void SetPlayer(shared_ptr<Player> player) { _myPlayer = player; }
public:
	UserInfo _userInfo;
	
private:
	shared_ptr<Player> _myPlayer;
};

