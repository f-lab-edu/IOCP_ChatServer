#include "stdafx.h"
#include "Listener.h"
#include "Session.h"
#include "IoEvent.h"
#include "IocpObject.h"

Listener::Listener(HANDLE iocpHandle) : _iocpHandle(iocpHandle)
{
	_listenSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (_listenSocket == INVALID_SOCKET)
	{
		std::cout << WSAGetLastError() << std::endl;
	}
}

void Listener::OnExecute(IoEvent* event, int SizeOfBytes)
{
	CompleteAccept((AcceptEvent*)event);
}

void Listener::StartAccept(int maxAccept)
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	addr.sin_port = htons(7777);

	if (SOCKET_ERROR == ::bind(_listenSocket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(SOCKADDR_IN))) {
		int errCode = WSAGetLastError();
		std::cout << "BIND ERROR: " << errCode;
		return;
	}
	
	if (SOCKET_ERROR == ::listen(_listenSocket, SOMAXCONN))
		return;

	auto key = shared_from_this();
	CreateIoCompletionPort((HANDLE)_listenSocket, _iocpHandle, (ULONG_PTR)&key, 0);

	for (int i = 0; i < maxAccept; i++) {
		AcceptEvent* acceptEvent = new AcceptEvent();
		acceptEvent->owner = key;
		_acceptEvents.push_back(acceptEvent);
		
		RegisterAccept(acceptEvent);
	}
}

void Listener::RegisterAccept(AcceptEvent* ioEvent)
{
	ioEvent->Init();
	ioEvent->session = std::make_shared<Session>(_iocpHandle);
	ULONG_PTR key = 0;
	CreateIoCompletionPort((HANDLE)ioEvent->session->GetSocket(), _iocpHandle, (ULONG_PTR)&key, 0);

	// ToDO 나중에 보관
	DWORD bytesRecevied = 0;
	if (false == NetworkUtil::AcceptEx(_listenSocket, ioEvent->session->GetSocket(), ioEvent->session->GetRecvBuffer()->WritePos(), 0,
		sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16,
		&bytesRecevied, ioEvent))
	{
		const int errorCode = ::WSAGetLastError();

		if (errorCode != ERROR_IO_PENDING) {
			std::cout << "wsaError: " << errorCode << std::endl;
			return;
		}
	}
}

void Listener::CompleteAccept(AcceptEvent* acceptEvent)
{

	std::shared_ptr<Session> session = acceptEvent->session;
 	if(SOCKET_ERROR == ::setsockopt(session->GetSocket(), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, reinterpret_cast<char*>(&_listenSocket), sizeof(_listenSocket)))
	{
		int errCode = WSAGetLastError();
		std::cout << "setSockopt ERROR: " << errCode;
	}
	
	std::cout << "ACCEPT!!" << std::endl;
	SOCKADDR_IN sockAddr;
	int sizeOfSockAddr = sizeof(sockAddr);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddr), &sizeOfSockAddr))
	{
		int errCode = WSAGetLastError();

		std::cout << "Getpeername ERROR: " << errCode;
		return; 
	}
	inet_ntop(AF_INET,&sockAddr, (PSTR)& session->ip, INET_ADDRSTRLEN);
	session->port = htons(sockAddr.sin_port);

	session->RegisterRecv();
	RegisterAccept(acceptEvent);
}
