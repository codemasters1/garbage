#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <winsock2.h>
#include <stdint.h>

#define VERSION 		0x1010

#define PT_PRESENCE 	0x01
#define PT_REQUEST		0x10
#define PT_RCV_ACK		0x11
#define PT_RESPONSE		0x12

typedef struct {
	uint16_t version	:16;
	uint8_t type		: 8;
	uint16_t command	:16; // for requests
	uint16_t rid		:16; // for requests, ACKs, responses, ...
	uint16_t length		:16;
	uint8_t data[0];
} Packet;

static SOCKET mainSock;
static HANDLE clientThread;

void initPacket(Packet *packet, uint8_t type, uint16_t command, uint16_t rid)
{
	packet->version 	= VERSION;
	packet->type 		= type;
	packet->command 	= command;
	packet->rid 		= rid;
	packet->length 		= 0;
}

void initPresencePacket(Packet *packet)
{
	initPacket(packet, PT_PRESENCE, 0, 0);
}

uint16_t initResponsePacket(Packet *packet, uint16_t command, uint16_t rid)
{
	//initPacket(packet, PT_RESPONSE, command, );
	
	return rid;
}

void processPacket(Packet *packet, uint16_t length, uint8_t * data)
{
	printf("%4x %d %4x %d %d\ndata: %s\n", packet->version, packet->type, packet->command, packet->rid, packet->length, data);

	switch(packet->type)
	{
		case PT_REQUEST:
			printf("received request: 0x%04x with data='%s'\n", packet->type, data);
			break;
	}
}

DWORD __stdcall ClientThread(void *ptr)
{
    TCHAR user[256];
    DWORD len = sizeof(user);
    GetUserName(user, &len);

	struct sockaddr_in server_addr;
	memset((char*) &server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1111);
    server_addr.sin_addr.s_addr = inet_addr("192.168.2.240");

	if (connect(mainSock, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		puts("error: connect()");
		return 1;
	}
	
	Packet pck;
	initPresencePacket(&pck);
	send(mainSock, (char*) &pck, sizeof(pck), 0);

	for(;;)
	{
		Packet buff;
		uint8_t data[1024];
		size_t result;
		
		result = recv(mainSock, (char*)&buff, sizeof(buff), 0);
		
		if (result == 0)
		{
			puts("connection gracefully closed.");
		}
		else if (result == SOCKET_ERROR)
		{
			puts("socket error occured.");
		}
		
		memset(data, 0, sizeof(data));
		if (buff.length > 0)
		{
			result = recv(mainSock, (char *) data, (buff.length < sizeof(data)) ? buff.length : sizeof(data), 0);
		}
		
		processPacket(&buff, buff.length, data);
		
	}
	
    return 0;
}

int main(void)
{
	WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != NO_ERROR)
	{
		puts("error: WSAStartup()");
		return 1;
	}

	mainSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSock == INVALID_SOCKET)
	{
		puts("error: socket()");
		return 1;
	}
	
	clientThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ClientThread, NULL, 0, NULL);
    WaitForSingleObject(clientThread, INFINITE);
	
	return 0;
}