﻿#pragma once
class Buffer
{
public:
	Buffer(int bufferSize);
	~Buffer();

public:
	char* ReadPos() { 
		return (_buffer + _readPos); 
	}
	char* WritePos() { 
		return (_buffer + _writePos); 
	}

	int RemainSize() { 
		return _bufferSize - _writePos; 
	}
	int UsedSize() {
		return _writePos; 
	}
	int DataSize() { 
		return _writePos - _readPos; 
	}

	void CompleteRead(int readByte);
	void CompleteWrite(int writeByte);

	void Clear();
	char* Reserve(int size);
private:
	int _readPos;
	int _writePos;

	char* _buffer;
	int _bufferSize;
};

