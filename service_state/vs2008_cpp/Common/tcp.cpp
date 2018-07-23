// この警告／エラーを抑制する　error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // for windows
#include <string>
#include <vector>
#include "basetype.h"
#include "tcp.h"



static bool32 _initialized = FALSE32;

/*
  tcp initialize
*/
void tcp_initialize()
{
	WSADATA wsaData;
	if( !_initialized ){
		WSAStartup(MAKEWORD(2,0), &wsaData);
		_initialized = TRUE32;
	}
}

/*
  tcp finalize
*/
void tcp_finalize()
{
	if( _initialized ){
		WSACleanup();
		_initialized = FALSE32;
	}
}

/*
  TCPServer constractor
*/
TCPServer::TCPServer()
{
	m_opened = FALSE32;
	memset(&m_sock,0,sizeof(SOCKET));
}

/*
  TCPServer destractor
*/
TCPServer::~TCPServer()
{
	close();
}

/*
  TCPServer open
*/
void TCPServer::open(std::string ip, int32 port)
{
	if(m_opened){
		return;
	}
	m_ip = ip;
	m_port = port;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));
	m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if( INVALID_SOCKET == m_sock ) {
		//printf("socket : %d\n", WSAGetLastError());
		//例外
	}	
	addr.sin_family = AF_INET;
	addr.sin_port = htons((uint16)m_port);
	addr.sin_addr.S_un.S_addr = INADDR_ANY; // TODO: m_ip 指定できるようにする必要がある
	// addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if( 0 != ::bind(m_sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) ){
		// printf("bind : %d\n", WSAGetLastError());
		//例外
	}
	if( 0 != ::listen(m_sock, 1) ){
		// printf("listen : %d\n", WSAGetLastError());
		//例外
	}
	m_opened = TRUE32;
}

/*
  TCPServer close
*/
void TCPServer::close()
{
	::closesocket(m_sock);
}

/*
  TCPServer accept
*/
TCPSocket& TCPServer::accept()
{
	struct sockaddr_in client;
	memset(&client, 0, sizeof(struct sockaddr_in));
	int32 len = sizeof(client);
	SOCKET sock = ::accept(m_sock, (struct sockaddr *)&client, (int*)&len);
	if(INVALID_SOCKET == sock){
		// printf("accept : %d\n", WSAGetLastError());
		// 例外発生
	}
	TCPSocket* p_tcp_socket = new TCPSocket();
	p_tcp_socket->m_opened = TRUE32;
	p_tcp_socket->m_sock = sock;
	return *p_tcp_socket;
}

////////////////////////////////////////////////////////////////////////////////

/*
  TCPSocket constractor
*/
TCPSocket::TCPSocket()
{
	m_opened = FALSE32;
	memset(&m_sock,0,sizeof(SOCKET));
}

/*
  TCPSocket destractor
*/
TCPSocket::~TCPSocket()
{
	close();
}

/*
  TCPSocket open
*/
void TCPSocket::open(std::string ip, int32 port)
{
	if(m_opened){
		return;
	}
	m_ip = ip;
	m_port = port;
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_port = htons((uint16)m_port);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
	m_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if( 0 != ::connect(m_sock,
					 (struct sockaddr *)&addr,
					 sizeof(struct sockaddr_in)) ){
		// printf("connect : %d\n", WSAGetLastError());
		// 例外
	}
	m_opened = TRUE32;
}

/*
  TCPSocket send
*/
int32 TCPSocket::send(std::vector<int8>& buffer)
{
	return (int32)::send(m_sock, (const char*)&buffer[0], (int)buffer.size(), 0);
}

/*
  TCPSocket receive
*/
int32 TCPSocket::receive(std::vector<int8>& buffer, int32 timeout)
{
	return (int32)::recv(m_sock, (char *)&buffer[0], (int)buffer.size(), timeout);
}

void TCPSocket::close()
{
	closesocket(m_sock);
}
