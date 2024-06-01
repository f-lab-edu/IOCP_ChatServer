#include "stdafx.h"
#include "Map.h"

#include "../Chat_Client/ServerSession.h"

class ServerSession;

Map::Map(Pos minPos, Pos MaxPos)
{
}

Map::~Map()
{
    
}

void Map::Enter(std::shared_ptr<ServerSession> session)
{
    std::shared_ptr<Player> player = CreatePlayer();
    player->SetOwner(session);
    
    _joinedPlayer.emplace(player->GetId(),player);
}

void Map::Exit(int playerId)
{
    auto player = _joinedPlayer.find(playerId);
    xassert(player == _joinedPlayer.end());

    (*player).second->SetOwner(nullptr);        // referense release
    _joinedPlayer.erase(player);
}

std::shared_ptr<Player> Map::CreatePlayer()
{
    static int playerId = 1;
    
    shared_ptr<Player> player = make_shared<Player>();
    player->SetId(playerId++);

    return move(player);
}
