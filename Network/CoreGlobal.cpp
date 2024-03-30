#include "stdafx.h"
#include "CoreGlobal.h"

ThreadManager* GThreadManager = nullptr;

CoreGlobal::CoreGlobal()
{
	NetworkUtil::Init();
	GThreadManager = new ThreadManager();
}

CoreGlobal::~CoreGlobal()
{
	delete GThreadManager;
}

