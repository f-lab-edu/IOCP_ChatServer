#pragma once


const long bufferSize = 196605;

class Buffer;

class BufferManager
{
public:
	BufferManager(int bufferNum = 5);
	~BufferManager();
public:
	Buffer* AssignBuffer();
	void ReturnBuffer(Buffer* buffer);
	
private:
	Concurrency::concurrent_queue<Buffer*> _buffers;
};

