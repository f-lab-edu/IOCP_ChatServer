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

Packet::Packet(unsigned char type, Buffer* buffer)
{
	if (type != ePacketType::WRITE_PACKET)
		/*크래시 */ return;

	packetType = type;

	_writeBuffer = buffer;
}

void Packet::startPacket(unsigned short packetId)
{
	_idx += sizeof(PacketHeader);
	_header.packetId = packetId;
}

void Packet::endPacket(unsigned short packetId)
{
	if (_header.packetId != packetId)
		/* 크래시 */return;
	
	_header.size = _idx;
	memcpy(_writeBuffer->WritePos(), &_header, sizeof(PacketHeader));
	_writeBuffer->CompleteWrite(_idx);
}
