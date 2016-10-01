#ifndef RELOCATIONTABLE
#define RELOCATIONTABLE

#include <fstream>
#include <string>

using namespace std;

class RelocationTableEntry;

class RelocationTable{

public:

    RelocationTable(char*);

    ~RelocationTable();

    void insertNewEntry(int, string, int);

    void writeTableToFile(ofstream&);

private:

    char *sectionName;
    RelocationTableEntry *first, *last;

    string convertDecimalToHex(unsigned long long decimal, int b);
};

#endif
