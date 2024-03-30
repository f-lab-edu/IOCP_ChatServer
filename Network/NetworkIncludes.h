#pragma once

#include <iostream>

#include <vector>
#include <map>
#include <unordered_map>

#include <functional>

#include <memory>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <mswsock.h>
#pragma comment(lib, "ws2_32.lib")


#include "RingBuffer.h"
#include "NetworkUtil.h"
#include "ThreadManager.h"
#include "CoreGlobal.h"
#include "Packet.h"

#include "IoEvent.h"
#include "IocpObject.h"
#include "Session.h"
#include "Listener.h"

#include "Protocol.h"

using namespace std;