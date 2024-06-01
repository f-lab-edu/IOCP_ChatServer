#pragma once


// Protocol 네이밍
// C2S -> S to C
// S2C -> C to S

enum Protocol
{
	LATENCY_CHECK = 0,	
	C2S_ENTER_MAP = 1,
	S2C_ENTER_MAP_NOTIFY,
	
	C2S_CHAT_REQ,
	S2C_CHAT_RES,
	
	C2S_EXIT_MAP,
	S2C_EXIT_MAP_NOTIFY,
};