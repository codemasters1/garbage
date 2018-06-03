#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <Ws2tcpip.h>
#include <unistd.h>
#include "main.h"


DWORD WINAPI SocketConnectionTimeout(ThreadOptions *opt)
{
    opt->error = connect(opt->s, opt->name, opt->namelen);

    return 0;
}

DWORD GetPageStatusCode(char *Ip, char *Path, DWORD *ErrorCode, DWORD Timeout)
{
    DWORD Status = 0;

    SOCKET sck = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sck == INVALID_SOCKET)
    {
        closesocket(sck);
        return WSAGetLastError();
    }

    setsockopt(sck, SOL_SOCKET, SO_RCVTIMEO, (char*) &Timeout, sizeof(Timeout));
    setsockopt(sck, SOL_SOCKET, SO_SNDTIMEO, (char*) &Timeout, sizeof(Timeout));

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.S_un.S_addr = inet_addr(Ip);
    server_addr.sin_port = htons(80);

    /*
    if (connect(sck, (struct sockaddr*) &server_addr, sizeof(server_addr)))
    {
        *ErrorCode = WSAGetLastError();
        closesocket(sck);
        return Status;
    }
    */

    ThreadOptions opt;
    opt.s       = sck;
    opt.name    = (struct sockaddr*) &server_addr;
    opt.namelen = sizeof(server_addr);

    HANDLE t = CreateThread(
        (LPSECURITY_ATTRIBUTES) 0,
        0,
        (LPTHREAD_START_ROUTINE) SocketConnectionTimeout,
        (PVOID) &opt,
        0,
        0);

    if (t == NULL)
    {
        return Status;
    }

    if (WaitForSingleObject(t, Timeout) == WAIT_TIMEOUT)
    {
        *ErrorCode = WSAETIMEDOUT;
        return Status;
    }

    if (opt.error)
    {
        *ErrorCode = WSAGetLastError();
        closesocket(sck);
        return Status;
    }

    char req[1024];

    strcpy(req, HEADER_METHOD);
    strcat(req, Path);
    strcat(req, HEADER_HTTP_VERSION);
    strcat(req, HEADER_EOL);
    strcat(req, HEADER_USER_AGENT);
    strcat(req, REQUEST_HEADER);
    strcat(req, HEADER_EOL);
    strcat(req, HEADER_HOST);
    strcat(req, Ip);
    strcat(req, HEADER_EOL);
    strcat(req, HEADER_CACHE_CONTROL);
    strcat(req, HEADER_EOL);
    strcat(req, HEADER_EOL);

    int sent = send(sck, req, sizeof(req), 0);

    if (sent == SOCKET_ERROR)
    {
        *ErrorCode = WSAGetLastError();
        closesocket(sck);
        return Status;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int recv_r = recv(sck, buffer, sizeof(buffer), 0);

    if (recv_r == SOCKET_ERROR)
    {
        *ErrorCode = WSAGetLastError();
        closesocket(sck);
        return Status;
    }
    else
    {
        char s[4];
        memset(s, 0, sizeof(s));

        if (memcmp(buffer, "HTTP/1.1", 8) == 0)
        {
           strncpy(s, buffer + 9, 3);
           Status = atoi(s);
        }
    }

    closesocket(sck);
    return Status;
}

/*

int GetUrlStatusCode(char *Url, DWORD *ErrorCode)
{
    HINTERNET hInternet = InternetOpen(
        "Finder v0.1",
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0 );

    if (hInternet == NULL)
    {
        //puts("InternetOpen");

        *ErrorCode = GetLastError();
        return 0;
    }

    DWORD Timeout = 750;

    InternetSetOption(hInternet, INTERNET_OPTION_CONNECT_TIMEOUT,       &Timeout, sizeof(DWORD));
    InternetSetOption(hInternet, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,  &Timeout, sizeof(DWORD));
    InternetSetOption(hInternet, INTERNET_OPTION_DATA_SEND_TIMEOUT,     &Timeout, sizeof(DWORD));
    InternetSetOption(hInternet, INTERNET_OPTION_RECEIVE_TIMEOUT,       &Timeout, sizeof(DWORD));
    InternetSetOption(hInternet, INTERNET_OPTION_SEND_TIMEOUT,          &Timeout, sizeof(DWORD));

    HINTERNET hFile = InternetOpenUrl(
        hInternet,
        Url,
        NULL,
        0,
        INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD,
        0);

    if (hFile == NULL)
    {
        *ErrorCode = GetLastError();

        return 0;
    }

    DWORD status_code = 0;
    DWORD length = sizeof(DWORD);

    if (HttpQueryInfo(
        hFile,
        HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
        &status_code,
        &length,
        NULL))
    {
        return status_code;

    }
    else
    {
        *ErrorCode = GetLastError();
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

    return 0;
}

*/

void buildUrl(const char *ip, char *buffer, size_t size)
{
    const char *protocol = "http://";
    const char *path = "/userRpmNatDebugRpm26525557/linux_cmdline.html";

    strcpy(buffer, protocol);
    strcat(buffer, ip);
    strcat(buffer, path);
}

