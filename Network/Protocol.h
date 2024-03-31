#pragma once


// Protocol ³×ÀÌ¹Ö
// C2S -> S to C
// S2C -> C to S

enum Protocol
{
	C2S_ENTER_ROOM = 1,
	S2C_ENTER_ROOM_NOTIFY = 1,
	
	C2S_CHAT_REQ,
	S2C_CHAT_RES,
	
	C2S_EXIT_ROOM,
	S2C_EXIT_ROOM_NOTIFY,
};