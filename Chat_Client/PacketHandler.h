#pragma once
class PacketHandler
{
public:
	static void LATENCY_CHECK_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet);
	static void S2C_ENTER_MAP_NOTIFY_Handler(shared_ptr<Session> session, std::shared_ptr<Packet>  packet);
	static void S2C_CHAT_RES_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet);
	static void S2C_EXIT_MAP_NOTIFY_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet);
};

