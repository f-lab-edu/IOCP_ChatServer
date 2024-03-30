#include "stdafx.h"


Session::Session(HANDLE iocpHandle) : _iocpHandle(iocpHandle), _recvEvent(EventType::Recv), _connectEvent(EventType::Connect), _recvBuffer(65535)
{
	_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (_socket == INVALID_SOCKET)
	{
		std::cout << WSAGetLastError() << std::endl;
	}

}

void Session::OnExecute(IoEvent* ioEvent, int SizeOfBytes)
{
	switch (ioEvent->m_eventType)
	{
	case EventType::Connect:
		CompletedConnect();
		break;
	case EventType::Send:
		CompletedSend(((SendEvent*)ioEvent)->thread_id, SizeOfBytes);
		break;
	case EventType::Recv:
		CompletedRecv(SizeOfBytes);
		break;
	case EventType::Disconnect:
		CompletedDisconnect();
		break;
	}
}

bool Session::RegisterConnect()
{
	return false;
}

bool Session::RegisterDisconnect()
{
	return false;
}

void Session::RegisterSend()
{
}

void Session::RegisterRecv()
{
	_recvEvent.Init();

	WSABUF wsaBuf;
	wsaBuf.buf = _recvBuffer.WritePos();
	wsaBuf.len = _recvBuffer.RemainSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;

	_recvEvent.owner = shared_from_this();
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, (OVERLAPPED*)&_recvEvent, nullptr)) {
		
		int errorCode = ::WSAGetLastError();
		if (errorCode != ERROR_IO_PENDING) {
			std::cout << "wsaError: " << errorCode << std::endl;
			RegisterRecv();
			// OR Disconnect;
			return;
		}
	}
}

void Session::CompletedConnect()
{

	RegisterRecv();
	OnConnected();
}

void Session::CompletedSend(int thread_id, int sizeOfBytes)
{
	OnSend(sizeOfBytes);
	GThreadManager->RenewalBuffer(thread_id, sizeOfBytes);
}

void Session::CompletedRecv(int sizeOfBytes)
{
	_recvBuffer.CompleteWrite(sizeOfBytes);
	
	int processLen = OnRecv();
	_recvBuffer.CompleteRead(processLen);
	
	RegisterRecv();
}

void Session::CompletedDisconnect()
{
	OnDisconnect();
}

void Session::Connect(std::string ip, int port)
{
	SOCKADDR_IN myAddr;
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddr.sin_port = ::htons(0);

	if (SOCKET_ERROR == ::bind(_socket, (SOCKADDR*)&myAddr, sizeof(myAddr))) {
		int errCode = WSAGetLastError();

		std::cout << "bind ERROR: " << errCode;
	}

	CreateIoCompletionPort((HANDLE)_socket, _iocpHandle,0, 0);


	DWORD numOfBytes = 0;
	SOCKADDR_IN targetAddr;
	::memset(&targetAddr, 0, sizeof(targetAddr));
	targetAddr.sin_family = AF_INET;
	inet_pton(AF_INET, (PCSTR)ip.c_str(), &targetAddr.sin_addr);
	targetAddr.sin_port = ::htons(port);


	_connectEvent.Init();
	_connectEvent.owner = shared_from_this();

	if (false == NetworkUtil::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&targetAddr), sizeof(SOCKADDR_IN), nullptr, 0, &numOfBytes, &_connectEvent)) {
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING) {
			std::cout << "wsaError: " << errorCode << std::endl;
			return;
		}
	}

}

void Session::Send(Packet p)
{
	
	WSABUF wsaBuf;
	::memset(&wsaBuf, 0, sizeof(WSABUF));
	wsaBuf.buf = p.GetBuffer();
	wsaBuf.len = p.GetSize();

	// 현재는 send가 동시에 여러번 발생하지 않는다고 가정하고 life-cycle을 가져갔습니다. send가 동시에 여러번 trigger 될 수 있게 고치려면 어떻게 해야 될지 판단해주세요.
	_sendEvent.Init();
	_sendEvent.owner = shared_from_this();
	_sendEvent.thread_id = LThreadId;

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, &wsaBuf, 1, & numOfBytes, 0, &_sendEvent, nullptr)) {
		
		int errorCode = ::WSAGetLastError();

		if (errorCode != ERROR_IO_PENDING) {
			std::cout << "wsaError: " << errorCode << std::endl;
			return;
		}
	}
}

void Session::OnConnected()
{

	string tmp = "AAAAA";
	Packet p;
	p.startPacket(0);
	p.push(tmp);
	p.endPacket(0);
	Send(p);
}

void Session::OnSend(int sendSize)
{
}

int Session::OnRecv()
{
	unsigned char packetHeaderSize = sizeof(PacketHeader);
	int processLen = 0;
	char* buffer = _recvBuffer.ReadPos();
	while (true)
	{
		if (_recvBuffer.DataSize() < packetHeaderSize)
			break;

		PacketHeader header;
		memcpy(&header, buffer, packetHeaderSize);

		if (_recvBuffer.DataSize() < header.size)
			break;

		Packet p(buffer);
		OnAssemblePacket(p);

		processLen += header.size;
		buffer += header.size;
	}

	return processLen;

}

void Session::OnDisconnect()
{
}
