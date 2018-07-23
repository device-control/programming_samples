#ifndef _TCP_H_
#define _TCP_H_

#include <winsock2.h> // includes requred for first line. for windows.
#include <string>
#include <vector>
#include "basetype.h"
#include "uncopyable.h"

class TCPSocket;

class TCPServer: private Uncopyable
{
private:
	//members
	std::string m_ip; // ip address
	int32 m_port; // port number
	bool32 m_opened;
	// for windows
	SOCKET m_sock;
	// struct sockaddr_in m_addr;
	// struct sockaddr_in m_client;
public:
	TCPServer();
	~TCPServer();
	// tcp listener でOS別に必要な情報を纏める
	void open(std::string ip, int32 port); // リッスン開始
	TCPSocket& accept(); // 接続待ち
	void close(); // リッスン破棄

};
	
class TCPSocket: private Uncopyable
{
private:
	//members
	std::string m_ip; // ip address
	int32 m_port; // port number
	bool32 m_opened;
	// for windows
	SOCKET m_sock;
	// struct sockaddr_in m_addr;
	// struct sockaddr_in m_client;
public:
	TCPSocket();
	~TCPSocket();
	// TCPSocket(SOCKET); としたいけど SOCKET i/f が公開されるのはよくない。依存増加
	friend TCPServer;
	// tcp 接続情報
	void open(std::string ip, int32 port);
	int32 send(std::vector<int8>& buffer);
	int32 receive(std::vector<int8>& buffer, int32 timeout);
	void close();
};

void tcp_initialize();
void tcp_finalize();
#endif // _TCP_H_
