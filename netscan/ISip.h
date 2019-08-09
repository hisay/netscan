#pragma once


class ISip
{
public:
	virtual void OnConnOK(const char*ip,void*) = 0;
	virtual void OnConnFailed(const char*ip,void*) = 0;
	
};