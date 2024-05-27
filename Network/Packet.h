#pragma once


#	pragma pack(push)
#	pragma pack(1)
struct PacketHeader
{
	unsigned short size;
	unsigned short packetId;
#ifdef _DEBUG
#endif
#ifdef LATENCY_RECORD_OPTION
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

#ifdef LATENCY_RECORD_OPTION
	clock_t GetSendTick() {return _header.sendTick; }

	inline void SetSendTick(clock_t tick)
	{
		if(packetType != ePacketType::WRITE_PACKET)
		{
			std::cout << "set sendTick when packet type is not write";
			xassert(false);
			return;
		}
		_header.sendTick = tick;
		memcpy(_writeBuffer->WritePos() - _idx, &_header, sizeof(PacketHeader));
	}
#endif
	
private:
	PacketHeader _header;
private:
	/* Write용 */
	Buffer* _writeBuffer;
	// 전송할 데이터 끝

private:
	/* Read용 */
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
			std::cout << "push when startFlag:" + std::to_string(startFlag) + " endFlag: " + std::to_string(endFlag) << std::endl;
			xassert(false);
			return;
		}
		if (packetType != ePacketType::WRITE_PACKET)
		{
			std::cout << "push packet is not writeType";
			xassert(false);
			return;
		}
			

		if (_idx + sizeof(T) > packetMaxSize)
		{
			std::cout << "push greater than maximum size"; 
			xassert(false);
			return;
		}

		memcpy(_writeBuffer->WritePos() + _idx, &value, sizeof(T));
		_idx += sizeof(T);
	}


	inline void push(std::string& value)
	{
		if (startFlag == false || endFlag == true)
		{
            std::cout << "push when startFlag:" + std::to_string(startFlag) + " endFlag: " + std::to_string(endFlag) << std::endl;
            xassert(false);
            return;
        }

		if (packetType != ePacketType::WRITE_PACKET)
		{
			std::cout << "push packet is not writeType"; 
			xassert(false);
			return;
		}
		

		if (value.size() > UCHAR_MAX)
		{
			std::cout << "push std::string size is greater than ucharMax"; 
            xassert(false);
			return;
		}

		unsigned char size = static_cast<unsigned char>(value.size());
		push(size);

		if (_idx + size > packetMaxSize)
		{
        	std::cout << "push greater than maximum size"; 
        	xassert(false);
        	return;
        }

		memcpy(_writeBuffer->WritePos() + _idx, value.c_str(), value.size());
		_idx += size;

	}

	inline void push(char* value, int size)
	{
		if (startFlag == false || endFlag == true)
		{
			std::cout << "push when startFlag:" + std::to_string(startFlag) + " endFlag: " + std::to_string(endFlag) << std::endl;
			xassert(false);
			return;
		}

		if (packetType != ePacketType::WRITE_PACKET)
		{
        	std::cout << "push when packet is not writeType";
        	xassert(false);
        	return;
        }	
		
		
		if(_idx + size > packetMaxSize)
		{
			std::cout << "packetSize is greater than maximum";
			xassert(false);
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
			xassert(false);
			return;
		}

		if(_idx + sizeof(T) > _header.size)
		{
			std::cout << "pop greater than packet size";
			xassert(false);
			return;
        }
		
		
		memcpy(&value, _readBuffer + _idx, sizeof(T));
		_idx += sizeof(T);
	}

	inline void pop(std::string& value)
	{
		if (packetType != ePacketType::READ_PACKET)
		{
			std::cout << "pop when packet is not readType";
			xassert(false);
			return;
		}

		unsigned char len;
		pop(len);
		
		if (_idx + len > _header.size)
		{
			std::cout << "pop greater than packetSize";
			xassert(false);
			return;
		}

		value.append(static_cast<char*>(_readBuffer + _idx), len);
		_idx += len;
	}
};

