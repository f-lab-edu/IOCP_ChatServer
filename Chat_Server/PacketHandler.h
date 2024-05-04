#pragma once
class PacketHandler
{
public:
	static void LATENCY_CHECK_Handler(shared_ptr<Session> session, Packet* packet);
	static void C2S_ENTER_ROOM_Handler(shared_ptr<Session> session, Packet* packet);
	static void C2S_CHAT_REQ_Handler(shared_ptr<Session> session, Packet* packet);
	static void C2S_EXIT_ROOM_Handler(shared_ptr<Session> session, Packet* packet);
};

