#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale>
#include <sstream>
#include "rapidxml/rapidxml.hpp"
using namespace std;

#pragma comment(lib,"ws2_32.lib")

class DisplayWeb{
    public:
    std::string getXML(){
        WSADATA wsaData;
        SOCKET Socket;
        SOCKADDR_IN SockAddr;
        int lineCount=0;
        int rowCount=0;
        struct hostent *host;
        std::cout<<"here";
        locale local;
        char buffer[10000];
        int i = 0 ;
        int nDataLength;
        string website_HTML;

        //api key
        string API_KEY = "6d584491";
        //website url
        string url =  "http://www.omdbapi.com/?&apikey=" + API_KEY + "&t=avengers";
        cout<<url;
        //HTTP GET
        string get_http = "GET / HTTP/1.1\r\nHost: " + url + "\r\nConnection: close\r\n\r\n";

            if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0){
                cout << "WSAStartup failed.\n";
                system("pause");
                //return 1;
            }

            Socket=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
            host = gethostbyname(url.c_str());

            SockAddr.sin_port=htons(80);
            SockAddr.sin_family=AF_INET;
            SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);

            if(connect(Socket,(SOCKADDR*)(&SockAddr),sizeof(SockAddr)) != 0){
                cout << "Could not connect";
                system("pause");
                //return 1;
            }

            
            // send GET / HTTP
            send(Socket,get_http.c_str(), strlen(get_http.c_str()),0 );

            // recieve html
            while ((nDataLength = recv(Socket,buffer,10000,0)) > 0){        
                int i = 0;
                while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r'){
                    cout<<buffer[i];
                    website_HTML+=buffer[i];
                    i += 1;
                }               
            }
            
            std::cout<<"here";

            closesocket(Socket);
            WSACleanup();
            cout<<website_HTML<<"html";
            return website_HTML;
    }
};


class Collection{
    private:
        //std::map() movies;
};

class XMLParser{
    public:
    void init(){
        DisplayWeb web;
        string xml_target = web.getXML();
        std::cout<<xml_target;
    }
};

class Menu{
    private:
        int choice {};
    public:    
        void display_collection(){

        }
        void display_menu(){
            std::cout<<"________MOVIE DB__________________"<<std::endl;
            std::cout<<"1. Show your collection.__________"<<std::endl;
            std::cout<<"2. Load your collection.__________"<<std::endl;
            std::cout<<"3. Export your collection.________"<<std::endl;
            int temp_choice;
            string temp;
            std::cin>>temp;
            choice = std::stoi(temp);
            switch (choice)
            {
            case 1:
                XMLParser parser;
                parser.init();
                break;
            
            default:
                display_menu();
                break;
            }
        }
};





int main( void ){
    Menu menu;
    menu.display_menu();
    return 0;
}