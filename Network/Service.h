#pragma once
#include <concrt.h>
#include <functional>
#include <map>
#include <mutex>
#include <set>

#include "IocpWorker.h"
#include "Session.h"

class Service
{
    using SessionFactory = std::function<std::shared_ptr<Session>(void)>;
public:
    Service(const WCHAR* ip, int port, SessionFactory factory);
    virtual ~Service();

public:
    bool IocpRegister(std::shared_ptr<class IocpObject> iocpObject);

    virtual void Open();
    virtual void Close();

    std::shared_ptr<Session> CreateSession();
    void AddSession(std::shared_ptr<Session> session);
    void DeleteSession(std::shared_ptr<Session>&& session);

public:
    const WCHAR* GetIp() { return _ip; }
    int GetPort() { return _port; }

    HANDLE GetIocpHandle() { return _iocpHandle; }
private:
    bool _isOpen;
private:
    HANDLE _iocpHandle;
    
	WCHAR _ip[INET_ADDRSTRLEN];
	int _port = 0;
    
    IocpWorker _iocpWorker;

    std::mutex _sessionContainerLock;
    std::set<std::shared_ptr<Session>> _sessions;
    SessionFactory _sessionFactory;
};
