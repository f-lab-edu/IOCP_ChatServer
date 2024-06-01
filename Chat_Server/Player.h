#pragma once
#include "Base.h"

class ClientSession;
class Map;

struct PlayerStat
{
    int _hp;
    int _maxHp;
    int _strength;
};

class Player : public enable_shared_from_this<Player>
{
public:
    Player() = default;
    ~Player() = default;

    void Move(Pos pos);
    void Teleport(Pos pos);
    void Attack(int hittedPlayerId);
    void Hit(std::shared_ptr<Player> attacker);
    void Die();
    
public:
    int GetId(){ return _playerId; }
    void SetId(int id){ _playerId = id; }

    shared_ptr<ClientSession> GetOwner() { return _owner; }
    void SetOwner(shared_ptr<ClientSession> session) { _owner = session; }

    const PlayerStat& GetStat() { return const_cast<const PlayerStat&>(_playerStat); }
private:
    int _playerId;
    PlayerStat _playerStat;
    
    shared_ptr<ClientSession> _owner;
    
    Map* _currentMap;
    Pos _pos;
};
