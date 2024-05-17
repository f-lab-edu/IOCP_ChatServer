#pragma once
#include <set>

extern Room* g_Room;

class Room
{
public:
	void Broadcast(shared_ptr<Packet> p);

	void Join(shared_ptr<ClientSession> session, Packet* EnterReqPacket);

	void Exit(shared_ptr<ClientSession> session, Packet* ExitReqPacket);
private:
	set<shared_ptr<ClientSession>> _sessions;
	int _id;
};

