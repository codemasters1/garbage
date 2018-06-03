#ifndef _CMD_H
#define _CMD_H

#include "common.h"

void CmdDoScreenshot(Command *, Response *);
//void CmdDownloadFile(Command *, int, TCHAR **);
//void CmdUploadFile(Command *, int, TCHAR **);
//void CmdRun(Command *, int, TCHAR **);

extern CommandHandler commands[] = {
    {0x10,          CmdDoScreenshot}
};

#endif // _CMD_H
