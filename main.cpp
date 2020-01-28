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
#include "rapidxml/rapidxml_print.hpp"

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

    bool intractor(list<string> data){
        ofstream file("out.xml");
        xml_document <> doc;    
        xml_node<>* decl = doc.allocate_node(node_declaration);
        decl->append_attribute(doc.allocate_attribute("version", "1.0"));
        decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
        doc.append_node(decl);

        xml_node<>* pgnx = doc.allocate_node(node_element, "root");
        pgnx->append_attribute(doc.allocate_attribute("response", "TRUE"));
        doc.append_node(pgnx);
        list<string>::iterator it;
        for (it = data.begin(); it != data.end(); it++)
        {
            xml_node<>* child = doc.allocate_node(node_element, "movie");
            child->append_attribute(doc.allocate_attribute("name", (*it).c_str()));
            pgnx->append_node(child);
        }

        file << doc;


        file.close();
        doc.clear();

        return false;
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
        int iterator = 0;
        for(auto i: lista_filmow){
            std::cout<<iterator<<": "<<i<<std::endl;
            iterator++;
        }
    }
    void add_movie(){
        cout<<"Write Movie title you want to add to your collection: ";
        string movie_name;
        cin >> movie_name;
        string url = "http://www.omdbapi.com/?apikey=6d584491^&r=xml^&t=";
        url += movie_name;
        auto answea = exec(("curl " + url).c_str());
        XMLParser parser;
        std::vector<char> data(answea.begin(), answea.end());
        auto response = parser.extractor(data);
        lista_filmow.merge(response);
        print_collection();
        collection_menu();
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
        list<string>::iterator it;
        string temp;
        cin>>temp;
        advance(it, stoi(temp));
        lista_filmow.erase(it);
        print_collection();
    }

    void export_collection(){
        XMLParser parser;
        if(parser.intractor(lista_filmow))
        {
            cout<<"FILE HAS BEEN EXPORTED SUCCESSFULLY"<<endl;
        }
        else{
            cout<<"There were some errors"<<endl;
        }
        print_collection();
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