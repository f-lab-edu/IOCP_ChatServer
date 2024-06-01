#include "stdafx.h"
#include "Player.h"

void Player::Move(Pos pos)
{
    _pos = _pos + pos;

    shared_ptr<Packet> packet = make_shared<Packet>(WRITE_PACKET);
    packet->startPacket(Protocol::S2C_MOVE_NOTIFY);
    packet->push(_playerId);
    packet->push(_pos.x);
    packet->push(pos.y);
    packet->endPacket(Protocol::S2C_MOVE_NOTIFY);
    _currentMap->Broadcast(packet);
}

void Player::Teleport(Pos pos)
{
    _pos = pos;
    
    shared_ptr<Packet> packet = make_shared<Packet>(WRITE_PACKET);
    packet->startPacket(Protocol::S2C_MOVE_NOTIFY);
    packet->push(_playerId);
    packet->push(_pos.x);
    packet->push(pos.y);
    packet->endPacket(Protocol::S2C_MOVE_NOTIFY);
    _currentMap->Broadcast(packet);
}

void Player::Attack(int hittedPlayerId)
{
    // TODO: ATTACK Vailidate ex) 거리 측정, 공격 쿨타임, 등등
    shared_ptr<Player> hittedPlayer = _currentMap->FindPlayer(hittedPlayerId);
    
   if(hittedPlayer == nullptr)
       return;
    
    hittedPlayer->Hit(shared_from_this());
}

void Player::Hit(std::shared_ptr<Player> attacker)
{
    // TODO: HP 깎
    _playerStat._hp -= attacker->GetStat()._strength;
    if(_playerStat._hp <= 0)
        Die();
}

void Player::Die()
{
}

