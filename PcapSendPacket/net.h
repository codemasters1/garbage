#ifndef __NET_H
#define __NET_H

#include <stdint.h>

typedef struct EthAddrStruct {
    uint8_t addr[6];
} EthAddr;

typedef struct EthPacketStruct {
    EthAddr dst;
    EthAddr src;
    uint16_t type;
} EthPacket;

typedef struct PPPoEPacketStruct {
    EthPacket eth;
    uint8_t ver:4;
    uint8_t type:4;
    uint8_t code;
    uint16_t session;
    uint16_t length;
    uint8_t payload[12];
} PPPoEPacket;

#endif // NET_H_INCLUDED
