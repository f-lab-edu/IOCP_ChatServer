#pragma once
class PacketHandler
{
public:
	static void LATENCY_CHECK_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet);
	static void C2S_ENTER_MAP_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet);
	static void C2S_CHAT_REQ_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet);
	static void C2S_EXIT_MAP_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet);
	static void C2S_ATTACK_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet);
};

