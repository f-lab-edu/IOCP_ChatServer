#pragma once

enum class EventType
{
	Connect,
	Disconnect,
	Accept,
	Recv,
	Send,
};

class IoEvent : public OVERLAPPED {

public:
	IoEvent(EventType type) { m_eventType = type; }

	void Init() {
		OVERLAPPED::hEvent = 0;
		OVERLAPPED::Internal = 0;
		OVERLAPPED::InternalHigh = 0;
		OVERLAPPED::Offset = 0;
		OVERLAPPED::OffsetHigh = 0;
	}
public:
	EventType m_eventType;
	std::shared_ptr<class IocpObject> owner;
};


class AcceptEvent : public IoEvent {
public:
	AcceptEvent() : IoEvent(EventType::Accept) { }

public:
	std::shared_ptr<class Session> session;
};

class SendEvent : public IoEvent
{
public:
	SendEvent() : IoEvent(EventType::Send) {}

public:
	std::vector<WSABUF> buffers;
};