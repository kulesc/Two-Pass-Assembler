#include "RelocationTableEntry.h"

RelocationTableEntry::RelocationTableEntry(int offset, string type, int value){

    this->offset = offset;
    this->type = type;
    this->value = value;
    this->next = nullptr;
}

RelocationTableEntry::~RelocationTableEntry(){


}

int RelocationTableEntry::getOffset(){

    return offset;
}

string RelocationTableEntry::getType(){

    return type;
}

int RelocationTableEntry::getValue(){

    return value;
}

void RelocationTableEntry::setNext(RelocationTableEntry *next){

    this->next = next;
}

RelocationTableEntry* RelocationTableEntry::getNext(){

    return next;
}
