#include "stdafx.h"
#include "NetworkUtil.h"


LPFN_CONNECTEX NetworkUtil::ConnectEx = nullptr;
LPFN_ACCEPTEX NetworkUtil::AcceptEx = nullptr;

void NetworkUtil::Init()
{
	WSADATA wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) != 0)
		/* Å©·¡½Ã */return;

	DWORD bytes = 0;
	GUID guid = WSAID_CONNECTEX;
	SOCKET dummySocket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	WSAIoctl(dummySocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), (LPDWORD*)&NetworkUtil::ConnectEx, sizeof(NetworkUtil::ConnectEx), &bytes, NULL, NULL);

	guid = WSAID_ACCEPTEX;
	WSAIoctl(dummySocket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), (LPDWORD*)&NetworkUtil::AcceptEx, sizeof(NetworkUtil::AcceptEx), &bytes, NULL, NULL);
}
