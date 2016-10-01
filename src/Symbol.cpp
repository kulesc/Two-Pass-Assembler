#include "Symbol.h"
#include <iostream>

char* Symbol::getName(){
    return name;
}

void Symbol::setName(char* name){
    this->name = name;
}

int Symbol::getSection(){
    return section;
}

void Symbol::setSection(int section){
    this->section = section;
}

int Symbol::getOffset(){
    return offset;
}

void Symbol::setOffset(int offset){
    this->offset = offset;
}

char Symbol::getVisibility(){
    return visibility;
}

void Symbol::setVisibility(char visibility){
    this->visibility = visibility;
}

int Symbol::getSymbolNo(){
    return symbolNo;
}

void Symbol::setSymbolNo(int symbolNo){
    this->symbolNo = symbolNo;
}

Symbol* Symbol::getNext(){
    return next;
}

void Symbol::setNext(Symbol* next){
    this->next = next;
}

Symbol::Symbol(){
    this->name = new char[10];
    name[0] = 'U';
	name[1] = 'N';
	name[2] = 'D';
	name[3] = 'E';
	name[4] = 'F';
	name[5] = 'I';
	name[6] = 'N';
	name[7] = 'E';
	name[8] = 'D';
	name[9] = '\0';
    this->section = 0;
    this->offset = 0;
    this->visibility = 'l';
    this->symbolNo = 0;
    this->next = nullptr;
}

Symbol::Symbol(char* name, int section, int offset, char visibility, int symbolNo){
    this->name = name;
    this->section = section;
    this->offset = offset;
    this->visibility = visibility;
    this->symbolNo = symbolNo;
    this->next = nullptr;
}

Symbol::~Symbol(){
    delete name;
}
