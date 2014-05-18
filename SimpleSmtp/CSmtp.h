#ifndef _CSMTP_H_
#define _CSMTP_H_
#include <string>

#define MAX_SMTP_RESPONSE_LEN			1024
#define MAX_SMTP_SEND_LEN				1024
#define MAX_SMTP_CMD_LEN				30
#define MAX_SMTP_CMD_RESPCODE_LEN		4
#define MAX_SMTP_CMD_NUM				10

#define DEFAULT_SMTP_PORT				25

typedef struct
{
	char szCmd[MAX_SMTP_CMD_LEN];
	char szRespCode[MAX_SMTP_CMD_RESPCODE_LEN];
}smtp_command_respons;
typedef enum
{
	SMTP_CONNECT_SERVER = 0,
	SMTP_HELO,
	SMTP_AUTHLOGIN,
	SMTP_USERADDR,
	SMTP_USERPASSW,
	SMTP_MAIL_FROM,
	SMTP_RCPT_TO,
	SMTP_DATA_REQ,
	SMTP_DATA_CONTENT,
	SMTP_QUIT
}en_smtp_cmd;
const static smtp_command_respons SMTP_COMMAND[MAX_SMTP_CMD_NUM] = {
								{"CONNECT SERVER", "220"},
								{"HELO %s\r\n", "250"},
								{"AUTH LOGIN\r\n", "334"},
								{"%s\r\n", "334"},
								{"%s\r\n", "235"},
								{"MAIL FROM: <%s>\r\n", "250"},
								{"RCPT TO: <%s>\r\n", "250"},
								{"DATA\r\n", "354"},
								{"%s\r\n", "250"},
								{"QUIT\r\n", "221"}
								};



class CSocket;

class CSmtpRole
{
private:
	std::string m_strAddr;
	std::string m_strPassw;
public:
	bool m_isValid;
	bool m_isAuthorizied;
	std::string m_logInfo;

	//default construct function
	CSmtpRole();
	//construct function
	CSmtpRole(std::string strAddr, std::string strPassw);

	//deconstruct function
	~CSmtpRole();
	//check addr is valid & update m_isValid
	void checkAddrValid();
	//check passwd is right & update m_isAuthorizied
	void checkPasswAuthorizied();
	void checkAuthLogin();
	//get m_strAddr
	std::string getAddr();
	void sendEMail(CSmtpRole &rcpt, std::string &strSubject, std::string &strBody);
	void sendEMailEx(CSmtpRole &rcpt, std::string &strSubject, std::string &strBody, std::string &strFilePath);
private:
	bool connectServer(CSocket &s);
	bool sendHelo(CSocket &s);
	bool sendAuthLogin(CSocket &s);
	bool sendUserAddr(CSocket &s);
	bool sendUserPassw(CSocket &s);
	bool sendMailFrom(CSocket &s);
	bool sendRcptTo(CSocket &s, std::string &strRcpt);
	bool sendDataReq(CSocket &s);
	bool sendDataContent(CSocket &s, std::string &strContent);
	bool sendQuit(CSocket &s);
};

class CSmtpManage
{
private:
	CSmtpRole m_sender;
	CSmtpRole m_rcpt;
public:
	CSmtpManage(CSmtpRole &sender, CSmtpRole &rcpt);
	void sendMail(const char *subj, const char *content);
};


#endif