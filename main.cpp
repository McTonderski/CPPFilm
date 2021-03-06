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
#include <iomanip>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <string>
#include <fstream>
#include <map>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

using namespace rapidxml;
using namespace std;

#pragma comment(lib,"ws2_32.lib")

class Movie{
private:
    string title;
    int year;
    int rate;
public:
    Movie(string title, int year, int rate){
        this->title = title;
        this->year = year;
        this->rate = rate;
    }
    bool operator < (Movie const &rhs){
        return true;
    }
    void set_title(string title){
        this->title = title;
    }
    void set_year(int year){
        this->year = year;
    }
    void set_rate(int rating){
        this->rate = rating;
    }
    string get_title() const {
        return this->title;
    }
    int get_year() const {
        return this->year;
    }
    int get_rate() const {
        return this->rate;
    }
};


class XMLParser{
    private:
        std::string const_charer(const char * inpute){
            return inpute;
        }
    public:
    list<Movie> extractor(vector<char> buffer)
    {
        xml_document <> doc;
        xml_node<> * root_node;
        buffer.push_back('\0');
        list<Movie> temp;
        doc.parse<0>(&buffer[0]);
        root_node = doc.first_node("root");
        for (xml_node<> * brewery_node = root_node->first_node("movie"); brewery_node != nullptr; brewery_node = brewery_node->next_sibling())
        {   
            Movie a(const_charer(brewery_node->first_attribute("title")->value()), stoi(const_charer(brewery_node->first_attribute("year")->value())), stoi(const_charer(brewery_node->first_attribute("imdbRating")->value())));
            temp.push_back(a);
        }
        return temp;
    }

    bool intractor(list<Movie> data){
        ofstream file("out.xml");
        xml_document <> doc;    
        xml_node<>* decl = doc.allocate_node(node_declaration);
        decl->append_attribute(doc.allocate_attribute("version", "1.0"));
        decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
        doc.append_node(decl);

        xml_node<>* pgnx = doc.allocate_node(node_element, "root");
        pgnx->append_attribute(doc.allocate_attribute("response", "TRUE"));
        doc.append_node(pgnx);
        list<Movie>::iterator it;
        for (it = data.begin(); it != data.end(); it++)
        {
            xml_node<>* child = doc.allocate_node(node_element, "movie");
            child->append_attribute(doc.allocate_attribute("title", it->get_title().c_str()));
            child->append_attribute(doc.allocate_attribute("year", to_string(it->get_year()).c_str()));
            child->append_attribute(doc.allocate_attribute("imdbRating", to_string(it->get_rate()).c_str()));
            pgnx->append_node(child);
        }

        file << doc;


        file.close();
        doc.clear();

        return true;
    }
};

class Collection{
private:
    list<Movie> lista_filmow;
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

    Collection(string address, bool tru){
        addr = address;
        ifstream file(address);
        vector<char>buffer((istreambuf_iterator<char>(file)),istreambuf_iterator<char>());
        XMLParser parser;
        lista_filmow = parser.extractor(buffer);
        parser.intractor(lista_filmow);
    }

    void print_collection(){
        int iterator = 0;
        cout<<"id"<<setw(20)<<"title"<<setw(20)<<"year"<<setw(20)<<"rate"<<endl;
        list<Movie>::iterator it;
        for(it = lista_filmow.begin(); it != lista_filmow.end(); it++){
            cout<<iterator<<setw(20)<<it->get_title()<<setw(20)<<it->get_year()<<setw(20)<<it->get_rate()<<std::endl;
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
        list<Movie> response = parser.extractor(data);
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
        list<Movie>::iterator it;
        string temp;
        cin>>temp;
        it = lista_filmow.begin();
        advance(it, stoi(temp));
        lista_filmow.erase(it);
        print_collection();
        collection_menu();
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
        collection_menu();
    }

    void sort_collection(){
        std::cout<<"------Sort after:------"<<std::endl;
        cout<<"1. title"<<endl;
        cout<<"2. year"<<endl;
        cout<<"3. rate"<<endl;
        cout<<"4. exit"<<endl;
        string temp;
        cin>>temp;
        switch(stoi(temp)){
            case 1:
                sort_title();
                break;
            case 2:
                sort_year();
                break;
            case 3:
                sort_rate();
                break;
            case 4:
                return;
                break;
            default:
                collection_menu();
                break;
        };      
    }
    struct LastNameComp {
        bool operator()(const Movie& a, const Movie& b) {
            return std::lexicographical_compare(
               a.get_title().begin(), a.get_title().end(),
               b.get_title().begin(), b.get_title().end()  
            );
        }
    };
    void sort_title(){
        lista_filmow.sort(LastNameComp());
        print_collection();
        collection_menu();
    }
    
    void sort_year(){
        lista_filmow.sort([](const Movie & l, const Movie & r){
            return (l.get_year() < r.get_year()) ? true : false;
        });
        print_collection();
        collection_menu();
    }
    
    void sort_rate(){
        lista_filmow.sort([](const Movie & l, const Movie & r){
            return (l.get_rate() < r.get_rate()) ? true : false;
        });
        print_collection();
        collection_menu();
    }

    void collection_menu(){
        std::cout<<"------Collection menu------"<<std::endl;
        cout<<"1. add movie"<<endl;
        cout<<"2. delete movie"<<endl;
        cout<<"3. export collection"<<endl;
        cout<<"4. sort collection"<<endl;
        cout<<"5. exit"<<endl;
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
            case 4:
                sort_collection();
                break;
            case 5:
                return;
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
        void display_menu(){
            std::cout<<"        MOVIE DB                    "<<std::endl;
            std::cout<<"1. Show your collection.            "<<std::endl;
            std::cout<<"2. Load your collection.            "<<std::endl;
            std::cout<<"3. Export your collection.          "<<std::endl;
            std::cout<<"4. Exit.                            "<<std::endl;
            int temp_choice;
            string temp;
            std::cin>>temp;
            choice = std::stoi(temp);
            switch(choice)
            {
                case 1:
                {
                    std::string collection_link = "samplexml.xml";
                    Collection collection(collection_link);
                    break;
                }
                case 2:
                {
                    std::string collection_link;
                    std::cin >> collection_link;
                    Collection collection(collection_link);
                    break;
                }
                case 3: 
                {
                    string link = "samplexml.xml";
                    Collection collection(link, true);
                    this->display_menu();
                    break;
                }
                case 4:
                    return;
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
    // XMLParser xml;
    // xml.extractor();
    // DisplayWeb web;
    // cout<<web.getXML();
    
    return 0;
}