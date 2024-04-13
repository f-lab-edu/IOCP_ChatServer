#pragma once
#include <unordered_map>
#include <thread>
#include <functional>
#include <mutex>

using namespace std;

extern thread_local int LThreadId;

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager() = default;

public:
	void ThreadInitialize();
	void ThreadStart(function<void(void)> work);
	void ThreadFinalize();

	void JoinAll();
private:
	vector<thread> _threads;

	mutex lock;
};

