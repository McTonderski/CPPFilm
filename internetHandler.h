#include <windows.h>
#include <string>
#include <stdio.h>
#include <iostream>

using std::string;

#pragma comment(lib,"ws2_32.lib")

HINSTANCE hInst;
WSADATA wsaData;
void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename);
SOCKET connectToServer(char *szServerName, WORD portNum);
int getHeaderLength(char *content);
char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);

class InternetHandler{
    public:
    std::string get_response(std::string title);
    void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename);
    SOCKET connectToServer(char *szServerName, WORD portNum);
    int getHeaderLength(char *content);
    char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);

};