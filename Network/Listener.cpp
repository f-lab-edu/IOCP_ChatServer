#include "stdafx.h"
#include "Listener.h"
#include "Session.h"
#include "IoEvent.h"
#include "IocpObject.h"


Listener::Listener(ServerService* service) 
{
	_listenSocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (_listenSocket == INVALID_SOCKET)
	{
		std::cout << WSAGetLastError() << std::endl;
	}
	_service = service;
}

void Listener::OnExecute(IoEvent* event, int SizeOfBytes)
{
	CompleteAccept((AcceptEvent*)event);
}

void Listener::StartAccept(int maxAccept)
{
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	IN_ADDR address;
	
	::InetPtonW(AF_INET, _service->GetIp(), &address);
	addr.sin_addr = address;
	addr.sin_port = htons(_service->GetPort());

	if (SOCKET_ERROR == ::bind(_listenSocket, reinterpret_cast<SOCKADDR*>(&addr), sizeof(SOCKADDR_IN))) {
		int errCode = WSAGetLastError();
		std::cout << "BIND ERROR: " << errCode;
		return;
	}
	
	if (SOCKET_ERROR == ::listen(_listenSocket, SOMAXCONN))
		return;

	auto key = shared_from_this();
	CreateIoCompletionPort((HANDLE)_listenSocket, _service->GetIocpHandle(), (ULONG_PTR)&key, 0);

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
	ioEvent->session = _service->CreateSession();

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
	inet_ntop(AF_INET,&sockAddr, (PSTR)& session->_ip, INET_ADDRSTRLEN);
	session->_port = htons(sockAddr.sin_port);
	
	session->CompletedConnect();

	RegisterAccept(acceptEvent);
}
