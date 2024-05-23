#include "Worker.h"
#include "stdafx.h"
#include "ThreadManager.h"

Worker::Worker()
    : _isActive(false), _threadCount(0)
{
    
}

Worker::~Worker()
{
}

void Worker::StartWork(int threadCount)
{
    xassert(threadCount <= 0);
    xassert(_isActive == true);
    
    _threadCount = threadCount; 
    _isActive = true;
    
    for(int i = 0; i < threadCount; i++)
    {
        GThreadManager->ThreadStart([]()
        {
            Worker::Work();
        });
    }
}

void Worker::StopWork()
{
    _isActive = false;
}
