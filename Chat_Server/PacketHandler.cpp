#include "stdafx.h"
#include "PacketHandler.h"

#include "GameServerService.h"

void PacketHandler::LATENCY_CHECK_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	shared_ptr<ClientSession> cliSession = static_pointer_cast<ClientSession>(session);
	
	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
#ifdef LATENCY_RECORD_OPTION
	p->SetSendTick(packet->GetSendTick());
#endif
	
	p->startPacket(Protocol::LATENCY_CHECK);
	p->endPacket(Protocol::LATENCY_CHECK);
	cliSession->Send(p);
}

void PacketHandler::C2S_ENTER_MAP_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	shared_ptr<ClientSession> cliSession = static_pointer_cast<ClientSession>(session);

	packet->pop(cliSession->_userInfo.nickName);
	// 다른사람에게 입장 소식 알리기

	GameServerService* service = cliSession->GetService<GameServerService>();
	if(service == nullptr)
	{
		xassert(true);
		return;
	}

	Map* map = service->GetMap();
	if(map == nullptr)
	{
		xassert(true);
		return;
	}

	map->Enter(cliSession, packet);
}
	
void PacketHandler::C2S_CHAT_REQ_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	// 다른 사람에게 채팅 정보 보내기
	shared_ptr<ClientSession> cliSession = static_pointer_cast	<ClientSession>(session);
	
	string chat;
	packet->pop(chat);
	chat = cliSession->_userInfo.nickName + ": " + chat;

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
#ifdef LATENCY_RECORD_OPTION 
	p->SetSendTick(packet->GetSendTick());
#endif
	
	p->startPacket(Protocol::S2C_CHAT_RES);
	p->push(chat);
	p->endPacket(Protocol::S2C_CHAT_RES);
	GameServerService* service = cliSession->GetService<GameServerService>();
	if(service == nullptr)
	{
		xassert(true);
		return;
	}

	Map* map = service->GetMap();
	if(map == nullptr)
	{
		xassert(true);
		return;
	}

	map->Broadcast(packet);

}

void PacketHandler::C2S_EXIT_MAP_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	shared_ptr<ClientSession> cliSession = static_pointer_cast<ClientSession>(session);
	
	int playerId = 0;

	packet->pop(playerId);

	GameServerService* service = cliSession->GetService<GameServerService>();
	if(service == nullptr)
	{
		xassert(true);
		return;
	}

	Map* map = service->GetMap();
	if(map == nullptr)
	{
		xassert(true);
		return;
	}

	map->Exit(playerId, packet);
}

void PacketHandler::C2S_ATTACK_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	shared_ptr<ClientSession> cliSession = static_pointer_cast<ClientSession>(session);

	shared_ptr<Player> player = cliSession->GetPlayer();

	int playerId = 0;
	
	packet->pop(playerId);
	
	player->Attack(playerId);
}
