#include "stdafx.h"
#include <string>
#include <vector>
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
			if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_CONNECT_SERVER].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
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
				STMP_DBG_OUTPUT("connectServer: Response error!\nback string: %s\n", szBuf);
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
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_HELO].szCmd, m_strAddr.c_str());
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_HELO].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send helo info successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendHelo: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}
bool CSmtpRole::sendAuthLogin(CSocket &s)
{
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_AUTHLOGIN].szCmd);
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_AUTHLOGIN].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send authlogin request successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendAuthLogin: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}
bool CSmtpRole::sendUserAddr(CSocket &s)
{
	char *tmp = NULL;
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
		tmp = base64Encode(m_strAddr.c_str(), m_strAddr.length());
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_USERADDR].szCmd, tmp);
		delete []tmp;
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_USERADDR].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send user addr successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendUserAddr: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}
bool CSmtpRole::sendUserPassw(CSocket &s)
{
	char *tmp = NULL;
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
		tmp = base64Encode(m_strPassw.c_str(), m_strPassw.length());
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_USERPASSW].szCmd, tmp);
		delete []tmp;
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_USERPASSW].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send user passw successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendUserPassw: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}
bool CSmtpRole::sendMailFrom(CSocket &s)
{
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_MAIL_FROM].szCmd, m_strAddr.c_str());
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_MAIL_FROM].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send mail from info successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendMailFrom: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}
bool CSmtpRole::sendRcptTo(CSocket &s, std::string &strRcpt)
{
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_RCPT_TO].szCmd, strRcpt.c_str());
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_RCPT_TO].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send rcpt to info successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendRcptTo: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}
bool CSmtpRole::sendDataReq(CSocket &s)
{
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_DATA_REQ].szCmd);
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_DATA_REQ].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send data request successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendDataReq: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}

bool CSmtpRole::sendDataContent(CSocket &s, std::string &strContent)
{
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN*3] = {0};
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN*3 - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_DATA_CONTENT].szCmd, strContent.c_str());
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_DATA_CONTENT].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send data content successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendDataContent: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}
bool CSmtpRole::sendQuit(CSocket &s)
{
	if (s.isConnected())
	{
		char szBuf[MAX_SMTP_RESPONSE_LEN] = {0};
		sprintf_s(szBuf, MAX_SMTP_RESPONSE_LEN - 1, SMTP_COMMAND[en_smtp_cmd::SMTP_QUIT].szCmd);
		s.sendData(szBuf, strlen(szBuf));

		memset(szBuf, 0, MAX_SMTP_RESPONSE_LEN);
		int len = s.recvData(szBuf, MAX_SMTP_RESPONSE_LEN-1);
		if (len > 0 && strncmp(szBuf, SMTP_COMMAND[en_smtp_cmd::SMTP_QUIT].szRespCode, MAX_SMTP_CMD_RESPCODE_LEN - 1) == 0)
		{
			STMP_DBG_OUTPUT("send quit request successfully!\nback string: %s\n", szBuf);
			return true;
		}
		else if (len <= 0)
		{
			STMP_DBG_OUTPUT("socket recv error!\nerror code: %d\n", GetLastError());
		}
		else
		{
			STMP_DBG_OUTPUT("sendQuit: Response error!\nback string: %s\n", szBuf);
		}
		s.closeSocket();
	}
	else
	{
		STMP_DBG_OUTPUT("not connect to Server, retry connectServer()\n");
	}
	return false;
}

