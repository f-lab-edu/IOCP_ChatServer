#pragma once
#include "Base.h"

class Map;

class Player
{
public:
    Player() = default;
    ~Player() = default;

    void Move(Pos pos);
    void Teleport(Pos pos);
    
public:
    int GetId(){ return _playerId; }
    void SetId(int id){ _playerId = id; }

    void SetOwner(shared_ptr<ServerSession> session) { _owner = session; }
private:
    int _playerId;

    shared_ptr<ServerSession> _owner;
    
    Map* _currentMap;
    Pos _pos;
};
