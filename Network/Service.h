#pragma once
#include <functional>
#include <memory>
#include <Esent.h>
#include <concrt.h>
#include "IocpWorker.h"
#include "LogicWorker.h"
#include <mutex>
#include <set>

using SessionFactory = std::function<std::shared_ptr<Session>(void)>;

class Session;
class Service
{
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

    void PushJob(std::shared_ptr<Packet> p) { _logicWorker.PushJob(move(p)); }
public:
    const WCHAR* GetIp() { return _ip; }
    int GetPort() { return _port; }

    HANDLE GetIocpHandle() { return _iocpHandle; }
private:
    bool _isOpen;
private:
    HANDLE _iocpHandle;
    
	WCHAR _ip[22];
	int _port = 0;
    
    IocpWorker _iocpWorker;
    LogicWorker _logicWorker;

    std::mutex _sessionContainerLock;
    std::set<std::shared_ptr<Session>> _sessions;
    SessionFactory _sessionFactory;
};
