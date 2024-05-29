#pragma once
#include "Worker.h"

class IocpWorker : public Worker
{
public:
    IocpWorker() = default;
    ~IocpWorker() override = default;
    
public:
    void Init() override;

public:
    void SetHandle(HANDLE handle) { _iocpHandle = handle; }
    void Work() override;

private:
    HANDLE _iocpHandle;
};
