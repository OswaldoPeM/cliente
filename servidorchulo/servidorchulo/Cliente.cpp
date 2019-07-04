#include "Cliente.h"



std::string Cliente::getIpAddress()
{
	std::string ip;
	std::cout << "\nA que Ip desea conectar? :";
	std::cin >> ip;
	std::cout << std::endl;
	return ip;
}

std::string Cliente::getNick()
{
	std::string nick;
	std::cout << "\nQue nickname desea llevar? :";
	std::cin >> nick;
	std::cout << std::endl;
	return nick ;
}

Cliente::Cliente()
{
}


Cliente::~Cliente()
{
}

void Cliente::setIpAddress()
{
	m_ipAddress = getIpAddress();
	m_port = 54000;
	m_nick = getNick();
}

bool Cliente::Initialize()
{
	m_ver= MAKEWORD(2, 2);
	m_wsResult = WSAStartup(m_ver, &m_data);
	if(m_wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << m_wsResult << std::endl;
		return false;
	}
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		return false;
	}
	// Fill in a hint structure
	
	m_hint.sin_family = AF_INET;
	m_hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress.c_str(), &m_hint.sin_addr);
	// Connect to server
	m_connResult = connect(m_sock, (sockaddr*)&m_hint, sizeof(m_hint));
	if (m_connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(m_sock);
		WSACleanup();
		return false;
	}
	return true;
}

void Cliente::sendRecv()
{
	// Prompt the user for some text
	std::cout << "> ";
	std::getline(std::cin, m_userInput);
	toJson(m_userInput);
	if (m_userInput.size() > 0)		// Make sure the user has typed in something
	{
		// Send the text
		int sendResult = send(m_sock,  m_userInput.c_str(), m_userInput.size() + 1, 0);
		if (sendResult != SOCKET_ERROR)
		{
			// Wait for response
			ZeroMemory(m_buf, 4096);
			int bytesReceived = recv(m_sock, m_buf, 4096, 0);
			if (bytesReceived > 0)
			{
				// Echo response to console
				std::cout << "SERVER> " << std::string(m_buf, 0, bytesReceived) << std::endl;
			}
		}
	}
}

std::string Cliente::toJson(std::string msg)
{
	std::string J1 = "{\"from\":\"" + m_nick + "\",\"msg\":\"" + msg + "\",\"to\":\"" + m_to + "\"}";
	return J1;
}
