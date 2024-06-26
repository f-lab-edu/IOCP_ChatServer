#pragma once

class Worker
{
public:
    Worker() = default;
    virtual ~Worker();

    virtual void Init();
    
    void StartWork(int threadCount);
    void StopWork();
   
    virtual void Work() abstract;

public:
    bool IsActive() { return _isActive; };

private:
    int _threadCount;
    bool _isActive;
};
