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
	RingBuffer* GetSendBuffer(int thread_id);
public:
	void ThreadInitialize();
	void ThreadStart(function<void(void)> work);
	void ThreadFinalize();

	void JoinAll();

	void RenewalBuffer(int thread_id, int processSize);
private:
	vector<thread> _threads;
	unordered_map<int, RingBuffer*> _sendBuffers;

	mutex lock;
};

