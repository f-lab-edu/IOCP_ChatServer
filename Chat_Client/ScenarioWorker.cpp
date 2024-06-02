#include "stdafx.h"
#include "ScenarioWorker.h"

ScenarioWorker::ScenarioWorker()
{
}

ScenarioWorker::~ScenarioWorker()
{
}

void ScenarioWorker::Init()
{
    Worker::Init();

    // begin
    _beginScenario.push_back(Protocol::C2S_ENTER_MAP);

    // repeat
    _repeatScenario.push_back(Protocol::C2S_CHAT_REQ);

    // end
    _endScenario.push_back(Protocol::C2S_EXIT_MAP);
}

void ScenarioWorker::Work()
{
    for(int id : _beginScenario)
        ExecuteScenario(id);

    std::chrono::milliseconds sleep_ms(1000);
	this_thread::sleep_for(sleep_ms);

		
    while(endFlag == false)
    {
        for(int id : _repeatScenario)
            ExecuteScenario(id);

		std::chrono::milliseconds sleep_ms(10);
		this_thread::sleep_for(sleep_ms);
    }

	this_thread::sleep_for(sleep_ms);



    for(int id : _endScenario)
            ExecuteScenario(id);

}

void ScenarioWorker::ExecuteScenario(int scenarioId)
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

void ScenarioWorker::EnterMap()
{
    shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
    p->startPacket(Protocol::C2S_ENTER_MAP);
    p->push(GetOwner()->_nickName);
    p->endPacket(Protocol::C2S_ENTER_MAP);

    GetOwner()->Send(p);
}

void ScenarioWorker::Chat()
{
    
    string chat;

    chat = "hello";

    shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
    p->startPacket(Protocol::C2S_CHAT_REQ);
    p->push(chat);
    p->endPacket(Protocol::C2S_CHAT_REQ);

    GetOwner()->Send(move(p));
}

void ScenarioWorker::ExitMap()
{
    shared_ptr<Packet> p =make_shared<Packet>(ePacketType::WRITE_PACKET);

    p->startPacket(Protocol::C2S_EXIT_MAP);
    p->endPacket(Protocol::C2S_EXIT_MAP);

    GetOwner()->Send(move(p));
}

void ScenarioWorker::Move()
{
}

void ScenarioWorker::Attack()
{
}

void ScenarioWorker::ScenarioEnd()
{
    endFlag = true;
}

