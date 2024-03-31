#include "stdafx.h"
#include "PacketHandler.h"

void PacketHandler::C2S_ENTER_ROOM_Handler(Session* session, Packet& packet)
{
	// 다른사람에게 입장 소식 알리기
}

void PacketHandler::C2S_CHAT_REQ_Handler(Session* session, Packet& packet)
{
	// 다른 사람에게 채팅 정보 보내기
}

void PacketHandler::C2S_EXIT_ROOM_Handler(Session* session, Packet& packet)
{
	// 다른사람에게 퇴장 소식 알리기
}
