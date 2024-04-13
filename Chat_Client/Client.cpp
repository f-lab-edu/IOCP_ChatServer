#include "stdafx.h"

DWORD WINAPI WriteThreadMain(shared_ptr<ServerSession> session);
DWORD WINAPI IocpThreadMain();

HANDLE iocpHandle;
int main() {


	WSADATA wsaData;
	if (0 != ::WSAStartup(MAKEWORD(2, 2), &wsaData))
		return 0;

	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	std::shared_ptr<ServerSession> session = std::make_shared<ServerSession>(iocpHandle);

	cin >> session->nickName;

	session->Connect("127.0.0.1", 7777);

	if (iocpHandle == INVALID_HANDLE_VALUE) {
		std::cout << "iocpHanlde Invalid";
		return 0;
	}	



	for (int i = 0; i < 1; i++)
		GThreadManager->ThreadStart(IocpThreadMain);
	
	GThreadManager->ThreadStart([&session]()
		{
			WriteThreadMain(session);
		});


	GThreadManager->JoinAll();

}

DWORD WINAPI WriteThreadMain(shared_ptr<ServerSession> session)
{

	while (1)
	{
		string chat;
		cin >> chat;

		if (chat.compare("q") == 0)
		{
			session->DoDisconnect();
			break;
		}

		Packet* p = new Packet(ePacketType::WRITE_PACKET,session->GetSendBuffer());
		p->startPacket(Protocol::C2S_CHAT_REQ);
		p->push(chat);
		p->endPacket(Protocol::C2S_CHAT_REQ);

		session->Send(p);
	}

	return 0;
}

DWORD WINAPI IocpThreadMain() {

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