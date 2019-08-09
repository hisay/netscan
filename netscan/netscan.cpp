// netscan.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Scan.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Scan s;
	s.InitNet();
	s.StartScanPort( argv[1], atoi(argv[2]));
	getchar();
	return 0;
}

