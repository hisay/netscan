#pragma once
#include "ISip.h"
#include <Windows.h>
#include <string>
using namespace std;


class Scan : public ISip
{
public:
	Scan(void);
	~Scan(void);
	void StartScanPort(const char* ip,int port);
	virtual void OnConnOK(const char*ip,void*)  ;
	virtual void OnConnFailed(const char*ip,void*)  ;
	static DWORD WINAPI PortThread(LPVOID lp);
	static	void InitNet();

protected:
	string	m_sip;
	int		m_nPort;
	BYTE	b1,b2,b3,b4;
};

