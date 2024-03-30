#include "stdafx.h"
#include "Packet.h"

Packet::Packet(char* buffer)
{
	packetType = ePacketType::READ_PACKET;

	_data = buffer;

	memcpy(&_header, _data, sizeof(PacketHeader));
	_idx += sizeof(PacketHeader);

}

void Packet::startPacket(unsigned short packetId)
{
	packetType = ePacketType::WRITE_PACKET;

	_idx += sizeof(PacketHeader);

	_header.packetId = packetId;

	_data = GThreadManager->GetSendBuffer(LThreadId)->WritePos();
}

void Packet::endPacket(unsigned short packetId)
{
	if (_header.packetId != packetId)
		/* Å©·¡½Ã */return;
	
	_header.size = _idx;
	memcpy(_data, &_header, sizeof(PacketHeader));
	unsigned short tmp = (short)*_data;
	GThreadManager->GetSendBuffer(LThreadId)->CompleteWrite(_idx);
}
