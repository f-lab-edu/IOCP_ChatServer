#include "stdafx.h"
#include "Room.h"

Room* g_Room = nullptr;

void Room::Broadcast(Packet* packet)
{
	for (auto& r : _sessions)
		r->Send(packet);
}

void Room::Join(shared_ptr<ClientSession> session)
{
	_sessions.insert(session);
	
	session->_userInfo.userId = ++_id;

	Packet* p = new Packet();
	p->startPacket(Protocol::S2C_ENTER_ROOM_NOTIFY);
	string contents;
	contents = "[입장] " + session->_userInfo.nickName + "님이 입장했습니다.";
	p->push(contents);
	p->endPacket(Protocol::S2C_ENTER_ROOM_NOTIFY);

	Broadcast(p);
}

void Room::Exit(shared_ptr<ClientSession> session)
{
	_sessions.erase(session);

	Packet* p = new Packet();
	p->startPacket(Protocol::S2C_EXIT_ROOM_NOTIFY);
	p->push(session->_userInfo.nickName);
	p->endPacket(Protocol::S2C_EXIT_ROOM_NOTIFY);

	Broadcast(p);
}
