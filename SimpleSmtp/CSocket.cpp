#include "stdafx.h"
#include "CSocket.h"
#include <windows.h>

#pragma comment(lib, "WS2_32.lib")

void CSocket::initWSA()
{
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);
}
void CSocket::cleanWSA()
{
	WSACleanup();
}
void CSocket::getHostIPbyName(char *pHostName, char *pHostIP)
{
	if(pHostName == NULL || pHostIP == NULL)
	{
		return;
	}
	hostent *host = gethostbyname(pHostName);
	if(host == NULL)
	{
		return;
	}
	sprintf(pHostIP, "%d.%d.%d.%d\0", *(unsigned char*)(host->h_addr_list[0])
									, *(unsigned char*)(host->h_addr_list[0]+1)
									, *(unsigned char*)(host->h_addr_list[0]+2)
									, *(unsigned char*)(host->h_addr_list[0]+3));
}
bool CSocket::createConnection(char *pHostName, int nPort)
{
	if(pHostName == NULL || nPort <= 0)
	{
		return false;
	}
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrServ;
	char szIP[25] = {0};
	CSocket::getHostIPbyName(pHostName, szIP);
	int len = strlen(szIP);
    if (len > 0 && len < 25)
	{
		addrServ.sin_family = AF_INET;
		addrServ.sin_addr.S_un.S_addr = inet_addr(szIP);
		addrServ.sin_port = htons(nPort);
		int ret = connect(m_socket, (sockaddr*)&addrServ, sizeof(SOCKADDR));
		if(ret != 0)
		{
			ret = GetLastError();
			STMP_DBG_OUTPUT("connect failed!\nerror code: %d\n", ret);
			printf("%d \n", ret);
			return false;
		}
		STMP_DBG_OUTPUT("open socket connect\n");
		m_connected = true;
		return true;
	}
	else
	{
		STMP_DBG_OUTPUT("can't retrieve IP by HostName\n");
		return false;
	}
}
void CSocket::closeSocket()
{
	closesocket(m_socket);
	m_socket = NULL; 
	STMP_DBG_OUTPUT("close socket connect\n");
	//WSACleanup();
}
bool CSocket::isConnected()
{
	return m_connected;
}

int CSocket::recvData(char *buf, int len)
{
	return recv(m_socket, buf, len, 0);
}

int CSocket::sendData(char *buf, int len)
{
	return send(m_socket, buf, len, 0);
}