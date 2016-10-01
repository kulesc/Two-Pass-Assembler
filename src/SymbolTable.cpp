#include "SymbolTable.h"
#include "Symbol.h"
#include <cstring>
#include <fstream>
#include <iomanip>

using namespace std;

SymbolTable::SymbolTable(){
    first = last = lastSection = new Symbol();
    counter = 1;
}

SymbolTable::~SymbolTable(){
    while(first){
        last = first;
        first = first->getNext();
        delete last;
    }
    first = last = lastSection = nullptr;
}

void SymbolTable::addSymbol(char* name, int section, int offset, char visibility){
    Symbol *newSymbol = new Symbol(name, section, offset, visibility, counter++);
    last->setNext(newSymbol);
    last = newSymbol;
}

void SymbolTable::addSection(char* name){
    Symbol *newSymbol = new Symbol(name, lastSection->getSymbolNo() + 1, 0, 'l', lastSection->getSymbolNo() + 1);
    newSymbol->setNext(lastSection->getNext());
    lastSection->setNext(newSymbol);
    if(last == lastSection) last = newSymbol;
    lastSection = newSymbol;
    while(newSymbol->getNext()){
        newSymbol = newSymbol->getNext();
        newSymbol->setSymbolNo(newSymbol->getSymbolNo() + 1);
    }
    counter++;
}

Symbol* SymbolTable::findSymbol(char* name){
    Symbol *result = first;
    while(result){
        if(strcmp(name, result->getName()) == 0) break;
        result = result->getNext();
    }
    return result;
}

void SymbolTable::saveToFile(ofstream& file){

    Symbol *tmp = first;
    file << setw(10) << "SymbolNo" << setw(15) << "SymbolName" << setw(10) <<
    "Section" << setw(10) << "Offset" << setw(15) << "Visibility" << endl << endl;
    while(tmp){
        file << setw(10) << tmp->getSymbolNo() << setw(15) << tmp->getName() << setw(10)
        << tmp->getSection() << setw(10) << tmp->getOffset() << setw(15) << tmp->getVisibility()
        << endl;
        tmp = tmp->getNext();
    }
}

int SymbolTable::getLastSectionID(){

    return lastSection->getSymbolNo();
}
