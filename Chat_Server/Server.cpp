#include "stdafx.h"

DWORD WINAPI EchoThreadMain();

HANDLE iocpHandle;

int main() {
	g_Room = new Room();


	GameServerService* service = new GameServerService(L"127.0.0.1",7777,[](){
		return make_shared<ClientSession>();
	});
	service->Open();
	
	GThreadManager->JoinAll();
}

DWORD WINAPI EchoThreadMain() {
	while (1) {
		DWORD bytesTransffered = 0;
		IoEvent* ioEvent = nullptr;
		ULONG_PTR key = 0;
		if(::GetQueuedCompletionStatus(iocpHandle, &bytesTransffered, &key, reinterpret_cast<LPOVERLAPPED*>(&ioEvent), INFINITE))
		{
			std::shared_ptr<IocpObject> iocpObject = nullptr;
			iocpObject = ioEvent->owner;
	  			iocpObject->OnExecute(ioEvent, bytesTransffered);
		}
		else {
		 	int errCode = ::WSAGetLastError();
		 	std::cout << errCode;
			ioEvent->owner->OnExecute(ioEvent, 0);
		}
	}
}