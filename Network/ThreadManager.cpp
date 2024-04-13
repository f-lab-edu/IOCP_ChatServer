#include "stdafx.h"
#include "ThreadManager.h"

thread_local int LThreadId = 0;

ThreadManager::ThreadManager()
{
	_threads = vector<thread>();
}

void ThreadManager::ThreadInitialize()
{
	static int idGenerator = 1;

	LThreadId = idGenerator++;
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
}

void ThreadManager::JoinAll()
{
	for (auto& thread : _threads)
		thread.join();
}
