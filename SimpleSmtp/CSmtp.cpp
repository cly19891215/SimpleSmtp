#include "stdafx.h"
#include <string>

#include "CSmtp.h"
#include "CSocket.h"
#include "Util.h"
#include <time.h>

//default construct function
CSmtpRole::CSmtpRole():m_strAddr(""), m_strPassw(""), m_logInfo(""), m_isValid(false), m_isAuthorizied(false)
{
#ifdef _CSMTP_DEBUG_
	printf("default construct successfully!\n");
#endif
}

	//construct function
CSmtpRole::CSmtpRole(std::string strAddr, std::string strPassw):m_strAddr(strAddr), m_strPassw(strPassw), m_logInfo(""), m_isValid(false), m_isAuthorizied(false)
{
#ifdef _CSMTP_DEBUG_
	
#endif
}

//deconstruct function
CSmtpRole::~CSmtpRole()
{
#ifdef _CSMTP_DEBUG_
	printf("deconstruct successfully!\nm_strAddr=%s\nm_strPassw=%s\n", m_strAddr.c_str(), strPassw.c_str());
#endif
}

//get m_strAddr
std::string CSmtpRole::getAddr()
{
	return m_strAddr;
}
//check addr is valid & update m_isValid
void CSmtpRole::checkAddrValid()
{
	CSocket s;
	connectServer(s);
}

//check passwd is right & update m_isAuthorizied
void CSmtpRole::checkPasswAuthorizied()
{
	CSocket skHost;
	char szHostName[100] = {0};
	char szTmp[MAX_SMTP_RESPONSE_LEN] = {0};
	char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
	int len = 0;
	char *tmp = NULL;
	if (m_strAddr != "")
	{
		int count = m_strAddr.find_first_of("@");
		sprintf_s(szHostName, 100, "smtp.%s", (m_strAddr.c_str() + count + 1));
		if(skHost.createConnection(szHostName, 25))
		{
			//m_isAuthorizied = true;
			len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
			if (strncmp(szBuf, "220", 3) == 0)
			{
				printf("response right!\n%s\n", szBuf);

				memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
				sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "helo %s\r\n", m_strAddr.c_str());
				skHost.sendData(szBuf, strlen(szBuf));

				memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
				len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
				if (strncmp(szBuf, "250", 3) == 0)
				{
					printf("user valid!\n%s\n", szBuf);
					m_isValid = true;

					memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
					sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "AUTH LOGIN\r\n");
					skHost.sendData(szBuf, strlen(szBuf));

					memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
					len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);

					if (strncmp(szBuf, "334", 3) == 0)
					{
						printf("please login!\r\n%s\n", szBuf);
						
						tmp = base64Encode(m_strAddr.c_str(), m_strAddr.length());
						memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
						sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "%s\r\n", tmp);
						delete []tmp;
						skHost.sendData(szBuf, strlen(szBuf));

						memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
						len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
						if(strncmp(szBuf, "334", 3) == 0)
						{
							printf("User Addr right, send Passw next!\n%s\n", szBuf);

							tmp = base64Encode(m_strPassw.c_str(), m_strPassw.length());
							memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
							sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "%s\r\n", tmp);
							delete []tmp;
							skHost.sendData(szBuf, strlen(szBuf));

							memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
							len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
							if (strncmp(szBuf, "235", 3) == 0)
							{
								printf("Authorizied Login Success!\n%s\n", szBuf);
								
								memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
								sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "MAIL FROM: <%s>\r\n", m_strAddr.c_str());
								skHost.sendData(szBuf, strlen(szBuf));

								memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
								len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
								if (strncmp(szBuf, "250", 3) == 0)
								{
									printf("mail from right!\n%s\n", szBuf);

									memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
									sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "RCPT TO: <%s>\r\n", "live_dream@yeah.net");
									skHost.sendData(szBuf, strlen(szBuf));

									memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
									len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);

									if(strncmp(szBuf, "250", 3) == 0)
									{
										printf("mail to right!\n%s\n", szBuf);

										memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
										sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "DATA\r\n");
										skHost.sendData(szBuf, strlen(szBuf));

										memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
										len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
										


										memset(szTmp, 0, MAX_SMTP_RESPONSE_LEN);
										time_t seconds;//调用本地时间
										time(&seconds);
										strftime(szTmp, MAX_SMTP_RESPONSE_LEN, "%a, %d %b %y %H:%M:%S +0800", localtime(&seconds));
										
										/*sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "From: %s\r\nTo: %s\r\nDate: %s\r\nSubject: %s\r\n\r\nX-Mailer: %s\r\nX-Priority: %s\r\nMIME-Version: 1.0\r\nConten-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n", 
											m_strAddr.c_str(),
											"live_dream@yeah.net",
											szTmp,
											"Crazy Effort",
											"SMailer",
											"3",
											"#BOUNDARY#");*/
										memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
										sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "From: %s\r\nTo: %s\r\nSubject: %s\r\nMIME-Version: 1.0\r\n\r\n%s\r\n.\r\n",
										m_strAddr.c_str(),
										"live_dream@yeah.net",
										"Crazy Effort",
										"try your best"
										);
										skHost.sendData(szBuf, strlen(szBuf));

										memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
										len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);


										/*memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
										sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "--%s\r\nContent-Type: %s\r\nContent-Disposition: %s\r\nContent-Transfer-Encoding: %s\r\n\r\n%s\r\n\r\n",
											"#BOUNDARY#",
											"text/plain; charset=gb2312",
											"",
											"8bit",
											"try your best");
										skHost.sendData(szBuf, strlen(szBuf));

										memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
										len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);*/

										memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
										sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, "QUIT\r\n");
										skHost.sendData(szBuf, strlen(szBuf));

										memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
										len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
									}
									else
									{
										printf("mail to wrong!\n");
									}
								}
								else
								{
									printf("mail from wrong!\n");
								}

							}
							else
							{
								printf("Authorizied Login failure!\n");
							}
						}
						else
						{
							printf("User Addr Error!\n");
						}

					}
					else
					{
						printf("auth login failed\r\n%s\n", szBuf);
					}
				}
				else
				{
					printf("user invalid!\n");
				}
			}
			else
			{
				printf("response wrong!\n");
			}
		}
		else
		{
			printf("connect error!\n");
		}
		skHost.closeSocket();
	}

}

