#include "stdafx.h"

#include "ClientService.h"

int main() {

	ClientService* service = new ClientService(L"127.0.0.1",7777,[]()
	{
		return make_shared<ServerSession>();
	},2);
	service->Open();
	
	GThreadManager->JoinAll();
} 