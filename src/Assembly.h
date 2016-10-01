#ifndef ASSEMBLY
#define ASSEMBLY

#include <fstream>
#include <string>

using namespace std;

class SymbolTable;

class StringTokenizer;

class RelocationTable;

class Assembly{

public:

    Assembly(char*, char*);

    ~Assembly();

    char* readLine();

    void createOutputFile();

    int determineTypeOfToken(char*);

    void firstPass();

    void secondPass();

    unsigned long long createMachineCode(char*, StringTokenizer*, RelocationTable*, int);

    string convertDecimalToHex(unsigned long long, int);

    void writeMachineCodeToFile(string, char*);

private:

    static const int LINE_LENGTH;
    static const int NUMBER_OF_DIRECTIVES = 7;
    static const int NUMBER_OF_MNEMONICS = 147;
    static const int NUMBER_OF_SECTIONS = 3;
    static const int NUMBER_OF_INSTRUCTIONS = 21;
    static const int NUMBER_OF_CONDITIONS = 7;
    static const char *sections[NUMBER_OF_SECTIONS];
    static const char *directives[NUMBER_OF_DIRECTIVES];
    static const char *mnemonics[NUMBER_OF_MNEMONICS];
    static const char *instructions[NUMBER_OF_INSTRUCTIONS];
    static const char *conditions[NUMBER_OF_CONDITIONS];

    char *inputFileName;
    char *outputFileName;
    ifstream inputFileStream;
    ofstream outputFileStream;
    SymbolTable *symbolTable;
    bool endOfProgram;

    RelocationTable **rTables;

    static const int PUBLIC;
    static const int EXTERN;
    static const int CHAR;
    static const int WORD;
    static const int LONG;
    static const int ALIGN;
    static const int SKIP;
    static const int END;
    static const int SECTION;
    static const int MNEMONIC;
    static const int REGISTER;
    static const int CONSTANT;

};

#endif
