#include "stdafx.h"
#include <set>

DWORD WINAPI EchoThreadMain();

HANDLE iocpHandle;
int main() {

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

	std::set<std::shared_ptr<Session>> sessions;
	for (int i = 0; i < 1; i++) {
		std::shared_ptr<Session> session = std::make_shared<Session>(iocpHandle);
		session->Connect("127.0.0.1",7777);
		sessions.insert(session);
	}

	GThreadManager->JoinAll();

}

DWORD WINAPI EchoThreadMain() {

	while (1) {
		DWORD bytesTransffered = 0; 
		IoEvent* ioEvent = nullptr;
		ULONG_PTR key = 0;

		if (true == ::GetQueuedCompletionStatus(iocpHandle, &bytesTransffered,&key, reinterpret_cast<LPOVERLAPPED*>(&ioEvent), INFINITE)) {
			auto iocpObject = ioEvent->owner;
			iocpObject->OnExecute(ioEvent, bytesTransffered);
		}
		else {
			int errCode = ::WSAGetLastError();
			
			std::cout << std::system_category().message(errCode);
		}

	}
}		