#include "stdafx.h"
#include "PacketHandler.h"

void PacketHandler::LATENCY_CHECK_Handler(shared_ptr<Session> session, Packet* packet)
{
	shared_ptr<ClientSession> cliSession = static_pointer_cast<ClientSession>(session);
	
	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
	p->SetSendTick(packet->GetSendTick());
	p->startPacket(Protocol::LATENCY_CHECK);
	p->endPacket(Protocol::LATENCY_CHECK);
	cliSession->Send(p);
}

void PacketHandler::C2S_ENTER_ROOM_Handler(shared_ptr<Session> session, Packet* packet)
{
	shared_ptr<ClientSession> cliSession = static_pointer_cast<ClientSession>(session);

	packet->pop(cliSession->_userInfo.nickName);
	// 다른사람에게 입장 소식 알리기
	g_Room->Join(cliSession, packet);
}

void PacketHandler::C2S_CHAT_REQ_Handler(shared_ptr<Session> session, Packet* packet)
{
	// 다른 사람에게 채팅 정보 보내기
	shared_ptr<ClientSession> cliSession = static_pointer_cast	<ClientSession>(session);
	
	string chat;
	packet->pop(chat);
	chat = cliSession->_userInfo.nickName + ": " + chat;

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
	p->SetSendTick(packet->GetSendTick());
	p->startPacket(Protocol::S2C_CHAT_RES);
	p->push(chat);
	p->endPacket(Protocol::S2C_CHAT_RES);
	g_Room->Broadcast(p);
}

void PacketHandler::C2S_EXIT_ROOM_Handler(shared_ptr<Session> session, Packet* packet)
{
	shared_ptr<ClientSession> cliSession = static_pointer_cast<ClientSession>(session);
	g_Room->Exit(cliSession, packet);
}
