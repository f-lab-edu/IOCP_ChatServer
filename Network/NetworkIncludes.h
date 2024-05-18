#pragma once

#include <iostream>

#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <concurrent_queue.h>
#include <functional>
#include <atomic>
#include <time.h>
#include <fstream>
#include <filesystem>

#include <memory>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <mswsock.h>
#pragma comment(lib, "ws2_32.lib")

#include "Macro.h"
#include "Buffer.h"
#include "NetworkUtil.h"
#include "ThreadManager.h"
#include "BufferManager.h"
#include "CoreGlobal.h"
#include "Packet.h"

#include "IoEvent.h"
#include "IocpObject.h"
#include "Session.h"
#include "Listener.h"

#include "Protocol.h"

#include "Macro.h"
#include "Singleton.h"

using namespace std;
using namespace Concurrency;

#pragma warning(disable: 4819) // encording warning disable
