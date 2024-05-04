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
	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
	p->startPacket(Protocol::C2S_ENTER_ROOM);
	p->push(nickName);
	p->endPacket(Protocol::C2S_ENTER_ROOM);

	Send(move(p));

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

void ServerSession::AddLatency(clock_t latency)
{
	latencys.push_back(move(latency));

	if (latencys.size() >= latencyAvgInterval)
	{
		MeasureLatency();
	}
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

void ServerSession::MeasureLatency()
{
	auto avg = accumulate(latencys.begin(), latencys.end(), 0) / latencys.size();
	cout << "Latency avg: " << avg << endl;

	auto min = min_element(latencys.begin(), latencys.end());
	cout << "Latency min: " << *min << endl;

	auto max = max_element(latencys.begin(), latencys.end());
	cout << "Latency max: " << *max << endl;
	latencys.clear();
}
