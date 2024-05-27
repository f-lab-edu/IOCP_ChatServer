#pragma once


extern thread_local int LThreadId;

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager() = default;

public:
	void ThreadInitialize();
	void ThreadStart(std::function<void(void)> work);
	void ThreadFinalize();

	void JoinAll();
private:
	std::vector<std::thread> _threads;

	std::mutex lock;
};

