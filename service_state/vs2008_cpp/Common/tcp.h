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
	// tcp listener ��OS�ʂɕK�v�ȏ���Z�߂�
	void open(std::string& ip, int32 port); // ���b�X���J�n
	TCPSocket& accept(); // �ڑ��҂�
	void close(); // ���b�X���j��

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
	// TCPSocket(SOCKET); �Ƃ��������� SOCKET i/f �����J�����̂͂悭�Ȃ��B�ˑ�����
	friend TCPServer;
	// tcp �ڑ����
	void open(std::string& ip, int32 port);
	int32 send(std::vector<int8>& buffer);
	int32 receive(std::vector<int8>& buffer, int32 timeout);
	void close();
};

void tcp_initialize();
void tcp_finalize();
#endif // _TCP_H_
