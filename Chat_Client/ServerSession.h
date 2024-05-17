#pragma once
#include <string>
class ServerSession : public Session
{
public:
	void SetTestFlag(bool flag) { isTestMode = flag; }
private:
	bool isTestMode;
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
	
	void MeasureLatency();
public:
	string nickName;
	
	vector<clock_t> latencys;
	int latencyAvgInterval = 1000;

public:
	DWORD WINAPI ChattingLogic();
public:
	string nickName;

};

