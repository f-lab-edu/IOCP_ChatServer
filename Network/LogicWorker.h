#pragma once
#include "Worker.h"
#include <concurrent_queue.h>

class LogicWorker : public Worker
{
public:
    LogicWorker();
    ~LogicWorker() override;
    
public:
    void Init() override;
    void Work() override;

	void PushJob(std::shared_ptr<Packet> p);
private:
    HANDLE _jobAddEvent;
    Concurrency::concurrent_queue<std::shared_ptr<class Packet>> _assembledPackets;

};

