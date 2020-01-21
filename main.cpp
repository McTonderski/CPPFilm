#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale>
#include <sstream>
#include <list>
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
            // printf("The movie %s was released in %s. \n", 
            //     brewery_node->first_attribute("title")->value(),
            //     brewery_node->first_attribute("year")->value());
            //     // Interate over the beers
            std::string a(const_charer(brewery_node->first_attribute("title")->value()));
            temp.push_back(a);
        }
        return temp;
    }
};

class Collection{
    list<std::string> lista_filmow;
    list<std::pair<std::string, int>> collection;
    public:
    Collection(std::string address = "samplexml.xml"){
        ifstream file(address);
        vector<char>buffer((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
        XMLParser parser;
        std::cout<<"id\t\t\tname\t\t\tscore"<<std::endl;
        lista_filmow = parser.extractor(buffer);
        int counter = 0;

        for(auto i: lista_filmow){
            std::cout<<counter++<<":\t\t\t"+i<<std::endl;
        }
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
            std::cout<<"2. Export your collection.________"<<std::endl;
            int temp_choice;
            string temp;
            std::cin>>temp;
            choice = std::stoi(temp);
            switch(choice)
            {
                case 1:
                {
                    std::string collection_link;
                    std::cout<<"Provide a path to your Collection ( a for default Collection ): ";
                    std::cin >> collection_link;
                    if(collection_link == "a")
                        Collection collection;
                    else
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