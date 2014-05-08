#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <winsock2.h>

class CSocket
{
private:
	SOCKET m_socket;
	bool m_connected;
public:
	CSocket():m_socket(NULL), m_connected(false)
	{}
	~CSocket()
	{
		closeSocket();
	}
	static void initWSA();
	static void cleanWSA();
	static void getHostIPbyName(char *, char *);
	bool createConnection(char *ip, int nPort);
	int recvData(char *buf, int len);
	int sendData(char *buf, int len);
	void closeSocket();
	bool isConnected();

};



#endif