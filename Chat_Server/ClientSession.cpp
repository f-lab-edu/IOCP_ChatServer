#include "stdafx.h"
#include "ClientSession.h"

void ClientSession::OnConnected()
{

}

void ClientSession::OnSend(int sendSize)
{
}

void ClientSession::OnDisconnect()
{
}

void ClientSession::OnAssemblePacket(Packet& packet)
{
	Session* session = this;

	switch (packet.GetPacketId())
	{
	case Protocol::C2S_ENTER_ROOM:
		PacketHandler::C2S_ENTER_ROOM_Handler(session, packet);
		break;
	case Protocol::C2S_CHAT_REQ:
		PacketHandler::C2S_CHAT_REQ_Handler(session, packet);
		break;
	case Protocol::C2S_EXIT_ROOM:
		PacketHandler::C2S_EXIT_ROOM_Handler(session, packet);
		break;
	default:
		break;
	}
}
