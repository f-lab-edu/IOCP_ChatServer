#include "stdafx.h"
#include "Scenario.h"

Scenario::Scenario()
{
}

Scenario::~Scenario()
{
}

void Scenario::Init()
{
    // begin
    _beginScenario.push_back(Protocol::C2S_ENTER_MAP);

    // repeat
    _repeatScenario.push_back(Protocol::C2S_CHAT_REQ);

    // end
    _endScenario.push_back(Protocol::C2S_EXIT_MAP);

    ////////////////////////////////////////////////////////////
    currentScenarioIdx = 0;
}

void Scenario::ExecuteScenario(int scenarioId)
{
    switch (scenarioId)
    {
    case Protocol::C2S_ENTER_MAP:
        EnterMap();
        break;
    case Protocol::C2S_CHAT_REQ:
        Chat();
        break;
    case Protocol::C2S_EXIT_MAP:
        ExitMap();
        break;
    case Protocol::C2S_MOVE:
        Move();
        break;
    case Protocol::C2S_ATTACK:
        Attack();
        break;
    }
    
}

void Scenario::StartScenario()
{
    for(int id : _beginScenario)
           ExecuteScenario(id);
}

void Scenario::EnterMap()
{
    shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
    p->startPacket(Protocol::C2S_ENTER_MAP);
    p->push(GetOwner()->_nickName);
    p->endPacket(Protocol::C2S_ENTER_MAP);

    GetOwner()->Send(p);
}

void Scenario::Chat()
{
    
    string chat;

    chat = "hello";

    shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
    p->startPacket(Protocol::C2S_CHAT_REQ);
    p->push(chat);
    p->endPacket(Protocol::C2S_CHAT_REQ);

    GetOwner()->Send(move(p));
}

void Scenario::ExitMap()
{
    shared_ptr<Packet> p =make_shared<Packet>(ePacketType::WRITE_PACKET);

    p->startPacket(Protocol::C2S_EXIT_MAP);
    p->endPacket(Protocol::C2S_EXIT_MAP);

    GetOwner()->Send(move(p));
}

void Scenario::Move()
{
    shared_ptr<Packet> p =make_shared<Packet>(ePacketType::WRITE_PACKET);
    p->startPacket(Protocol::C2S_MOVE);

    int x = rand() % 3 - 1;
    int y = rand() % 3 - 1;
    p->push(x);
    p->push(y);
    p->endPacket(Protocol::C2S_MOVE);

    GetOwner()->Send(p);
}

void Scenario::Attack()
{
}

void Scenario::ScenarioEnd()
{
     for(int id : _endScenario)
             ExecuteScenario(id);
}

void Scenario::NextScenario()
{
   ExecuteScenario(_repeatScenario[currentScenarioIdx]);
    currentScenarioIdx = (currentScenarioIdx + 1) % _repeatScenario.size();
}
