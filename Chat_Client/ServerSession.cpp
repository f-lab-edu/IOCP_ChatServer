#include "stdafx.h"
#include "ServerSession.h"



ServerSession::ServerSession(HANDLE iocp)
	: Session::Session(iocp)
{
}

void ServerSession::DoDisconnect()
{
	shared_ptr<Packet> p =make_shared<Packet>(ePacketType::WRITE_PACKET);

	p->startPacket(Protocol::C2S_EXIT_ROOM);
	p->endPacket(Protocol::C2S_EXIT_ROOM);

	Send(move(p));
}

void ServerSession::OnConnected()
{
	cout << "서버 연결 완료" << endl;

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
	p->startPacket(Protocol::C2S_ENTER_ROOM);
	p->push(nickName);
	p->endPacket(Protocol::C2S_ENTER_ROOM);

	Send(move(p));

	GThreadManager->ThreadStart([this]()
		{
			ChattingLogic();
		});
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

DWORD WINAPI ServerSession::ChattingLogic()
{
	while (1)
	{
		string chat;

		if (isTestMode)
			chat = "hello";
		else
			cin >> chat;

		if (chat.compare("q") == 0)
		{
			DoDisconnect();
			break;
		}

		shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
		p->startPacket(Protocol::C2S_CHAT_REQ);
		p->push(chat);
		p->endPacket(Protocol::C2S_CHAT_REQ);

		Send(move(p));

		Sleep(100);
	}

	return 0;
}

