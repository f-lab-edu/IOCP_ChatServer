#include "stdafx.h"
#include "PacketHandler.h"

void PacketHandler::S2C_ENTER_ROOM_NOTIFY_Handler(shared_ptr<Session> session, Packet* packet)
{
	string nick;
	packet->pop(nick);
	cout << nick << endl;
}

void PacketHandler::S2C_CHAT_RES_Handler(shared_ptr<Session> session, Packet* packet)
{
	string chat;
	packet->pop(chat);
	cout << chat << endl;
  }

void PacketHandler::S2C_EXIT_ROOM_NOTIFY_Handler(shared_ptr<Session> session, Packet* packet)
{
}
