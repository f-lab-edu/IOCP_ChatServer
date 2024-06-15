#include "stdafx.h"
#include "PacketHandler.h"

void PacketHandler::LATENCY_CHECK_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	shared_ptr<ServerSession> serverSession = static_pointer_cast<ServerSession>(session);

#ifdef LATENCY_RECORD_OPTION
	serverSession->AddLatency(packet->GetPacketId(), clock() - packet->GetSendTick());
#endif
}

void PacketHandler::S2C_ENTER_MAP_NOTIFY_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{

	shared_ptr<ServerSession> serverSession = static_pointer_cast<ServerSession>(session);

	string nickname;
	int playerId = 0;
	packet->pop(nickname);
	packet->pop(playerId);

	if (serverSession->playerInfo.playerId == 0)
		serverSession->playerInfo.playerId = playerId;
	else
		serverSession->_enterPlayerId.push_back(playerId);
	

	string contents;
	contents = "[입장] " + nickname + "님이 입장했습니다."; 
	cout << contents << endl;
}

void PacketHandler::S2C_CHAT_RES_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	string chat;
	packet->pop(chat);
	cout << chat << endl;
}

void PacketHandler::S2C_EXIT_MAP_NOTIFY_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	string str;
	packet->pop(str);
	cout << str << endl;
}

void PacketHandler::S2C_MOVE_NOTIFY_Handler(shared_ptr<Session> session, std::shared_ptr<Packet> packet)
{
	// TODO: Player를 관리하는 곳에서 id에 맞는 친구를 움직이기
	// TODO: 움직인 곳을 cout으로 출력
	// TMP: 일단 출력만
}