bool CSmtpRole::connectServer(CSocket &s)
{
	char szHostName[100] = {0};
	char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
	if (m_strAddr != "")
	{
		//format server addr from strAddr. replace xx@ with smtp.
		int count = m_strAddr.find_first_of("@");
		sprintf_s(szHostName, 100, "smtp.%s", (m_strAddr.c_str() + count + 1));
		if(s.createConnection(szHostName, DEFAULT_SMTP_PORT))
		{
			int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
			if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_CONNECT_SERVER].szRespCode, 3) == 0)
			{
				STMP_DBG_OUTPUT("connect server success!\nback string: %s\n", szBuf);
				return true;
			}
			else if (len <= 0)
			{
				STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
			}
			else
			{
				STMP_DBG_OUTPUT("connect server error!\nback string: %s\n", szBuf);
			}
			
		}
		else
		{
			STMP_DBG_OUTPUT("socket connect failed!\n");
		}
	}
	else
	{
		STMP_DBG_OUTPUT("m_strAddr is empty!");
	}
	s.closeSocket();
	return false;
}
bool CSmtpRole::sendHelo(CSocket &s)
{
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN, SMTP_COMMAND[], m_strAddr.c_str());
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = skHost.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (strncmp(szBuf, "250", 3) == 0)
		{
		}
	}
	return false;
}
bool CSmtpRole::sendAuthLogin(CSocket &s)
{
	return false;
}
bool CSmtpRole::sendUserAddr(CSocket &s)
{
	return false;
}
bool CSmtpRole::sendUserPassw(CSocket &s)
{
	return false;
}
bool CSmtpRole::sendMailFrom(CSocket &s)
{
	return false;
}
bool CSmtpRole::sendRcptTo(CSocket &s)
{
	return false;
}
bool CSmtpRole::sendData(CSocket &s)
{
	return false;
}