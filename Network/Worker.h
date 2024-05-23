#pragma once

class Worker
{
public:
    Worker();
    virtual ~Worker();

    void StartWork(int threadCount);
    void StopWork();
   
    virtual void Work() abstract;

public:
    bool IsActive() { return _isActive; };

private:
    int _threadCount;
    bool _isActive;
};
