#pragma once


class NetworkUtil
{
public:
	static void Init();

	static LPFN_CONNECTEX ConnectEx;
	static LPFN_ACCEPTEX AcceptEx;
	static LPFN_DISCONNECTEX DisconnectEx;


};

