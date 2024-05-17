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

void ServerSession::Send(shared_ptr<Packet> p)
{
	p->SetSendTick(clock());
	Session::Send(p);
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

	GThreadManager->ThreadStart([this]()
		{
			ServerSession::LatencyCheck(10);
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
	case Protocol::LATENCY_CHECK:
		PacketHandler::LATENCY_CHECK_Handler(session, packet);
		break;
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

		Sleep(1);
	}

	return 0;
}

void ServerSession::AddLatency(unsigned short packetId, clock_t latency)
{
	latencys.emplace(packetId,latency);

	if (latencys.count(packetId) >= latencyAvgInterval)
		MeasureLatency(packetId);
	
}

void ServerSession::LatencyCheck(int sleepMs)
{
	while (true/*TODO: disconnect flag*/)
	{
		clock_t tick = clock();
		shared_ptr<Packet> latency = make_shared<Packet>(ePacketType::WRITE_PACKET);
		latency->startPacket(Protocol::LATENCY_CHECK);
		latency->push(tick);
		latency->endPacket(Protocol::LATENCY_CHECK);
		Send(latency);
		
		Sleep(sleepMs);
	}
}

void ServerSession::MeasureLatency(unsigned short packetId)
{
	auto idRange = latencys.equal_range(packetId);
	
	auto avg = accumulate(idRange.first,idRange.second, 0,
		[](int x, const multimap<unsigned short,clock_t>::value_type& y)
		{
			return x + y.second;
		}) / latencys.size();
	cout << "Latency avg: " << avg << endl;

	auto min = min_element(idRange.first,idRange.second);
	cout << "Latency min: " << min->second << endl;

	auto max = max_element(idRange.first,idRange.second);
	cout << "Latency max: " << max->second << endl;
	latencys.clear();
}
