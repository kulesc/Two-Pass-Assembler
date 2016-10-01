#include "StringTokenizer.h"
#include <iostream>
#include <cstring>

using namespace std;

StringTokenizer::StringTokenizer(char* line){

    this->line = line;
    this->position = 0;
    this->label = false;
}

StringTokenizer::~StringTokenizer(){

    delete [] line;
    line = nullptr;
}

bool StringTokenizer::wasLastTokenLabel(){

    return label;
}

char* StringTokenizer::getNextToken(){
    while((line[position] == ' ' || line[position] == '\n') && line[position] != '\0') position++;

    if(line[position] == '\0') return nullptr;

    int length = 0;
    bool isDelimiter = false;
    char *tokenStartPosition = line + position;

    while(line[position] != ':' && line[position] != ',' && line[position] != ' ' && line[position] != '\0'){

        position++;
        length++;
    }

    if(length == 0 && (line[position] == ':' || line[position] == ',')){
            isDelimiter = true;
            position++;
    }

    if(isDelimiter) return getNextToken();

    if(line[position] == ':') label = true;
    else label = false;

    char *token = new char[length + 1];
    for(int i = 0; i < length; i++) token[i] = tokenStartPosition[i];
    token[length] = '\0';

    return token;
}

int StringTokenizer::calculateNumberOfArguments(){

    int x = 0;
    for(unsigned int i = 0; i < strlen(line); i++) if(line[i] == ',') x++;
    return x;
}
