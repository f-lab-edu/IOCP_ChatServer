#include "BufferManager.h"

BufferManager::BufferManager(int bufferNum)
{
	for(int i = 0; i < bufferNum; i++)
		_buffers.push(new Buffer(bufferSize));
}

BufferManager::~BufferManager()
{
	while (!_buffers.empty())
	{
		Buffer* buffer;
		if (_buffers.try_pop(buffer))
			delete buffer;
	}
}

Buffer* BufferManager::AssignBuffer()
{
	if (_buffers.empty())
		return new Buffer(bufferSize);

	Buffer* buf;
	while (!_buffers.try_pop(buf))
	{
	}

	return buf;
}
