#pragma once
#include "Worker.h"

class IocpWorker : Worker
{
public:
    IocpWorker(HANDLE handle);
    ~IocpWorker() override;
    
public:
    void Work() override;

    HANDLE _iocpHandle;
};
