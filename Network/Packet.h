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
	/* Write��*/
	Buffer* _writeBuffer;
	// ������ ������ ��

private:
	/* Read��*/
	char* _readBuffer;
	
public:
	bool GetStartFlag() { return startFlag; }
	bool GetEndFlag() { return endFlag; }
private:
	bool startFlag = false;
	bool endFlag = false;
private:
	unsigned char packetType = ePacketType::NONE;
	unsigned short _idx = 0;

public:
	template<typename T>
	inline void push(T& value)
	{
		if(startFlag == false || endFlag == true)
		{
			std::cout << "push when startFlag:" + to_string(startFlag) + " endFlag: " + to_string(endFlag) << std::endl;
			assert(false);
			return;
		}
		if (packetType != ePacketType::WRITE_PACKET)
		{
			std::cout << "push packet is not writeType"; 
			assert(false);
			return;
		}

		if (_idx + sizeof(T) > packetMaxSize)
		{
			std::cout << "push greater than maximum size"; 
			assert(false);
			return;
		}

		memcpy(_writeBuffer->WritePos() + _idx, &value, sizeof(T));
		_idx += sizeof(T);
	}


	inline void push(string& value)
	{
		if (startFlag == false || endFlag == true)
		{
            std::cout << "push when startFlag:" + to_string(startFlag) + " endFlag: " + to_string(endFlag) << std::endl;
            assert(false);
            return;
        }

		if (packetType != ePacketType::WRITE_PACKET)
		{
			std::cout << "push packet is not writeType"; 
			assert(false);
			return;
		}
		

		if (value.size() > UCHAR_MAX)
		{
			std::cout << "push string size is greater than ucharMax"; 
            assert(false);
			return;
		}
		unsigned char size = static_cast<unsigned char>(value.size());
		push(size);

		if (_idx + size > packetMaxSize)
		{
        	std::cout << "push greater than maximum size"; 
        	assert(false);
        	return;
        }

		memcpy(_writeBuffer->WritePos() + _idx, value.c_str(), value.size());
		_idx += size;

	}

	inline void push(char* value, int size)
	{
		if (startFlag == false || endFlag == true)
		{
			std::cout << "push when startFlag:" + to_string(startFlag) + " endFlag: " + to_string(endFlag) << std::endl;
			assert(false);
			return;
		}

		if (packetType != ePacketType::WRITE_PACKET)
		{
        	std::cout << "push when packet is not writeType";
        	assert(false);
        	return;
        }	
		
		
		if(_idx + size > packetMaxSize)
		{
			std::cout << "packetSize is greater than maximum";
			assert(false);
			return;
		}

		memcpy(_writeBuffer->WritePos() + _idx, value, size);
		_idx += size;
	}

public:

	template<typename T>
	inline void pop(T& value)
	{
		if (packetType != ePacketType::READ_PACKET)
		{
			std::cout << "pop when packet is not readType";
			assert(false);
			return;
		}

		if(_idx + sizeof(T) > _header.size)
		{
			std::cout << "pop greater than packet size";
			assert(false);
			return;
        }
		
		
		memcpy(&value, _readBuffer + _idx, sizeof(T));
		_idx += sizeof(T);
	}

	inline void pop(string& value)
	{
		if (packetType != ePacketType::READ_PACKET)
		{
			std::cout << "pop when packet is not readType";
			assert(false);
			return;
		}

		unsigned char len;
		pop(len);
		
		if (_idx + len > _header.size)
		{
			std::cout << "pop greater than packetSize";
			assert(false);
			return;
		}

		value.append(static_cast<char*>(_readBuffer + _idx), len);
		_idx += len;
	}
};

