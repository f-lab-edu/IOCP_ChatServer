#include "stdafx.h"
#include "Room.h"

Room* g_Room = nullptr;
using namespace std;

void Room::Broadcast(shared_ptr<Packet> packet)
{
	for (auto& r : _sessions)
		r->SendByCopy(packet->GetBuffer());

}

void Room::Join(shared_ptr<ClientSession> session)
{
	_sessions.insert(session);
	
	session->_userInfo.userId = ++_id;

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET,session->GetSendBuffer());
	p->startPacket(Protocol::S2C_ENTER_ROOM_NOTIFY);
	string contents;
	contents = "[ÀÔÀå] " + session->_userInfo.nickName + "´ÔÀÌ ÀÔÀåÇß½À´Ï´Ù.";
	p->push(contents);
	p->endPacket(Protocol::S2C_ENTER_ROOM_NOTIFY);

	Broadcast(p);
}

void Room::Exit(shared_ptr<ClientSession> session)
{
	_sessions.erase(session);

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET, session->GetSendBuffer());

	string contents;
	contents = "[ÅðÀå] " + session->_userInfo.nickName + "´ÔÀÌ ÅðÀåÇß½À´Ï´Ù.";
	p->startPacket(Protocol::S2C_EXIT_ROOM_NOTIFY);
	p->push(contents);
	p->endPacket(Protocol::S2C_EXIT_ROOM_NOTIFY);

	Broadcast(p);
}