void CSmtpRole::sendEMail(CSmtpRole &rcpt, std::string &strSubject, std::string &strBody)
{
	CSocket s;
	if(!connectServer(s))
	{
		return;
	}
	if(!sendHelo(s))
	{
		return;
	}
	if(!sendAuthLogin(s))
	{
		return;
	}
	if(!sendUserAddr(s))
	{
		return;
	}
	if(!sendUserPassw(s))
	{
		return;
	}
	if(!sendMailFrom(s))
	{
		return;
	}
	if(!sendRcptTo(s, rcpt.m_strAddr))
	{
		return;
	}
	if(!sendDataReq(s))
	{
		return;
	}
	std::string strContent;
	strContent += "From: " + m_strAddr
		+ "\r\nTo: " + rcpt.m_strAddr
		+ "\r\nSubject: " + strSubject
		+ "\r\nMIME-Version: 1.0\r\n\r\n"
		+ strBody + "\r\n.\r\n";
	if(!sendDataContent(s, strContent))
	{
		return;
	}
	if(!sendQuit(s))
	{
		return;
	}
	s.closeSocket();
}
//void CSmtpRole::sendEMailEx(CSmtpRole &rcpt, std::string &strSubject, std::string &strBody, std::vector<std::string> &vstrFilePath)
void CSmtpRole::sendEMailEx(CSmtpRole &rcpt, std::string &strSubject, std::string &strBody, std::string &strFilePath)
{
	CSocket s;
	char *tmp = NULL;
	std::string strHtml;
	strHtml += "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">\r\n"
		 "<HTML><HEAD>\r\n"
         "<META http-equiv=Content-Type content=\"text/html; charset=gb2312\">\r\n"
         "<META content=\"MSHTML 6.00.2600.0\" name=GENERATOR>\r\n"
         "<STYLE></STYLE>\r\n"
         "</HEAD>\r\n"
         "<BODY bgColor=#ffffff>\r\n"
         "<DIV><FONT size=2><PRE>"
		+ strBody
		+ "</PRE></FONT></DIV></BODY></HTML>";
	if(!connectServer(s))
	{
		return;
	}
	if(!sendHelo(s))
	{
		return;
	}
	if(!sendAuthLogin(s))
	{
		return;
	}
	if(!sendUserAddr(s))
	{
		return;
	}
	if(!sendUserPassw(s))
	{
		return;
	}
	if(!sendMailFrom(s))
	{
		return;
	}
	if(!sendRcptTo(s, rcpt.m_strAddr))
	{
		return;
	}
	if(!sendDataReq(s))
	{
		return;
	}
	std::string strContent;
	strContent += "From: <" + m_strAddr
		+ ">\r\nTo: <" + rcpt.m_strAddr
		+ ">\r\nSubject: " + strSubject
		+ "\r\nMIME-Version: 1.0\r\n"
		//+ "\r\nMIME-Version: 1.0\r\n\r\n"
		//+ strBody + "\r\n.\r\n";
		+ "Content-Type: multipart/mixed;\r\n"
        + "   boundary=\"----=_NextPart_000_000A_01C26646.7D0E7AC0\"\r\n"
		+ "X-Priority: 3\r\n"
		//+ "X-MSMail-Priority: Normal\r\n"
		//+ "X-Mailer: Morncolorsoft E_Mail Sender 1.00.0000.0000\r\n"
        //+ "X-Mime: Morncolorsoft Mailer Inside V1.00.0000.0000\r\n\r\n"
        //+ "This is a multi-part message in MIME format.\r\n\r\n"
        + "------=_NextPart_000_000A_01C26646.7D0E7AC0\r\n"
        + "Content-Type: multipart/alternative;\r\n"
        + "   boundary=\"----=_NextPart_001_000B_01C26646.7D0E7AC0\"\r\n\r\n\r\n"
        + "------=_NextPart_001_000B_01C26646.7D0E7AC0\r\n"
        + "Content-Type: text/plain;\r\n"
        + "   charset=\"gb2312\"\r\n"
        + "Content-Transfer-Encoding: base64\r\n\r\n";
	tmp = base64Encode(strBody.c_str(), strBody.length());
	strContent += tmp;
	delete []tmp;

	strContent += "\r\n\r\n"
		"------=_NextPart_001_000B_01C26646.7D0E7AC0\r\n"
		"Content-Type: text/html;\r\n"
		"   charset=\"gb2312\"\r\n"
        "Content-Transfer-Encoding: base64\r\n\r\n";
	tmp = base64Encode(strHtml.c_str(), strHtml.length());
	strContent += tmp;
	delete []tmp;
	strContent += "\r\n\r\n";
	strContent += "------=_NextPart_001_000B_01C26646.7D0E7AC0\r\n"
		"\r\n------=_NextPart_000_000A_01C26646.7D0E7AC0\r\n"
		"Content-Type: application/octet-stream;\r\n"
		"  name=\"" "Iloveu.txt" "\"\r\n"
		"Content-Transfer-Encoding: base64\r\n"
		"Content-Disposition: attachment;\r\n"
		"  filename=\"" "Iloveu.txt" "\"\r\n\r\n"
		"SSBsb3ZlIHlvdSE="  "\r\n"
		"\r\n------=_NextPart_000_000A_01C26646.7D0E7AC0--\r\n\r\n.\r\n";
	if(!sendDataContent(s, strContent))
	{
		return;
	}
	if(!sendQuit(s))
	{
		return;
	}
	s.closeSocket();
}