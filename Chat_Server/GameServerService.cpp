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
    MapInfo info {"마을", {0, 0}, {-5, -5}, {5, 5}};
    _villageMap = new Map(info);
}

void GameServerService::Close()
{
    ServerService::Close();
    delete _villageMap;
}
