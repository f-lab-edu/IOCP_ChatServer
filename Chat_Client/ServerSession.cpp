#include "stdafx.h"
#include "ServerSession.h"
void ServerSession::DoDisconnect()
{
	_scenario.ScenarioEnd();
}

void ServerSession::Connect(const WCHAR* ip, int port)
{
	_nickName = "A_" + to_string(GetSessionId());

	Session::Connect(ip, port);

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
	isTestMode = true;

	cout << "서버 연결 완료" << endl;


	_scenario.Init();
	_scenario.SetOwner(static_pointer_cast<ServerSession>(shared_from_this()));
	_scenario.StartScenario();
	
#ifdef LATENCY_RECORD_OPTION
	/*GThreadManager->ThreadStart([this]()
		{
			ServerSession::LatencyCheck(1000);
		});*/
#endif
}

void ServerSession::OnSend(int sendSize)
{
}

void ServerSession::OnDisconnect()
{

}

void ServerSession::OnAssemblePacket(std::shared_ptr<Packet> packet)
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
	case Protocol::S2C_ENTER_MAP_NOTIFY:
		PacketHandler::S2C_ENTER_MAP_NOTIFY_Handler(session, packet);
		break;
	case Protocol::S2C_CHAT_RES:
		PacketHandler::S2C_CHAT_RES_Handler(session, packet);
		break;
	case Protocol::S2C_EXIT_MAP_NOTIFY:
		PacketHandler::S2C_EXIT_MAP_NOTIFY_Handler(session, packet);
		break;
	}

	_scenario.NextScenario();
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
	shared_ptr<Packet> latency = make_shared<Packet>(ePacketType::WRITE_PACKET);
	latency->startPacket(Protocol::LATENCY_CHECK);
	latency->endPacket(Protocol::LATENCY_CHECK);
		
	while (true/*TODO: disconnect flag*/)
	{
		Send(latency);

		std::chrono::milliseconds sleep_ms(sleepMs);
		this_thread::sleep_for(sleep_ms);
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

