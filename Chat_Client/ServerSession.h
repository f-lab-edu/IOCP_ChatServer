#pragma once
#include <string>
class Service;

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
	void Send(shared_ptr<Packet> p) override;
private:
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	void OnAssemblePacket(Packet* packet) override;
	
public:
	DWORD WINAPI ChattingLogic();
public:
	void AddLatency(unsigned short packetId, clock_t latency);
	void LatencyCheck(int sleepMs);
	
	void MeasureLatency(unsigned short packetId);


	string _nickName;

	multimap<unsigned short, clock_t> _latencys;
	mutex _latencyLock;
	int latencyAvgInterval = 1000;

};

