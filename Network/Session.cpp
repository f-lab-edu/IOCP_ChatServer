﻿#include "stdafx.h"


Session::Session() : _recvEvent(EventType::Recv), 
_connectEvent(EventType::Connect), _disconnectEvent(EventType::Disconnect)
, _recvBuffer(65535), _isSendRegister(false)
{
	_socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	xassert((_socket == INVALID_SOCKET))

}

Session::~Session()
{
	::closesocket(_socket);
}

void Session::OnExecute(IoEvent* ioEvent, int SizeOfBytes)
{

	switch (ioEvent->m_eventType)
	{
	case EventType::Connect:
		CompletedConnect();
		break;
	case EventType::Send:
		CompletedSend(SizeOfBytes);
		break;
	case EventType::Recv:
		CompletedRecv(SizeOfBytes);
		break;
	case EventType::Disconnect:
		CompletedDisconnect();
		break;
	}
}

void Session::RegisterConnect()
{
	SOCKADDR_IN myAddr;
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddr.sin_port = ::htons(0);

	if (SOCKET_ERROR == ::bind(_socket, (SOCKADDR*)&myAddr, sizeof(myAddr)))
	{
		int errCode = WSAGetLastError();

		std::cout << "bind ERROR: " << errCode;
	}

	DWORD numOfBytes = 0;
	SOCKADDR_IN targetAddr;
	::memset(&targetAddr, 0, sizeof(targetAddr));
	targetAddr.sin_family = AF_INET;	
	IN_ADDR address;
	
	::InetPtonW(AF_INET, _service->GetIp(), &address);
	targetAddr.sin_addr = address;

	targetAddr.sin_port = ::htons(_port);


	_connectEvent.Init();
	_connectEvent.owner = shared_from_this();

	if (false == NetworkUtil::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&targetAddr), sizeof(SOCKADDR_IN), nullptr, 0, &numOfBytes, &_connectEvent))
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			std::cout << "wsaError: " << errorCode << std::endl;
			DoDisconnect();
			return;
		}
	}

}

void Session::RegisterDisconnect()
{
	_disconnectEvent.Init();

	if (false == NetworkUtil::DisconnectEx(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			std::cout << "wsaError: " << errorCode << std::endl;
			DoDisconnect();
			return;
		}
	}
}

void Session::RegisterSend()
{
	_sendEvent.buffers.clear();

	_sendEvent.Init();
	_sendEvent.owner = shared_from_this();

	while (!_sendRegisteredPacket.empty())
	{
		shared_ptr<Packet> p;
		
		if (true == _sendRegisteredPacket.try_pop(p))
		{
			WSABUF buf;
			buf.buf = p->GetBuffer()->ReadPos();
			buf.len = p->GetSize();

			_sendEvent.buffers.push_back(buf);
			_sendCompletePacket.push_back(move(p));
		}
	}

	DWORD numOfBytes = 0;
  
	auto result = ::WSASend(_socket, _sendEvent.buffers.data(),static_cast<DWORD>(_sendEvent.buffers.size()), &numOfBytes, 0, &_sendEvent, nullptr);
	if (result == 0)
		CompletedSend(numOfBytes);
	else if (SOCKET_ERROR == result)
	{

		int errorCode = ::WSAGetLastError();

		if (errorCode != ERROR_IO_PENDING)
		{
			std::cout << "wsaError: " << errorCode << std::endl;
			DoDisconnect();
			return;
		}
	}
	
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

	auto result = ::WSARecv(_socket, &wsaBuf, 1, &numOfBytes, &flags, (OVERLAPPED*)&_recvEvent, nullptr);
	if (result == 0)
		CompletedRecv(numOfBytes);
	else if (SOCKET_ERROR == result) {
		
		int errorCode = ::WSAGetLastError();
		if (errorCode != ERROR_IO_PENDING) {
			std::cout << "wsaError: " << errorCode << std::endl;
			DoDisconnect();
			return;
		}
	}
}

void Session::CompletedConnect()
{
	OnConnected();
	
	RegisterRecv();
}

void Session::CompletedSend(int sizeOfBytes)
{
	if (sizeOfBytes == 0)
	{
		DoDisconnect();
		return;
	}

	OnSend(sizeOfBytes);

	_sendCompletePacket.clear();

	/*if(_sendRegisteredPacket.empty() == false)
		RegisterSend();
	else*/
		_isSendRegister.store(false);
}

void Session::CompletedRecv(int sizeOfBytes)
{
	if (sizeOfBytes == 0)
	{
		DoDisconnect();
		return;
	}

	_recvBuffer.CompleteWrite(sizeOfBytes);

	int processLen = OnRecv();
	_recvBuffer.CompleteRead(processLen);

	RegisterRecv();
}

void Session::CompletedDisconnect()
{
	OnDisconnect();
}

void Session::Connect(const WCHAR* ip, int port)
{
	memcpy(_ip, ip, sizeof(_ip));
	_port = port;

	RegisterConnect();
}

void Session::Send(shared_ptr<Packet> p)
{
	if (_isDisconnect == true) 
		return;
  
	if (p->GetStartFlag() == false || p->GetEndFlag() == false)
		/* 크래시 */return;


	bool Flush = false;
	
	_sendRegisteredPacket.push(move(p));

	bool expected = false;
	if (_isSendRegister.compare_exchange_strong(expected, true))
		Flush = true;
	
	if (Flush == true)
		RegisterSend();
}

void Session::DoDisconnect()
{
	if (_isDisconnect == true)
		return;
	
	::shutdown(_socket, SD_BOTH);
	RegisterDisconnect();
	_isDisconnect = true;
}


int Session::OnRecv()
{
	unsigned char packetHeaderSize = sizeof(PacketHeader);
	int processLen = 0;
	char* buffer = _recvBuffer.ReadPos();

	int recvSize = _recvBuffer.DataSize();
	while (true)
	{
		if (recvSize - processLen < packetHeaderSize)
			break;

		PacketHeader header;
		memcpy(&header, buffer + processLen, packetHeaderSize);

		if (recvSize - processLen < header.size)
			break;

		shared_ptr<Packet> p = make_shared<Packet>(ePacketType::READ_PACKET, buffer + processLen);
		p->SetOwner(this);
		_service->PushJob(move(p));

		processLen += header.size;
	}

	return processLen;
}


