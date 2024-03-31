#pragma once
class PacketHandler
{
public:
	static void C2S_ENTER_ROOM_Handler(Session* session, Packet& packet);
	static void C2S_CHAT_REQ_Handler(Session* session, Packet& packet);
	static void C2S_EXIT_ROOM_Handler(Session* session, Packet& packet);
};

