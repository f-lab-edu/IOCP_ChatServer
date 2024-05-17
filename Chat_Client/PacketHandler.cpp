#include "stdafx.h"
#include "PacketHandler.h"

void PacketHandler::LATENCY_CHECK_Handler(shared_ptr<Session> session, Packet* packet)
{
	shared_ptr<ServerSession> serverSession = static_pointer_cast<ServerSession>(session);

	clock_t tick = 0;
	packet->pop(tick);
	clock_t curTick = clock();
	serverSession->AddLatency(packet->GetPacketId(), curTick - tick);
}

void PacketHandler::S2C_ENTER_ROOM_NOTIFY_Handler(shared_ptr<Session> session, Packet* packet)
{
	string str;
	packet->pop(str);
	cout << str << endl;
}

void PacketHandler::S2C_CHAT_RES_Handler(shared_ptr<Session> session, Packet* packet)
{
	string chat;
	packet->pop(chat);
	cout << chat << endl;
  }

void PacketHandler::S2C_EXIT_ROOM_NOTIFY_Handler(shared_ptr<Session> session, Packet* packet)
{
	string str;
	packet->pop(str);
	cout << str << endl;
}
