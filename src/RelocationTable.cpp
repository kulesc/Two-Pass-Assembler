#include "RelocationTable.h"
#include "RelocationTableEntry.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <cstring>

using namespace std;

/*
 * Method used to insert new entry
 * in relocation table.
 */
void RelocationTable::insertNewEntry(int offset, string type, int value){

    if(first == nullptr) first = last = new RelocationTableEntry(offset, type, value);
    else{
        RelocationTableEntry *newEntry = new RelocationTableEntry(offset, type, value);
        last->setNext(newEntry);
        last = newEntry;
    }
}

RelocationTable::RelocationTable(char *sectionName){

    this->sectionName = sectionName;
    first = last = nullptr;
}

RelocationTable::~RelocationTable(){

    while(first){
        last = first;
        first = first->getNext();
        delete last;
    }
    first = last = nullptr;
    delete [] sectionName;
    sectionName = nullptr;
}

/*
 * This method converts decimal number
 * to hexadecimal version.
 */
string RelocationTable::convertDecimalToHex(unsigned long long decimal, int b){

    string result = "";
    for(int i = 0; i < 2 * b; i++){
        unsigned long long tmp = decimal & 15;
        tmp += tmp >= 10 ? 55 : 48;
        result = (char)tmp + result;
        decimal >>= 4;
    }
    return result;
}

/*
 * This method formats and writes the relocation
 * table to a file.
 */
void RelocationTable::writeTableToFile(ofstream& file){

    RelocationTableEntry *tmp = first;
    file << endl << endl << '#';
    for(unsigned int i = 0; i < strlen(sectionName); i++) file << sectionName[i];
    file << endl;
    file << endl << setw(10) << "Offset" << setw(15) << "Type" << setw(15) <<
    "Value" << endl << endl;
    while(tmp){
        file << setw(10) << convertDecimalToHex(tmp->getOffset(), 4) << setw(15) << tmp->getType() << setw(15)
        << tmp->getValue() << endl;
        tmp = tmp->getNext();
    }
}
