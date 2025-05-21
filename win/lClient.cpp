#include "pch.h"
#include "win/lClient.h"

lClient::lClient()
{
	Initialization();
}

lClient::~lClient()
{
	Close();
}

void lClient::Initialization()
{
	lThread::Initialization();
	Error = 0;
}

void lClient::Close()
{
	lThread::Close();
}

void lClient::Connect(const char* IP, int port,int dt){
	WSADATA wsaData;
	struct sockaddr_in cAddr;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return;
	}
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		WSACleanup();
		return;
	}
	cAddr.sin_family = AF_INET;
	cAddr.sin_addr.s_addr = inet_addr(IP);
	cAddr.sin_port = htons(port);
	if (connect(sock, (struct sockaddr*)&cAddr, sizeof(cAddr)) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
		return;
	}
	start(dt);
}

void lClient::Recv(char* buf,unsigned int size)
{
	fd_set read, error;
	int sockfd = sock;
	int result;
	struct timeval t;
	if (Error != 0)return;
	t.tv_usec = 10000;
	FD_ZERO(&read);
	FD_ZERO(&error);
	FD_SET(sock, &read);
	FD_SET(sock, &error);
	result = select(sockfd + 1, &read, NULL, &error, &t);
	if (result == 0) {
		Error = 0;
		return;
	}
	if (result < 0) {
		Error = 1;
		return;
	}
	if (FD_ISSET(sock, &error)) {
		Error = 2;
		return;
	}
	if (FD_ISSET(sock, &read)) { 
		result = recv(sock, buf, size, 0); 
		if (result <= 0) {
			Error = 3;
			return;
		}
	}
}

void lClient::Send(char*buf , unsigned int size)
{
	fd_set write, error;
	int sockfd = sock;
	int result;
	struct timeval t;
	if (Error != 0)return;
	t.tv_usec = 10000;
	FD_ZERO(&write);
	FD_SET(sock, &write);
	FD_SET(sock, &error);
	result = select(sockfd + 1, NULL, &write, &error, &t);
	if (result == 0) {
		Error = 0;
		return;
	}
	if (result < 0) {
		Error = 1;
		return;
	}
	if (FD_ISSET(sock, &error)) {
		Error = 2;
		return;
	}
	if (FD_ISSET(sock, &write)) { 
		result = send(sock, buf, size, 0); 
		if (result <= 0) {
			Error = 3;
			return;
		}
	}
}

BOOL lClient::run(){
	return lThread::run();
}
