#pragma once
#include<string>

using namespace std;

#	pragma pack(push)
#	pragma pack(1)
struct PacketHeader
{
	unsigned short size;
	unsigned short packetId;
};
#	pragma pack(pop)

enum ePacketType : unsigned char
{
	NONE,
	READ_PACKET,
	WRITE_PACKET
};

const int packetMaxSize = 65535;
const int packetMaxId = 65535;

class Packet
{
public:
	Packet(unsigned char type, char* buffer);
	Packet(unsigned char type);
	~Packet();
public:
	void startPacket(int packetId);
	void endPacket(int packetId);

	Buffer* GetBuffer() { return _writeBuffer; }
	unsigned short GetSize() { return _header.size; }
	unsigned short GetPacketId() { return _header.packetId; }
private:
	PacketHeader _header;
private:
	/* Write용*/
	Buffer* _writeBuffer;
	// 전송할 데이터 끝

private:
	/* Read용*/
	char* _readBuffer;

private:
	unsigned char packetType = ePacketType::NONE;
	unsigned short _idx = 0;

public:
	template<typename T>
	inline void push(T& value)
	{
		if (packetType != ePacketType::WRITE_PACKET)
			/* 크래시 */return;

		if (_idx + sizeof(T) > packetMaxSize)
			/* 크래시 */return;

		memcpy(_writeBuffer->WritePos() + _idx, &value, sizeof(T));
		_idx += sizeof(T);
	}


	inline void push(string& value)
	{
		if (packetType != ePacketType::WRITE_PACKET)
			/* 크래시 */return;

		if (value.size() > 255) return;

		unsigned char size = value.size();
		push(size);

		if (_idx + size > packetMaxSize)
			/* 크래시 */ return;

		memcpy(_writeBuffer->WritePos() + _idx, value.c_str(), value.size());
		_idx += size;

	}

	inline void push(char* value, int size)
	{
		if (packetType != ePacketType::WRITE_PACKET)
			/* 크래시 */return;
		
		if(_idx + size > packetMaxSize)
			/* 크래시 */return;

		memcpy(_writeBuffer->WritePos() + _idx, value, size);
		_idx += size;
	}

public:

	template<typename T>
	inline void pop(T& value)
	{
		if (packetType != ePacketType::READ_PACKET)
			/* 크래시 */return;

		if(_idx + sizeof(T) > _header.size)
			/* 크래시 */return;
		
		memcpy(&value, _readBuffer + _idx, sizeof(T));
		_idx += sizeof(T);
	}

	inline void pop(string& value)
	{
		if (packetType != ePacketType::READ_PACKET)
			/* 크래시 */return;

		unsigned char len;
		pop(len);
		
		if (_idx + len > _header.size)
			/* 크래시 */return;

		value.append(static_cast<char*>(_readBuffer + _idx), len);
		_idx += len;
	}
};

