#pragma once
#include "isip.h"
#include <string>
#include <WinSock2.h>
using namespace std;

class CSockIP 
{
public:
	CSockIP(void);
	~CSockIP(void);
	void SetIPPort(const char* ip, int port);
	void SetNewIP(const char* ) ;
	void Update();
	void Close();
	void Connect();
	void SetSIP(ISip* p){ m_pIP = p;}
protected:
	void OnFailed();
	void OnOK();
	ISip*	m_pIP;
	string m_ip;
	int	m_port;
	bool m_bConn;
	SOCKET m_skt;
	time_t	m_nTick;
};

