#pragma once
#include<string>

using namespace std;

#	pragma pack(push)
#	pragma pack(1)
struct PacketHeader
{
	unsigned short size;
	unsigned short packetId;
#ifdef _DEBUG
	clock_t sendTick;
#endif
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

#ifdef _DEBUG
public:
	clock_t GetSendTick(){return _header.sendTick; }
	void SetSendTick(clock_t tick){ _header.sendTick = tick; }
#endif

private:
	PacketHeader _header;
private:
	/* Write  */
	Buffer* _writeBuffer;
	//                 

private:
	/* Read  */
	char* _readBuffer;

private:
	unsigned char packetType = ePacketType::NONE;
	unsigned short _idx = 0;

public:
	template<typename T>
	inline void push(T& value)
	{
		if (packetType != ePacketType::WRITE_PACKET)
			/* ũ     */return;

		if (_idx + sizeof(T) > packetMaxSize)
			/* ũ     */return;

		memcpy(_writeBuffer->WritePos() + _idx, &value, sizeof(T));
		_idx += sizeof(T);
	}


	inline void push(string& value)
	{
		if (packetType != ePacketType::WRITE_PACKET)
			/* ũ     */return;

		if (value.size() > 255) return;

		unsigned char size = value.size();
		push(size);

		if (_idx + size > packetMaxSize)
			/* ũ     */ return;

		memcpy(_writeBuffer->WritePos() + _idx, value.c_str(), value.size());
		_idx += size;

	}

	inline void push(char* value, int size)
	{
		if (packetType != ePacketType::WRITE_PACKET)
			/* ũ     */return;
		
		if(_idx + size > packetMaxSize)
			/* ũ     */return;

		memcpy(_writeBuffer->WritePos() + _idx, value, size);
		_idx += size;
	}

public:

	template<typename T>
	inline void pop(T& value)
	{
		if (packetType != ePacketType::READ_PACKET)
			/* ũ     */return;

		if(_idx + sizeof(T) > _header.size)
			/* ũ     */return;
		
		memcpy(&value, _readBuffer + _idx, sizeof(T));
		_idx += sizeof(T);
	}

	inline void pop(string& value)
	{
		if (packetType != ePacketType::READ_PACKET)
			/* ũ     */return;

		unsigned char len;
		pop(len);
		
		if (_idx + len > _header.size)
			/* ũ     */return;

		value.append(static_cast<char*>(_readBuffer + _idx), len);
		_idx += len;
	}
};

