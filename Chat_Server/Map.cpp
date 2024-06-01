#include "stdafx.h"
#include "Map.h"

#include "../Chat_Client/ServerSession.h"

class ServerSession;

Map::Map(MapInfo info)
{
}

Map::~Map()
{
    
}

void Map::Enter(std::shared_ptr<ClientSession> session, shared_ptr<Packet> enterReqPacket)
{
    std::shared_ptr<Player> player = CreatePlayer();
    player->SetOwner(session);
    
    _joinedPlayer.emplace(player->GetId(),player);

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
#ifdef LATENCY_RECORD_OPTION
	p->SetSendTick(enterReqPacket->GetSendTick());
#endif
	
	p->startPacket(Protocol::S2C_ENTER_MAP_NOTIFY);
	string contents;
	contents = "[입장] " + session->_userInfo.nickName + "님이" + _mapInfo._mapName + "에 입장했습니다.";
	p->push(contents);
	p->endPacket(Protocol::S2C_ENTER_MAP_NOTIFY);

	Broadcast(p);
}

void Map::Exit(int playerId, shared_ptr<Packet> exitReqPacket)
{
    auto player = _joinedPlayer.find(playerId);
    xassert(player == _joinedPlayer.end());

	std::shared_ptr<ClientSession> session = (*player).second->GetOwner();
    (*player).second->SetOwner(nullptr);        // referense release
    _joinedPlayer.erase(player);

	
	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
#ifdef LATENCY_RECORD_OPTION
	p->SetSendTick(exitReqPacket->GetSendTick());
#endif
	
	string contents;
	contents = "[퇴장] " + session->_userInfo.nickName + "님이" + _mapInfo._mapName + " 에서 퇴장했습니다.";
	p->startPacket(Protocol::S2C_EXIT_MAP_NOTIFY);
	p->push(contents);
	p->endPacket(Protocol::S2C_EXIT_MAP_NOTIFY);

	Broadcast(p);
}

std::shared_ptr<Player> Map::CreatePlayer()
{
    static int playerId = 1;
    
    shared_ptr<Player> player = make_shared<Player>();
    player->SetId(playerId++);

    return move(player);
}

void Map::Broadcast(shared_ptr<Packet> packet)
{
    for(auto& player : _joinedPlayer)
    {
        shared_ptr<ClientSession> p = player.second->GetOwner();
        p->Send(packet);
    }
}
