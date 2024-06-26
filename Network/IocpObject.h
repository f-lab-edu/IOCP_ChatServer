﻿#pragma once

class IocpObject : public std::enable_shared_from_this<IocpObject> {
public:
	IocpObject() = default;
	~IocpObject() = default;
	virtual HANDLE GetHandle() abstract;
public:
	virtual void OnExecute(class IoEvent* iocpEvnet, int SizeOfBytes) abstract;
};