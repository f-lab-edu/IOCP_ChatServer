#pragma once
#include "Base.h"
#include <mutex>
#include <map>

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
    void Broadcast(shared_ptr<Packet> packet);
    
    std::shared_ptr<Player> CreatePlayer();

public:
    std::shared_ptr<Player> FindPlayer(int playerId);
private:
    MapInfo _mapInfo;
    
    mutex _playerLock;
    std::map<int, std::shared_ptr<Player>> _joinedPlayer;
};
