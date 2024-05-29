#include "stdafx.h"
#include "ServerService.h"

ServerService::ServerService(const WCHAR* ip, int port, SessionFactory factory)
    :Service(ip,port,factory)
{
}

void ServerService::Open()
{
    Service::Open();
    _listener = std::make_shared<Listener>(this);

    _listener->StartAccept(5);
}

void ServerService::Close()
{
    Service::Close();
}
