#pragma once
#include <concrt.h>
#include <ws2ipdef.h>

class Service
{
public:
    Service(char* ip, int port);
    virtual ~Service();

public:
    bool IocpRegister(std::shared_ptr<class IocpObject> iocpObject);
private:
    HANDLE _iocpHandle;
    
	char _ip[INET_ADDRSTRLEN];
	int _port = 0;
};
