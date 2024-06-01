#pragma once
#include "Base.h"

class ServerSession;
class Player;

class Map
{
public:
    Map(Pos minPos, Pos MaxPos);
    ~Map();
public:
    void Enter(std::shared_ptr<ServerSession> session);
    void Exit(int playerId);

    std::shared_ptr<Player> CreatePlayer();
private:
    Pos _mapMinPos;
    Pos _mapMaxPos;

    std::map<int, std::shared_ptr<Player>> _joinedPlayer;
};
