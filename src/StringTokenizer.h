#ifndef STRINGTOKENIZER
#define STRINGTOKENIZER

class StringTokenizer{

public:

    StringTokenizer(char*);

    ~StringTokenizer();

    char* getNextToken();

    bool wasLastTokenLabel();

    int calculateNumberOfArguments();

private:

    char *line;

    bool label;

    int position;
};

#endif
