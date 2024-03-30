#include "stdafx.h"
#include "ThreadManager.h"

thread_local int LThreadId = 0;

ThreadManager::ThreadManager()
{
	_threads = vector<thread>();
	_sendBuffers = unordered_map<int, RingBuffer*>();
}

RingBuffer* ThreadManager::GetSendBuffer(int thread_id)
{
	lock.lock();
	RingBuffer* buffer = _sendBuffers[thread_id];
	lock.unlock();
	return buffer;
}

void ThreadManager::ThreadInitialize()
{
	static int idGenerator = 1;

	LThreadId = idGenerator++;
	
	lock.lock();
	_sendBuffers.emplace(LThreadId, new RingBuffer(196605));
	lock.unlock();
}

void ThreadManager::ThreadStart(function<void(void)> work)
{
	std::thread startThread([=]()
		{
			ThreadInitialize();
			work();
			ThreadFinalize();
		});

	_threads.push_back(move(startThread));
}

void ThreadManager::ThreadFinalize()
{
	for (auto& buffer : _sendBuffers)
		delete buffer.second;
}

void ThreadManager::JoinAll()
{
	for (auto& thread : _threads)
		thread.join();
}

void ThreadManager::RenewalBuffer(int thread_id, int processSize)
{
	lock.lock();
	_sendBuffers[thread_id]->CompleteRead(processSize);
	lock.unlock();
}
