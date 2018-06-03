#define HAVE_REMOTE

#include <pcap.h>
#include <stdio.h>
#include "net.h"


void print_packet(unsigned char *packet, int len)
{
    int i = 0;

    for (; i < len; i++)
    {
        printf("%02x ", packet[i]);
    }
}


int main()
{
    printf("EthPacket[%d]\n", sizeof(EthPacket));
    printf("PPPoEPacket[%d]\n", sizeof(PPPoEPacket));

    pcap_t *device;
	char errbuf[PCAP_ERRBUF_SIZE];

	if ((device = pcap_open_live("rpcap://\\Device\\NPF_{887CFCA6-5F05-484A-BEA4-3B5F500A0888}", 65536,	1, 1000, errbuf)) == NULL)
	{
		fprintf(stderr,"\nUnable to open the adapter.\n");
		return 2;
	}

	PPPoEPacket packet;
	memset(&packet, 0, sizeof(packet));

    packet.eth.dst.addr[0] = 0xFF;
    packet.eth.dst.addr[1] = 0xFF;
    packet.eth.dst.addr[2] = 0xFF;
    packet.eth.dst.addr[3] = 0xFF;
    packet.eth.dst.addr[4] = 0xFF;
    packet.eth.dst.addr[5] = 0xFF;

    packet.eth.src.addr[0] = 0xBC;
    packet.eth.src.addr[1] = 0xAE;
    packet.eth.src.addr[2] = 0xC5;
    packet.eth.src.addr[3] = 0x63;
    packet.eth.src.addr[4] = 0xC6;
    packet.eth.src.addr[5] = 0x46;

    packet.eth.type = htons(0x8863);

    packet.ver = 1;
    packet.type = 1;
    packet.code = 0x09;
    packet.session = 0x0000;
    packet.length = htons(12);

    print_packet((unsigned char *) &packet, sizeof(packet));

	/* Send down the packet */
	if (pcap_sendpacket(device,	(unsigned char *) &packet, sizeof(packet)) != 0)
	{
		fprintf(stderr,"\nError sending the packet: %s\n", pcap_geterr(device));
		return 3;
	}

	pcap_close(device);
	return 0;

    return 0;
}
