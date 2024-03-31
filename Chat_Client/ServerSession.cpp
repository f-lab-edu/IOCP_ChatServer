#include "stdafx.h"
#include "ServerSession.h"


void ServerSession::OnConnected()
{

}

void ServerSession::OnSend(int sendSize)
{
}

void ServerSession::OnDisconnect()
{
}

void ServerSession::OnAssemblePacket(Packet& packet)
{
	Session* session = this;
	switch (packet.GetPacketId())
	{
	case Protocol::S2C_ENTER_ROOM_NOTIFY:
		PacketHandler::S2C_ENTER_ROOM_NOTIFY_Handler(session, packet);
		break;
	case Protocol::S2C_CHAT_RES:
		PacketHandler::S2C_CHAT_RES_Handler(session, packet);
		break;
	case Protocol::S2C_EXIT_ROOM_NOTIFY:
		PacketHandler::S2C_EXIT_ROOM_NOTIFY_Handler(session, packet);
		break;
	}
}
