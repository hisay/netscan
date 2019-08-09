#include "StdAfx.h"

#include <WinSock2.h>
#include "SockIP.h"
#include "Scan.h"
#pragma comment(lib, "ws2_32.lib")

Scan::Scan(void)
{
	m_nPort = 0;
}


Scan::~Scan(void)
{
}

void Scan::InitNet()
{
	WSAData	wsa;
	WSAStartup( MAKEWORD(2,2), &wsa);
}

void Scan::StartScanPort(const char* ip,int port)
{
	m_sip = ip;
	m_nPort = port;
	CloseHandle( CreateThread( NULL, 0, &Scan::PortThread, LPVOID(this), 0, NULL));

}

string GetNewIP(BYTE& b1,BYTE&b2,BYTE &b3,BYTE &b4)
{
	b4++;
	if(b4>=255) { b4 = 1;
	b3++;
	if ( b3>=255){ b3=1;
		b2++;
		if(b2>=255){b2=1;
		 b1++;
		 if(b1>=255){return "";}
		}
	}
	}

	char buf[20];
	sprintf(buf,"%d.%d.%d.%d",b1,b2,b3,b4);
	return string(buf);
}
void Scan::OnConnOK(const char*ip,void* p){
	printf("[OK]%s\n",ip);
	((CSockIP*)p)->SetNewIP( GetNewIP(b1,b2,b3,b4).c_str());
}
void Scan::OnConnFailed(const char*ip,void* p){
	//printf("failed %s\n",ip);
	((CSockIP*)p)->SetNewIP( GetNewIP(b1,b2,b3,b4).c_str());;
}
DWORD WINAPI Scan::PortThread(LPVOID lp)
{
	Scan* ps = (Scan*)lp;

	BYTE&	b1 = ps->b1,&b2=ps->b2,&b3=ps->b3,&b4=ps->b4;
	sscanf(ps->m_sip.c_str(), "%d.%d.%d.%d", &b1,&b2,&b3,&b4);

	int	nCount = 10000;
	CSockIP* p = new CSockIP[nCount];
	for( int i=0; i<nCount;i++){
		p[i].SetIPPort( GetNewIP(b1,b2,b3,b4).c_str(), ps->m_nPort );
		p[i].SetSIP(ps);
	}
	while( true )
	{
		for( int i=0; i<nCount; i++){
			p[i].Update();
			}
		Sleep(1);
	}

	return 0;
}