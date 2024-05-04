#pragma once
#include <string>
#include <concurrent_vector.h>

class ServerSession : public Session
{
public:
	ServerSession(HANDLE iocp);

public:
	void DoDisconnect() override;

private:
	void OnConnected() override;
	void OnSend(int sendSize) override;
	void OnDisconnect() override;

	void OnAssemblePacket(Packet* packet) override;
	
public:
	void AddLatency(clock_t latency);
	void LatencyCheck(int sleepMs);
public:
	string nickName;
	
	concurrent_vector<clock_t> latencys;
	clock_t latencyMeasurementStartTick;
	clock_t latencyMeasurementInterval = 60000;
};

