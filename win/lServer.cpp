#include "pch.h"
#include "lServer.h"

lServer::lServer()
{
	Initialization();
}

lServer::~lServer()
{
	Close();
}

void lServer::Initialization()
{
	lThread::Initialization();
	ltClient.Initialization();
	ltClient.DeleteModeOff();
	Error = 0;
}

void lServer::Close()
{
	int i;
	lThread::Close();
	for (i = 0; i < ltClient.GetSize(); i++) {
		lClient* p = (lClient*)ltClient.Search(i);
		p->Close();
		delete(p);
	}
	ltClient.RemoveAll();
	if (this->EndEvent) {
		closesocket(sock);
		WSACleanup();
	}
}

void lServer::ServerTCPIPOpen(const char* ip, int port, int dt)
{
	struct sockaddr_in sAddr;
	memset(&sAddr,0,sizeof(struct sockaddr_in));
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { return; }
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) { 
		WSACleanup();
		return; 
	}
	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = inet_addr(ip);
	sAddr.sin_port = htons(port);
	if (bind(sock, (const sockaddr*)&sAddr, sizeof(sockaddr)) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
		return;
	}
	if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();
		return;
	}
	FD_ZERO(&sltAccept);
	FD_SET(sock, &sltAccept);
	start(dt);
}

BOOL lServer::run()
{
	if(Error == 0)Loop();
	return lThread::run();
}

void lServer::Loop()
{
	fd_set read,error;
	lClient* client;
	int sockfd = sock;
	struct timeval t;
	FD_ZERO(&read);
	FD_ZERO(&error);
	memmove(&read, &sltAccept, sizeof(fd_set));
	memmove(&error, &sltAccept, sizeof(fd_set));
	t.tv_usec = 10000;
	int result = select(sockfd + 1, &read, NULL, &error, &t);
	if (result == 0) {
		Error = 0;
		return;
	}
	if (result == SOCKET_ERROR) {
		Error = 1;//socketError
		return;
	}
	if (FD_ISSET(sock, &error)) {
		Error = 2;//SELECT Sensing Error
		return;
	}
	if (FD_ISSET(sock, &read)) {
		lClient* client = Accept();
		ltClient.AddTail(client);
	}
}

lClient* lServer::Accept()
{
	struct sockaddr_in cAddr;
	int len;
	lClient* client = new lClient();
	client->Initialization();
	client->sock = accept(sock, (struct sockaddr*)&cAddr, &len);
	client->start(dt);
	return client;
}
