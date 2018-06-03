#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>

#define VERSION 		0x1010

#define PT_PRESENCE 	0x01
#define PT_WELCOME      0x02
#define PT_PING         0x03
#define PT_PONG         0x04
#define PT_REQUEST		0x10
#define PT_RECV_ACK		0x11
#define PT_RESPONSE		0x12
#define PT_CMD_STATUS   0x13
#define PT_UNKNOWN      0xFF

#define RC_SS 0x10

typedef struct {
	uint16_t version	:16;
	uint8_t type		: 8;
	uint8_t command	    : 8; // for request
	uint16_t rid		:16; // for request, acknowledge, response
	int16_t code        :16; // code for response
	uint16_t length		:16;
	uint8_t data[0];
} Packet;

typedef struct
{
    uint16_t rid;
    uint8_t type;
    uint8_t **args;
} Command;

typedef struct {
    uint16_t rid;
    int16_t code;
    uint16_t length;
    uint8_t data;
} Response;

typedef void (*CommandCallback)(Command *, Response *);

typedef struct {
    uint16_t command;
    CommandCallback callback;
} CommandHandler;

#endif // _COMMON_H
