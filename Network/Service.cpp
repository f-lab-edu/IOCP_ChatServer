#include "stdafx.h"
#include "Service.h"


Service::Service(const WCHAR* ip, int port, SessionFactory factory)
{
    _iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
    xassert((_iocpHandle == INVALID_HANDLE_VALUE));

    memcpy(_ip, ip, sizeof(_ip));
    _port = port;

    _sessionFactory = factory;
    
    _iocpWorker.Init();
    _iocpWorker.SetHandle(_iocpHandle);
}

Service::~Service()
{
}

bool Service::IocpRegister(std::shared_ptr<IocpObject> iocpObject)
{
    return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, 0, 0);
}

void Service::Open()
{
    xassert(_isOpen == true);
    
    _isOpen = true;
    _iocpWorker.StartWork(2);
}

void Service::Close()
{
    
}

std::shared_ptr<Session> Service::CreateSession()
{
    xassert((_sessionFactory == nullptr));

    shared_ptr<Session> session = _sessionFactory();
    session->SetService(this);
    
	ULONG_PTR key = 0;
	CreateIoCompletionPort((HANDLE)session->GetSocket(), _iocpHandle, key, 0);

    return std::move(session);
}

void Service::AddSession(std::shared_ptr<Session> session)
{
    lock_guard<mutex> lock(_sessionContainerLock);
    _sessions.insert(session);
}

void Service::DeleteSession(std::shared_ptr<Session>&& session)
{
    lock_guard<mutex> lock(_sessionContainerLock);
    _sessions.erase(session);
}

