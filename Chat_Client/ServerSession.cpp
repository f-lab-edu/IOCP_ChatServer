#include "stdafx.h"
#include "ServerSession.h"
void ServerSession::DoDisconnect()
{
	shared_ptr<Packet> p =make_shared<Packet>(ePacketType::WRITE_PACKET);

	p->startPacket(Protocol::C2S_EXIT_ROOM);
	p->endPacket(Protocol::C2S_EXIT_ROOM);

	Send(move(p));
}

void ServerSession::Send(shared_ptr<Packet> p)
{
#ifdef LATENCY_RECORD_OPTION 
	p->SetSendTick(clock());
#endif
	
	Session::Send(p);
}

void ServerSession::OnConnected()
{

	cout << "서버 연결 완료" << endl;

	shared_ptr<Packet> p = make_shared<Packet>(ePacketType::WRITE_PACKET);
	p->startPacket(Protocol::C2S_ENTER_ROOM);
	p->push(_nickName);
	p->endPacket(Protocol::C2S_ENTER_ROOM);

	Send(move(p));

	_nickName = "A_" + to_string(GetSessionId());

	GThreadManager->ThreadStart([this]()
		{
			ChattingLogic();
		});

#ifdef LATENCY_RECORD_OPTION
	GThreadManager->ThreadStart([this]()
		{
			ServerSession::LatencyCheck(1000);
		});
#endif
}

void ServerSession::OnSend(int sendSize)
{
}

void ServerSession::OnDisconnect()
{

}

void ServerSession::OnAssemblePacket(Packet* packet)
{
	clock_t curTick = clock();

	shared_ptr<Session> session = static_pointer_cast<ServerSession>(shared_from_this());

#ifdef LATENCY_RECORD_OPTION
	AddLatency(packet->GetPacketId(), clock() - packet->GetSendTick());
#endif
	
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

		Sleep(10);
	}

	return 0;
}

void ServerSession::AddLatency(unsigned short packetId, clock_t latency)
{
	int count = 0;
	{
		lock_guard<mutex> lock(_latencyLock);

		_latencys.emplace(packetId,latency);
		count = _latencys.count(packetId);
	}
	if  (count >= latencyAvgInterval)
		MeasureLatency(packetId);
	
}

void ServerSession::LatencyCheck(int sleepMs)
{
	while (true/*TODO: disconnect flag*/)
	{
		shared_ptr<Packet> latency = make_shared<Packet>(ePacketType::WRITE_PACKET);
		latency->startPacket(Protocol::LATENCY_CHECK);
		latency->endPacket(Protocol::LATENCY_CHECK);
		Send(latency);
		
		Sleep(sleepMs);
	}
}

void ServerSession::MeasureLatency(unsigned short packetId)
{

	long long avg = 0;
	int max = 0;
	int min = 0;

	{

		lock_guard lock(_latencyLock);
		auto idRange = _latencys.equal_range(packetId);
		avg = accumulate(idRange.first,idRange.second, 0,
			[](int x, const multimap<unsigned short,clock_t>::value_type& y)
			{
				return x + y.second;
			}) / _latencys.size();
		min  = min_element(idRange.first,idRange.second)->second;
		max = max_element(idRange.first,idRange.second)->second;
		_latencys.erase(idRange.first,idRange.second);

	}
	std::cout << _nickName << "-> packetId: " << packetId <<  " Latency avg: " << avg << endl;
	std::cout << _nickName << "-> packetId: " << packetId <<  " Latency min: " << min << endl;
	std::cout << _nickName << "-> packetId: " << packetId << " Latency max: " << max << endl;

}

