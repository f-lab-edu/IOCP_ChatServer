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


class Packet
{
public:
	Packet() = default;
	Packet(char* buffer);
	~Packet() = default;
public:
	void startPacket(unsigned short packetId);
	void endPacket(unsigned short packetId);

	char* GetBuffer() { return _data; }
	unsigned short GetSize() { return _header.size; }
	unsigned short GetPacketId() { return _header.packetId; }
private:
	PacketHeader _header;
	char* _data;

	// 전송할 데이터 끝
private:
	enum ePacketType
	{
		NONE,
		READ_PACKET,
		WRITE_PACKET
	};

	unsigned char packetType = ePacketType::NONE;
	unsigned short _idx = 0;

public:
	template<typename T>
	inline void push(T& value)
	{
		if (packetType != ePacketType::WRITE_PACKET)
			/* 크래시 */return;

		memcpy(_data + _idx, &value, sizeof(T));
		_idx += sizeof(T);
	}


	inline void push(string& value)
	{
		if (packetType != ePacketType::WRITE_PACKET)
			/* 크래시 */return;

		if (value.size() > 255) return;

		unsigned char size = value.size();
		push(size);

		memcpy(_data + _idx, value.c_str(), value.size());
		_idx += size;
	}

public:

	template<typename T>
	inline void pop(T& value)
	{
		if (packetType != ePacketType::READ_PACKET)
			/* 크래시 */return;

		memcpy(&value, _data + _idx, sizeof(T));
		_idx += sizeof(T);
	}

	inline void pop(string& value)
	{
		if (packetType != ePacketType::READ_PACKET)
			/* 크래시 */return;

		unsigned char len;
		pop(len);
		
		value.append(static_cast<char*>(_data + _idx), len);
		_idx += len;
	}
};

