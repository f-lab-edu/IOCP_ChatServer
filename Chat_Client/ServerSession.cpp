#include "stdafx.h"
#include "ServerSession.h"



ServerSession::ServerSession(HANDLE iocp)
	: Session::Session(iocp)
{
}

void ServerSession::DoDisconnect()
{
	shared_ptr<Packet> p = new shared_ptr<Packet>(ePacketType::WRITE_PACKET, GetSendBuffer());

	p->startPacket(Protocol::C2S_EXIT_ROOM);
	p->endPacket(Protocol::C2S_EXIT_ROOM);

	Send(move(p));
}

void ServerSession::OnConnected()
{
	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET, GetSendBuffer());
	p->startPacket(Protocol::C2S_ENTER_ROOM);
	p->push(nickName);
	p->endPacket(Protocol::C2S_ENTER_ROOM);

	Send(move(p));
}

void ServerSession::OnSend(int sendSize)
{
}

void ServerSession::OnDisconnect()
{

}

void ServerSession::OnAssemblePacket(Packet* packet)
{
	shared_ptr<Session> session = static_pointer_cast<ServerSession>(shared_from_this());

	switch (packet->GetPacketId())
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
