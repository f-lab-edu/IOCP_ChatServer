#pragma once
#include "Service.h"

class ClientService : public Service
{
public:
    ClientService(const WCHAR* ip, int port, SessionFactory factory, int dummyCount);
    virtual ~ClientService() override;
public: 
    void Open() override;
    void Close() override;

private:
    int _dummyClientCount;
    std::vector<shared_ptr<Session>> _dummyClients;
};

