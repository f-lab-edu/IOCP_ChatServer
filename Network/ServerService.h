#pragma once
#include "Service.h"


class ServerService : public Service
{
public:
    ServerService(const WCHAR* ip, int port, SessionFactory factory);
    ~ServerService() override = default;
public:
    
    void Open() override;
    void Close() override;

private:
    std::shared_ptr<class Listener> _listener;
};
