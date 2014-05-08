// SimpleSmtp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CSmtp.h"
#include "CSocket.h"

int _tmain(int argc, _TCHAR* argv[])
{
	/*CSocket sk;
	sk.initWSA();
	if(sk.createConnection("smtp.163.com", 25))
	{
		printf("connect successfully!\n");
	}
	else
	{
		printf("connect failed!\n");
	}
	sk.closeSocket();
	sk.cleanWSA();*/
	CSocket::initWSA();
	CSmtpRole role("cly19891215@163.com", "chen19891215");
	role.checkPasswAuthorizied();
	CSocket::cleanWSA();
	system("pause");
	return 0;
}

