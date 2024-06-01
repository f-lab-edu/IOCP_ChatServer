#pragma once
#include <set>

extern Room* g_Room;

class Room
{
public:
	void Broadcast(std::shared_ptr<Packet> p);

	void Join(shared_ptr<ClientSession> session, std::shared_ptr<Packet> enterReqPacket);

	void Exit(shared_ptr<ClientSession> session, std::shared_ptr<Packet> exitReqPacket);
private:
	set<shared_ptr<ClientSession>> _sessions;
	int _id;
};

