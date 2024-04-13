#include "stdafx.h"
#include "CoreGlobal.h"

ThreadManager* GThreadManager = nullptr;
BufferManager* GBufferManager = nullptr;

CoreGlobal::CoreGlobal()
{
	NetworkUtil::Init();
	GThreadManager = new ThreadManager();
	GBufferManager = new BufferManager();
}

CoreGlobal::~CoreGlobal()
{
	delete GThreadManager;
	delete GBufferManager;
}

