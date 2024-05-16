#pragma once
#include "Buffer.h"
#include <concurrent_queue.h>
using namespace std;
using namespace Concurrency;

const long bufferSize = 196605;

class BufferManager
{
public:
	BufferManager(int bufferNum = 5);
	~BufferManager();
public:
	Buffer* AssignBuffer();
	void ReturnBuffer(Buffer* buffer);
	
private:
	concurrent_queue<Buffer*> _buffers;
};

