﻿#pragma once


// Protocol 네이밍
// C2S -> S to C
// S2C -> C to S

enum Protocol
{
	LATENCY_CHECK = 0,
	
	C2S_ENTER_MAP = 1,
	S2C_ENTER_MAP_NOTIFY = 2,
	
	C2S_CHAT_REQ = 3,
	S2C_CHAT_RES = 4,
	
	C2S_EXIT_MAP = 5,
	S2C_EXIT_MAP_NOTIFY = 6,

	C2S_MOVE = 7,
	S2C_MOVE_NOTIFY = 8,
};