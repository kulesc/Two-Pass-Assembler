#ifndef SYMBOL
#define SYMBOL

class Symbol{

private:

    char* name;
    int section, offset;
    char visibility;
    int symbolNo;
    Symbol* next;

public:

    char* getName();

    void setName(char*);

    int getSection();

    void setSection(int);

    int getOffset();

    void setOffset(int);

    char getVisibility();

    void setVisibility(char);

    int getSymbolNo();

    void setSymbolNo(int);

    Symbol* getNext();

    void setNext(Symbol*);

    Symbol();

    Symbol(char*, int, int, char, int);

    ~Symbol();
};

#endif
