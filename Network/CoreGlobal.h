#pragma once
extern class ThreadManager* GThreadManager;
extern class BufferManager* GBufferManager;
class CoreGlobal
{
public:
	CoreGlobal();
	~CoreGlobal();
};

static CoreGlobal GCoreGlobal;


