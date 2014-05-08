#ifndef _CSMTP_H_
#define _CSMTP_H_
#include <string>

#define MAX_SMTP_RESPONSE_LEN			1024
#define MAX_SMTP_SEND_LEN				1024


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
	//get m_strAddr
	std::string getAddr();

};


#endif