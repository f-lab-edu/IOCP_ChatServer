#pragma once
#include "Base.h"

class ClientSession;
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

    shared_ptr<ClientSession> GetOwner() { return _owner; }
    void SetOwner(shared_ptr<ClientSession> session) { _owner = session; }
private:
    int _playerId;

    shared_ptr<ClientSession> _owner;
    
    Map* _currentMap;
    Pos _pos;
};
