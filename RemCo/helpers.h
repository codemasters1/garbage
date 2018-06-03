#ifndef _HELPERS_H
#define _HELPERS_H

#include <windows.h>

int FtpSendFile(const TCHAR *, const TCHAR*, const TCHAR *, const TCHAR *, const TCHAR *, const TCHAR *, BOOL);
int URLDownloadToFile(TCHAR * const, TCHAR * const);

BOOL FileExists(const TCHAR *);
TCHAR **parseCommand(TCHAR *, int *);
TCHAR *basename(TCHAR *);
void saveSS(TCHAR *, DWORD);

#endif // _HELPERS_H
