#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

HINSTANCE hInst;
WSADATA wsaData;
void mParseUrl(char *mUrl, std::string &serverName, std::string &filepath, std::string &filename);
SOCKET connectToServer(char *szServerName, WORD portNum);
int getHeaderLength(char *content);
char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);

class InternetHandler{
    public:
    std::string get_response(std::string title);
    void mParseUrl(char *mUrl, std::string &serverName, std::string &filepath, std::string &filename);
    SOCKET connectToServer(char *szServerName, WORD portNum);
    int getHeaderLength(char *content);
    char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);

};