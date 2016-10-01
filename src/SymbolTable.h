#ifndef SYMBOLTABLE
#define SYMBOLTABLE

#include <fstream>

using namespace std;

class Symbol;

class SymbolTable{

private:

    Symbol *first, *last, *lastSection;
    int counter;

public:

    SymbolTable();

    ~SymbolTable();

    void addSymbol(char*, int, int, char);

    void addSection(char*);

    Symbol* findSymbol(char*);

    void saveToFile(ofstream&);

    int getLastSectionID();
};

#endif
