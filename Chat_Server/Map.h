#pragma once
#include "Base.h"

class Player;

struct MapInfo
{
    std::string _mapName;
    Pos _spawnPos;
    Pos _mapMinPos;
    Pos _mapMaxPos;
};

class Map
{
public:
    Map(MapInfo info);
    ~Map();
public:
    void Enter(std::shared_ptr<ClientSession> session, shared_ptr<Packet> enterReqPacket);
    void Exit(int playerId, shared_ptr<Packet> exitReqPacket);

    std::shared_ptr<Player> CreatePlayer();

private:
    void Broadcast(shared_ptr<Packet> packet);
private:
    MapInfo _mapInfo;
    std::map<int, std::shared_ptr<Player>> _joinedPlayer;
};
