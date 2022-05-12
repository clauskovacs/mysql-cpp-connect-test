#ifndef __LOGIN_CREDENTIALS_H_INCLUDED__
#define __LOGIN_CREDENTIALS_H_INCLUDED__

#include <string>

struct loginCredentials
{
	std::string SQLldriver = "{mysql}";
	std::string SQLserver = "xxx.xxx.xxx.xxx";
	std::string SQLdatabase = "";
	std::string SQLuid = "";
	std::string SQLpassword = "";
};

#endif
