#include "stdafx.h"
#include "Buffer.h"

Buffer::Buffer(int bufferSize)
	:_writePos(0), _readPos(0)
{

	_buffer = new char[bufferSize] { 0, };

	_bufferSize = bufferSize;
}

Buffer::~Buffer()
{
	delete _buffer;
}

void Buffer::CompleteRead(int readByte)
{
	if (_readPos + readByte > _bufferSize)
	{
		// 크래시 내기
		return;
	}

	_readPos += readByte;

	Clear();
		
}

void Buffer::CompleteWrite(int writeByte)
{
	if (_writePos + writeByte > _bufferSize)
	{
		// 크래시 내기
		return;
	}

	_writePos += writeByte;
}

void Buffer::Clear()
{
	if (_writePos == _readPos)
		_writePos = _readPos = 0;
}

char* Buffer::Reserve(int size)
{
	char* reservePos = WritePos();
	_writePos += size;

	return reservePos;
}
