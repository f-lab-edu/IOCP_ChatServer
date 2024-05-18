#include "stdafx.h"
#include "Room.h"

Room* g_Room = nullptr;
using namespace std;

void Room::Broadcast(shared_ptr<Packet> packet)
{
	for (auto& r : _sessions)
		r->Send(packet);

}

void Room::Join(shared_ptr<ClientSession> session, Packet* EnterReqPacket)
{
	_sessions.insert(session);
	
	session->_userInfo.userId = ++_id;

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
#ifdef LATENCY_RECORD_OPTION
	p->SetSendTick(EnterReqPacket->GetSendTick());
#endif
	
	p->startPacket(Protocol::S2C_ENTER_ROOM_NOTIFY);
	string contents;
	contents = "[입장] " + session->_userInfo.nickName + "님이 입장했습니다.";
	p->push(contents);
	p->endPacket(Protocol::S2C_ENTER_ROOM_NOTIFY);

	Broadcast(p);
}

void Room::Exit(shared_ptr<ClientSession> session, Packet* ExitReqPacket)
{
	_sessions.erase(session);

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
#ifdef LATENCY_RECORD_OPTION
	p->SetSendTick(ExitReqPacket->GetSendTick());
#endif
	
	string contents;
	contents = "[퇴장] " + session->_userInfo.nickName + "님이 퇴장했습니다.";
	p->startPacket(Protocol::S2C_EXIT_ROOM_NOTIFY);
	p->push(contents);
	p->endPacket(Protocol::S2C_EXIT_ROOM_NOTIFY);

	Broadcast(p);
}
