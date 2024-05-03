#include "stdafx.h"
#include "Packet.h"

Packet::Packet(unsigned char type, char* buffer)
{
	if (type != ePacketType::READ_PACKET)
		/*크래시 */ return;

	packetType = type;


	_readBuffer = buffer;

	memcpy(&_header, _readBuffer, sizeof(PacketHeader));
	_idx += sizeof(PacketHeader);
}

Packet::Packet(unsigned char type)
{
	if (type != ePacketType::WRITE_PACKET)
		return;

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
		/*크래시 */ return;

	_idx += sizeof(PacketHeader);
	_header.packetId = packetId;
}

void Packet::endPacket(int packetId)
{
	if (packetId > packetMaxId)
		/*크래시 */ return;

	if (_header.packetId != packetId)
		/* 크래시 */return;
	
	_header.size = _idx;
	memcpy(_writeBuffer->WritePos(), &_header, sizeof(PacketHeader));
	_writeBuffer->CompleteWrite(_idx);
}
