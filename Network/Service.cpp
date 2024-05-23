#include "Service.h"
#include "stdafx.h"

Service::Service()
{
    _iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,0,0,0);
    xassert((_iocpHandle == INVALID_HANDLE_VALUE));
}

bool Service::IocpRegister(std::shared_ptr<IocpObject> iocpObject)
{
    return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, 0, 0);
}
