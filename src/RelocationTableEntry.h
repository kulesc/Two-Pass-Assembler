#ifndef RELOCATIONTABLEENTRY
#define RELOCATIONTABLEENTRY

#include <string>

using namespace std;

class RelocationTableEntry{

public:

    RelocationTableEntry(int, string, int);

    ~RelocationTableEntry();

    int getOffset();

    string getType();

    int getValue();

    RelocationTableEntry* getNext();

    void setNext(RelocationTableEntry*);

private:

    int offset;
    string type;
    int value;
    RelocationTableEntry *next;

};

#endif
