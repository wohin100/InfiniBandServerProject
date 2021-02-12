#include "ConfigFileReader.h"
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>

void trimString(string &token);

void applyInputValueToProperty(const string &key, const string &value);

using namespace std;

int port = 80;
string dbUrl = "http://localhost";
string dbPort = "8086";
string dbName = "infiniband";

ConfigFileReader::ConfigFileReader() = default;

int ConfigFileReader::getPort(){
    return port;
}

string ConfigFileReader::getDbUrl(){
    return dbUrl;
}

string ConfigFileReader::getDbPort(){
    return dbPort;
}

string ConfigFileReader::getDbName(){
    return dbName;
}

void splitLine(string line){
    string delimiter = "=";

    size_t position = 0;
    string key;
    string value;
    int tokens = 0;
    while ((position = line.find(delimiter)) != string::npos) {
        key = line.substr(0, position);
        value = line.substr(position + 1, line.length());

        trimString(key);
        trimString(value);

        applyInputValueToProperty(key, value);

        line.erase(0, position + delimiter.length());
        tokens++;
    }
    if(tokens > 1){
        cerr << "Config file is invalid" << endl;
        throw "Config file is invalid";
    }
}

void applyInputValueToProperty(const string &key, const string &value) {
    if (key == "port"){
        port = stoi(value);
    }
    if (key == "dbUrl"){
        dbUrl = value;
    }
    if (key == "dbPort"){
        dbPort = value;
    }
    if (key == "dbName"){
        dbName = value;
    }
}

void trimString(string &token) {
    while(!token.empty() && isspace(token.front()) || isblank(token.front()) ){
        token.erase(token.begin());
    }
    while(!token.empty() && isspace(token.back()) || isblank(token.back()) ){
        token.pop_back();
    }
}

void ConfigFileReader::read(string path)
{
    ifstream file(path);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            splitLine(line);
        }
        file.close();
    }
    else{
        cerr << "Could not open config file" << endl;
        cerr << path << endl;
        exit(EXIT_FAILURE);
    }
}