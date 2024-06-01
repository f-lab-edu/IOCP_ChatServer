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
