#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale>
#include <stdio.h>
#include <sstream>
#include <list>
#include <stdlib.h>
#include <array>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <fstream>
#include <map>
#include "rapidxml/rapidxml.hpp"
#include "internetHandler.h"

using namespace rapidxml;
using namespace std;

#pragma comment(lib,"ws2_32.lib")


class XMLParser{
    private:
        std::string const_charer(const char * inpute){
            return inpute;
        }
    public:
    list<std::string> extractor(vector<char> buffer)
    {
        xml_document <> doc;
        xml_node<> * root_node;
        buffer.push_back('\0');
        list<std::string> temp;
        doc.parse<0>(&buffer[0]);
        root_node = doc.first_node("root");
        for (xml_node<> * brewery_node = root_node->first_node("movie"); brewery_node != nullptr; brewery_node = brewery_node->next_sibling())
        {   
            std::string a(const_charer(brewery_node->first_attribute("title")->value()));
            temp.push_back(a);
        }
        return temp;
    }
};

class Collection{
private:
    list<std::string> lista_filmow;
    std::string addr;
public:
    Collection(std::string address){
        addr = address;
        ifstream file(address);
        vector<char>buffer((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
        XMLParser parser;
        lista_filmow = parser.extractor(buffer);
        print_collection();
        collection_menu();
    }  
    void print_collection(){
        for(auto i: lista_filmow){
            std::cout<<i<<std::endl;
        }
    }
    void add_movie(){
        cout<<"Write Movie title you want to add to your collection: ";
        string movie_name;
        cin >> movie_name;
        string url = "http://www.omdbapi.com/?&apikey=6d584491&r=xml&t=";
        auto answea = exec(("curl " + url + movie_name).c_str());
        cout<<answea;
    }

    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
    return result;
}

    void del_movie(){

    }

    void export_collection(){

    }

    void collection_menu(){
        std::cout<<"------Collection menu------"<<std::endl;
        cout<<"1. add movie"<<endl;
        cout<<"2. delete movie"<<endl;
        cout<<"3. export collection"<<endl;
        string temp;
        cin>>temp;
        switch(stoi(temp)){
            case 1:
                add_movie();
                break;
            case 2:
                del_movie();
                break;
            case 3:
                export_collection();
                break;
            default:
                collection_menu();
                break;
        };      
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
            switch(choice)
            {
                case 1:
                {
                    std::string collection_link;
                    std::cin >> collection_link;
                    Collection collection(collection_link);
                    break;
                }
                default:
                    display_menu();
                    break;
            }
        }
};





int main( void ){
    Menu menu;
    menu.display_menu();
    // XMLParser xml;
    // xml.extractor();
    // DisplayWeb web;
    // cout<<web.getXML();
    
    return 0;
}