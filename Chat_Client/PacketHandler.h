#pragma once
class PacketHandler
{
public:
	static void S2C_ENTER_ROOM_NOTIFY_Handler(shared_ptr<Session> session, Packet* packet);
	static void S2C_CHAT_RES_Handler(shared_ptr<Session> session, Packet* packet);
	static void S2C_EXIT_ROOM_NOTIFY_Handler(shared_ptr<Session> session, Packet* packet);
};