char *CIDRtoIP(int CIDR)
{
    const char *masks[30] = {
        "128.0.0.0",
        "192.0.0.0",
        "224.0.0.0",
        "240.0.0.0",
        "248.0.0.0",
        "252.0.0.0",
        "254.0.0.0",
        "255.0.0.0",
        "255.128.0.0",
        "255.192.0.0",
        "255.224.0.0",
        "255.240.0.0",
        "255.248.0.0",
        "255.252.0.0",
        "255.254.0.0",
        "255.255.0.0",
        "255.255.128.0",
        "255.255.192.0",
        "255.255.224.0",
        "255.255.240.0",
        "255.255.248.0",
        "255.255.252.0",
        "255.255.254.0",
        "255.255.255.0",
        "255.255.255.128",
        "255.255.255.192",
        "255.255.255.224",
        "255.255.255.240",
        "255.255.255.248",
        "255.255.255.252"
    };

    if (CIDR >= 1 && CIDR <= 30)
    {
        return (char*) masks[CIDR - 1];
    }
    else
    {
        return NULL;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        puts("usage: scan [options] <ip|range>\n"
             "e.g. scan 192.168.1.1/24\n");

        return 2;
    }

    if (argc == 2 && strcmp(argv[1], "-h") == 0)
    {
        puts("help!\n");
        return 0;
    }

    int opt_timeout = 500;

    int c;

    while ((c = getopt(argc, argv, "t:")) != -1)
        switch (c)
        {
        case 't':
            opt_timeout = atoi(optarg);

            if (opt_timeout < TIMEOUT_MIN || opt_timeout > TIMEOUT_MAX)
            {
                printf("error: invalid timeout: it must be in range %u-%u", TIMEOUT_MIN, TIMEOUT_MAX);
                return 3;
            }

            break;
        case ':':
            printf("error: option -%c expects argument\n", optopt);
        break;
    }

    if (optind == argc)
    {
        puts("error: ip or range of adresses expected");
        return 2;
    }

    char ip[16];
    char mask = 0;

    memset(ip, 0, sizeof(ip));

    char *pos = strstr(argv[optind], "/");
    if (pos == NULL)
    {
        strcpy(ip, argv[optind]);
    }
    else
    {
        strncpy(ip, argv[optind], (DWORD) pos - (DWORD) argv[optind]);

        mask = atoi(pos + 1);
    }

    IN_ADDR sIP;
    sIP.S_un.S_addr = inet_addr(ip);

    if (sIP.S_un.S_addr == INADDR_NONE)
    {
        puts("error: invalid address\n");
        return 3;
    }

    WSADATA wsaData;

    if (WSAStartup( MAKEWORD(2, 0), &wsaData) != 0)
    {
        puts("error: could not initialize WSA");
    }

    if (mask > 0)
    {
        char *mask_ip = CIDRtoIP(mask);

        if (mask_ip == NULL)
        {
            puts("error: invalid mask\n");
            return 4;
        }

        IN_ADDR sMask;
        IN_ADDR sIP_end;

        sMask.S_un.S_addr = inet_addr(mask_ip);

        sIP_end.S_un.S_addr = ((~sMask.S_un.S_addr) | (sIP.S_un.S_addr & sMask.S_un.S_addr));
        sIP_end.S_un.S_un_b.s_b4 -= 1;
        sIP.S_un.S_addr = (sIP.S_un.S_addr & sMask.S_un.S_addr);
        sIP.S_un.S_un_b.s_b4 += 1;

        char IP[2][16];
        memset(IP[0], 0, sizeof(IP[0]));
        memset(IP[1], 0, sizeof(IP[0]));

        strncpy(IP[0], inet_ntoa(sIP), sizeof(IP[0]));
        strncpy(IP[1], inet_ntoa(sIP_end), sizeof(IP[0]));

        printf("Scanning ip range: %s - %s\n\n", IP[0], IP[1]);

        sIP_end.S_un.S_un_b.s_b4++;

        do {
            //memset(url, 0, sizeof(url));
            //buildUrl(inet_ntoa(sIP), url , sizeof(512));

            DWORD error = 0;
            DWORD status = GetPageStatusCode(inet_ntoa(sIP), "/userRpmNatDebugRpm26525557/linux_cmdline.html", &error, opt_timeout);

            if (status == 200)
            {
                printf("[+] %s: \t PROPABLY VULNERABLE!\n", inet_ntoa(sIP));
            }
            else
            {
                printf("[-] %s: \t not vulnerable (status: %u; error: %u)\n",
                    inet_ntoa(sIP),
                    (unsigned int) status,
                    (unsigned int) error);
            }

            if (sIP.S_un.S_un_b.s_b4 == 255)
            {
               if (sIP.S_un.S_un_b.s_b3 == 255)
               {
                   if (sIP.S_un.S_un_b.s_b2 == 255)
                   {
                        sIP.S_un.S_un_b.s_b1++;
                   }
                   sIP.S_un.S_un_b.s_b2++;
               }
               sIP.S_un.S_un_b.s_b3++;
            }
            sIP.S_un.S_un_b.s_b4++;
        } while(memcmp(&sIP, &sIP_end, sizeof(IN_ADDR)) != 0);
    }
    else
    {
        //memset(url, 0, sizeof(url));
        //buildUrl(ip, url , sizeof(url));

        DWORD error;
        DWORD status = GetPageStatusCode(ip, "/userRpmNatDebugRpm26525557/linux_cmdline.html", &error, opt_timeout);

        if (status == 200)
        {
            printf("%s: PROBABLY VULNERABLE!\n", ip);
        }
        else
        {
            printf("%s: NOT VULNERABLE!\n", ip);
        }
    }

    WSACleanup();

    return 0;
}
