#include "stdafx.h"
#include "RingBuffer.h"

RingBuffer::RingBuffer(int bufferSize)
	:_writePos(0), _readPos(0)
{
	//if (bufferSize > 65535)
	//{
	//	// 크래시 내기
	//	return;
	//}

	_buffer = new char[bufferSize] { 0, };

	_bufferSize = bufferSize;
}

RingBuffer::~RingBuffer()
{
	delete _buffer;
}

void RingBuffer::CompleteRead(int readByte)
{
	if (_readPos + readByte > _bufferSize)
	{
		// 크래시 내기
		return;
	}

	_readPos += readByte;

	if (_writePos == _readPos)
		_writePos = _readPos = 0;
		
}

void RingBuffer::CompleteWrite(int writeByte)
{
	if (_writePos + writeByte > _bufferSize)
	{
		// 크래시 내기
		return;
	}

	_writePos += writeByte;
}

char* RingBuffer::Reserve(int size)
{
	char* reservePos = WritePos();
	_writePos += size;

	return reservePos;
}
