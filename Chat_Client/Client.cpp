#include "stdafx.h"

DWORD WINAPI WriteThreadMain(shared_ptr<ServerSession> session);
DWORD WINAPI IocpThreadMain();

HANDLE iocpHandle;

vector<std::shared_ptr<ServerSession>> dummyClients;
int main() {

	WSADATA wsaData;
	if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    string nickName;
	cout << "닉네임을 입력해주세요: ";
	cin >> nickName;

	for (int i = 0; i < 20; i++)
	{
		std::shared_ptr<ServerSession> session = std::make_shared<ServerSession>(iocpHandle);
		session->nickName = nickName + "_" + to_string(i);
		session->SetTestFlag(i != 0);
        
		session->Connect("127.0.0.1", 7777);
	    cout << "서버와 연결 중입니다." << endl;
		dummyClients.push_back(move(session));
	}



	if (iocpHandle == INVALID_HANDLE_VALUE) {
		std::cout << "iocpHanlde Invalid";
		return 0;
	}	



	for (int i = 0; i < 1; i++)
		GThreadManager->ThreadStart(IocpThreadMain);
	
	


	GThreadManager->JoinAll();

}

DWORD WINAPI IocpThreadMain() {

	while (1) {
		DWORD bytesTransffered = 0; 
		IoEvent* ioEvent = nullptr;
		ULONG_PTR key = 0;

		if (true == static_cast<bool>(::GetQueuedCompletionStatus(iocpHandle, &bytesTransffered,&key, reinterpret_cast<LPOVERLAPPED*>(&ioEvent), INFINITE))) {
			auto iocpObject = ioEvent->owner;
 			iocpObject->OnExecute(ioEvent, bytesTransffered);
		}
		else {
			int errCode = ::WSAGetLastError();
			std::cout << std::system_category().message(errCode);
		}

	}
}		