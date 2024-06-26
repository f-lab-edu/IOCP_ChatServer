#include "stdafx.h"
#include "IocpWorker.h"

void IocpWorker::Init()
{
	Worker::Init();
	_iocpHandle = INVALID_HANDLE_VALUE; 
}

void IocpWorker::Work()
{
	xassert((_iocpHandle == INVALID_HANDLE_VALUE));
	
   while(IsActive())
   {
      	DWORD bytesTransffered = 0; 
      	IoEvent* ioEvent = nullptr;
      	ULONG_PTR key = 0;
      
		if (true == static_cast<bool>(::GetQueuedCompletionStatus(_iocpHandle, &bytesTransffered,&key, reinterpret_cast<LPOVERLAPPED*>(&ioEvent), INFINITE))) {
			auto iocpObject = ioEvent->owner;
			iocpObject->OnExecute(ioEvent, static_cast<int>(bytesTransffered));
		}
		else {
			int errCode = ::WSAGetLastError();
			std::cout << std::system_category().message(errCode);
			ioEvent->owner->OnExecute(ioEvent, 0);
		}
   }
}
