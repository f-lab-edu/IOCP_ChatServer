#include "stdafx.h"
#include "LogicWorker.h"

LogicWorker::LogicWorker()
{
	_jobAddEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}

LogicWorker::~LogicWorker()
{
	CloseHandle(_jobAddEvent);
}

void LogicWorker::Init()
{
	_assembledPackets.clear();
}

void LogicWorker::Work()
{
	while (true)
	{
		std::shared_ptr<Packet> packet = nullptr;

		_assembledPackets.try_pop(packet);
		
		if (packet == nullptr)
			continue;
		

		Session* session = packet->GetOwner();
		session->OnAssemblePacket(packet);
		
		::WaitForSingleObject(_jobAddEvent, 100);
	}
}

void LogicWorker::PushJob(shared_ptr<Packet> p)
{
	_assembledPackets.push(p);
	SetEvent(_jobAddEvent);
}
