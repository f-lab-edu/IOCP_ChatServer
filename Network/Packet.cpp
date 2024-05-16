#include "stdafx.h"
#include "Packet.h"

Packet::Packet(unsigned char type, char* buffer)
{
	if (type != ePacketType::READ_PACKET)
	{
    	std::cout << "packet is not readType in read constructor";
        xassert(false);
        return;
    }

	packetType = type;


	_readBuffer = buffer;

	memcpy(&_header, _readBuffer, sizeof(PacketHeader));
	_idx += sizeof(PacketHeader);
}

Packet::Packet(unsigned char type)
{
	if (type != ePacketType::WRITE_PACKET)
	{
		std::cout << "packet is not writeType in write constructor";
		xassert(false);
		return;
	}

	packetType = type;
	_writeBuffer = GBufferManager->AssignBuffer();
}

Packet::~Packet()
{
	if(packetType == ePacketType::WRITE_PACKET)
	{ 
		_writeBuffer->CompleteRead(_idx);
		GBufferManager->ReturnBuffer(_writeBuffer);
	}
}

void Packet::startPacket(int packetId)
{
	if (packetId > packetMaxId)
	{
		std::cout << "packetId is greater than maximum";
		xassert(false);
		return;
	}
	
	startFlag = true;
	_idx += sizeof(PacketHeader);
	_header.packetId = packetId;

}

void Packet::endPacket(int packetId)
{
	if (packetId > packetMaxId)
	{
    	std::cout << "packetId is greater than maximum";
    	xassert(false);
    	return;
    }

	if (_header.packetId != packetId)
	{
		std::cout << "packetId is not equal when endPacket";
        xassert(false);
        return;
    }
	
	endFlag = true;

	_header.size = _idx;
	memcpy(_writeBuffer->WritePos(), &_header, sizeof(PacketHeader));
	_writeBuffer->CompleteWrite(_idx);
}
