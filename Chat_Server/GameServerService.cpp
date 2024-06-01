#include "stdafx.h"
#include "GameServerService.h"

#include "Base.h"

GameServerService::GameServerService(const WCHAR* ip, int port, SessionFactory factory)
    :ServerService(ip,port,factory)
{
}

void GameServerService::Open()
{
    ServerService::Open();
    _villageMap = new Map({-5,-5}, {5,5});
}

void GameServerService::Close()
{
    ServerService::Close();
    delete _villageMap;
}
