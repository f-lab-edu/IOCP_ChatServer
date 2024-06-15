#pragma once
#include <string>
#include "Scenario.h"

class Service;

struct tmpPlayerInfo
{
	int playerId;
	int x;
	int y;
};
class ServerSession : public Session
{
public:
	void SetTestFlag(bool flag) { isTestMode = flag; }
private:
	bool isTestMode;
public:
	ServerSession() = default;
	~ServerSession() = default;

	HANDLE GetHandle() override { return reinterpret_cast<HANDLE>(GetSocket()); }

public:
	void DoDisconnect() override;
	void Connect(const WCHAR* ip, int port) override;
	void Send(shared_ptr<Packet> p) override;
private:
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	void OnAssemblePacket(std::shared_ptr<Packet> packet) override;
	
public:
	void AddLatency(unsigned short packetId, clock_t latency);
	void LatencyCheck(int sleepMs);
	void MeasureLatency(unsigned short packetId);

	tmpPlayerInfo playerInfo;
	vector<int> _enterPlayerId;

	string _nickName;
private:
	multimap<unsigned short, clock_t> _latencys;
	mutex _latencyLock;
	int latencyAvgInterval = 1000;

	Scenario _scenario;
};

