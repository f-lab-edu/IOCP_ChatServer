#include "stdafx.h"

DWORD WINAPI EchoThreadMain();

HANDLE iocpHandle;

int main() {
	g_Room = new Room();

	WSADATA wsaData;
	if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);


	if (iocpHandle == INVALID_HANDLE_VALUE) {
		std::cout << "iocpHanlde Invalid"; 
		return 0;
	}

	for (int i = 0; i < 1; i++)
		GThreadManager->ThreadStart(EchoThreadMain);


	std::shared_ptr<Listener> listener = std::make_shared<Listener>(iocpHandle, [=]()
		{
			return make_shared<ClientSession>(iocpHandle);
		});
	listener->StartAccept(1);

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