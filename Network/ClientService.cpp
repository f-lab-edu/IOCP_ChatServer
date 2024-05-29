#include "stdafx.h"
#include "ClientService.h"

ClientService::ClientService(const WCHAR* ip, int port, SessionFactory factory, int dummyCount)
    :Service(ip,port,factory), _dummyClientCount(dummyCount)
{
    
}

ClientService::~ClientService()
{
}

void ClientService::Open()
{
    Service::Open();
	for (int i = 0; i < _dummyClientCount; i++)
	{
		std::shared_ptr<Session> session = CreateSession();
		
		session->Connect(GetIp(),GetPort());
		cout << "서버와 연결 중입니다." << endl;
		_dummyClients.push_back(move(session));
	}
}

void ClientService::Close()
{
	Service::Close();
	_dummyClients.clear();
}
