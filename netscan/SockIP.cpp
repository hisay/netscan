#include "StdAfx.h"
#include "SockIP.h"
#include <time.h>

CSockIP::CSockIP(void)
{
	m_skt = INVALID_SOCKET;
	m_pIP = NULL;
	m_bConn = false;
	m_port = 0;
}


CSockIP::~CSockIP(void)
{
	Close();
}

void CSockIP::SetIPPort(const char* ip, int port)
{
	m_ip = ip;
	m_port = port;
}

void CSockIP::SetNewIP(const char* ip){
	
	m_ip = ip;
	m_bConn = false;
}

void CSockIP::Close()
{
	if ( m_skt != INVALID_SOCKET )
		closesocket(m_skt);
	m_skt = INVALID_SOCKET;
	m_bConn = false;
}
void CSockIP::OnOK()
{
	if ( m_pIP )m_pIP->OnConnOK( m_ip.c_str(), this);
}
void CSockIP::OnFailed()
{
	if ( m_pIP )m_pIP->OnConnFailed( m_ip.c_str(), this);
}
enum	E_SelSock
{
	ES_READ = 0,
	ES_WRITE = 1,
	ES_READWRITE,
	ES_TIMEOUT,
	ES_ERROR
};

E_SelSock SelectSocket(SOCKET nSock)
{
	timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100;
	fd_set	r,w,e;
	FD_ZERO(&r);
	FD_ZERO(&w);
	FD_ZERO(&e);
	FD_SET(nSock,&r);
	FD_SET(nSock,&w);
	FD_SET(nSock,&e);
	int nErr = select( nSock, &r,&w,&e,&tv);
	if( SOCKET_ERROR == nErr )
	{
		if(WSAEWOULDBLOCK == WSAGetLastError())
			return ES_TIMEOUT;
	}
	if ( 0 == nErr )
	{
		return ES_TIMEOUT;
	}else{
		bool br = FD_ISSET(nSock,&r) ? true : false;
		bool bw = FD_ISSET(nSock,&w) ? true : false;
		bool be = FD_ISSET(nSock,&e) ? true : false;
		if ( br && bw ) return ES_READWRITE;
		if ( br ) return ES_READ;
		if ( bw ) return ES_WRITE;
		if ( be ) return ES_ERROR;
	}
	return ES_ERROR;
}

void CSockIP::Update()
{
	if ( !m_bConn )
		Connect();
	else{
		E_SelSock ss = SelectSocket( m_skt );
		if ( ss == ES_TIMEOUT )
		{
			if ( time(NULL) - m_nTick > 5 )
			{
				m_bConn = false;
				OnFailed();
			}
		}else if(ss == ES_ERROR){
			OnFailed();
		}else{
			OnOK();
		}
	}
}

void CSockIP::Connect()
{
	Close();
	m_skt = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if( INVALID_SOCKET == m_skt )
	{
		OnFailed();
		return;
	}

	DWORD ul = 1;
	if(SOCKET_ERROR == ioctlsocket(m_skt,FIONBIO,&ul))
	{
		printf("FIONBIO set failed%d\n",WSAGetLastError());
		OnFailed();
		return;
	}

	SOCKADDR_IN	addr;
	addr.sin_addr.S_un.S_addr = inet_addr(m_ip.c_str());
	addr.sin_family = AF_INET;
	addr.sin_port = htons(m_port);

	if ( SOCKET_ERROR == connect( m_skt, (sockaddr*)&addr,sizeof(addr)))
	{
		if ( WSAGetLastError() == WSAEWOULDBLOCK )
		{
			m_bConn = true;
			m_nTick = time(NULL);
		}
		else
			OnFailed();
	}else{
		m_bConn= true;
		OnOK();
	}
}