#pragma once

class Map;

class GameServerService : public ServerService
{
public:
    GameServerService(const WCHAR* ip, int port, SessionFactory factory);
    ~GameServerService() override = default;
public:
    void Open() override;
    void Close() override;
private:

    Map* _villageMap;
};