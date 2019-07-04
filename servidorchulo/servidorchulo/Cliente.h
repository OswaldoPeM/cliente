#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

class Cliente
{
	
	int m_wsResult, m_connResult,m_port;
	char m_buf[4096];
	std::string m_ipAddress,m_nick,m_to="all", m_userInput;
	WSAData m_data;
	WORD m_ver;
	SOCKET m_sock;
	sockaddr_in m_hint;

	std::string getIpAddress();
	std::string getNick();
	std::string toJson(std::string msg);
public:
	Cliente();
	~Cliente();

	void setIpAddress();
	bool Initialize();
	void sendRecv();
};

