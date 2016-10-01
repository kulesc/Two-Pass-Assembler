#include "Assembly.h"
#include <fstream>
#include "Error.h"
#include <cstring>
#include <string>
#include "SymbolTable.h"
#include "Symbol.h"
#include "StringTokenizer.h"
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "RelocationTable.h"

using namespace std;

/* Creates new instance of assembly analyzer
 * that takes name of input and output file.
 */
Assembly::Assembly(char *inputFileName, char *outputFileName){

    this->inputFileName = inputFileName;
    inputFileStream.open(inputFileName, fstream::in);
	if(!inputFileStream.is_open()) throw Error(0);

	this->outputFileName = outputFileName;
	outputFileStream.open(outputFileName, fstream::out);
	if(!outputFileStream.is_open()) throw Error(1);

	this->symbolTable = new SymbolTable();
	this->endOfProgram = false;
	this->rTables = nullptr;
}

/*
 * Closing all used resources before instance
 * is destroyed.
 */
Assembly::~Assembly(){

    if(inputFileStream.is_open()) inputFileStream.close();
    if(outputFileStream.is_open()) outputFileStream.close();
    for(int i = 0; i < symbolTable->getLastSectionID(); i++) delete rTables[i];
    delete rTables;
    delete symbolTable;
}

/*
 * This method reads one line from input file
 * until it reaches end of file. In that case
 * it returns null pointer.
 */
char* Assembly::readLine(){

    char *newLine = new char[LINE_LENGTH];
    if(!inputFileStream.eof()){
        inputFileStream.getline(newLine, 100);
        char *line = new char[strlen(newLine) + 1];
        strcpy(line, newLine);
        delete [] newLine;
        return line;
    } else return NULL;
}

/*
 * This method creates a .txt variant of
 * described elf format used in class.
 */
void Assembly::createOutputFile(){

    symbolTable->saveToFile(outputFileStream);
}

/*
 * This constant represents longest line that
 * can be read from input file.
 */
const int Assembly::LINE_LENGTH = 101;

/*
 * Array containing all possible directives used in
 * described assembly language.
 */
const char *Assembly::directives[] = {
    "public", "extern", "char", "word",
    "long", "align", "skip"};

/*
 * Array containing all possible operation mnemonics
 * used in described assembly language.
 */
const char* Assembly::mnemonics[] = {
    "inteq", "intne", "intgt", "intge", "intlt", "intle", "intal",
    "addeq", "addne", "addgt", "addge", "addlt", "addle", "addal",
    "subeq", "subne", "subgt", "subge", "sublt", "suble", "subal",
    "muleq", "mulne", "mulgt", "mulge", "mullt", "mulle", "mulal",
    "diveq", "divne", "divgt", "divge", "divlt", "divle", "dival",
    "cmpeq", "cmpne", "cmpgt", "cmpge", "cmplt", "cmple", "cmpal",
    "andeq", "andne", "andgt", "andge", "andlt", "andle", "andal",
    "oreq", "orne", "orgt", "orge", "orlt", "orle", "oral",
    "noteq", "notne", "notgt", "notge", "notlt", "notle", "notal",
    "testeq", "testne", "testgt", "testge", "testlt", "testle", "testal",
    "ldreq", "ldrne", "ldrgt", "ldrge", "ldrlt", "ldrle", "ldral",
    "streq", "strne", "strgt", "strge", "strlt", "strle", "stral",
    "calleq", "callne", "callgt", "callge", "calllt", "callle", "callal",
    "ineq", "inne", "ingt", "inge", "inlt", "inle", "inal",
    "outeq", "outne", "outgt", "outge", "outlt", "outle", "outal",
    "moveq", "movne", "movgt", "movge", "movlt", "movle", "moval",
    "shreq", "shrne", "shrgt", "shrge", "shrlt", "shrle", "shral",
    "shleq", "shlne", "shlgt", "shlge", "shllt", "shlle", "shlal",
    "ldcheq", "ldchne", "ldchgt", "ldchge", "ldchlt", "ldchle", "ldchal",
    "ldcleq", "ldclne", "ldclgt", "ldclge", "ldcllt", "ldclle", "ldclal",
    "ldceq", "ldcne", "ldcgt", "ldcge", "ldclt", "ldcle", "ldcal"
};

/*
 * Array containing section names in described
 * assembly language.
 */
const char* Assembly::sections[] = {
    "text", "data", "bss"};

/*
 * Array containing instructions defined
 * in the described assembly language.
 */
const char* Assembly::instructions[] = {
    "int", "add", "sub", "mul", "div",
    "cmp", "and", "or", "not", "test",
    "ldr", "str", "call", "in", "out",
    "mov", "shr", "shl", "ldch", "ldcl",
    "ldc"
};

/*
 * Array containing conditional parts
 * of instruction mnemonics.
 */
const char* Assembly::conditions[] = {
    "eq", "ne", "gt", "ge", "lt", "le",
    "al"};

/*
 * Constants used to determine what kind of token
 * is currently being analysed.
 */
const int Assembly::PUBLIC = 1;
const int Assembly::EXTERN = 2;
const int Assembly::CHAR = 3;
const int Assembly::WORD = 4;
const int Assembly::LONG = 5;
const int Assembly::ALIGN = 6;
const int Assembly::SKIP = 7;
const int Assembly::END = 8;
const int Assembly::SECTION = 9;
const int Assembly::MNEMONIC = 10;
const int Assembly::REGISTER = 11;
const int Assembly::CONSTANT = 12;

/*
 * This method takes token as an argument and
 * determines type of legal expression in
 * described assembly language or returns zero
 * if expression is not valid.
 */
int Assembly::determineTypeOfToken(char* token){

    if(token == nullptr) return 0;

    if(token[0] == '.'){

        /* check if end of program is reached */
        if(strcmp(token + 1, "end") == 0) return END;

        /* check if token is directive */
        for(int i = 0; i < NUMBER_OF_DIRECTIVES; i++)
            if(strcmp(directives[i], token + 1) == 0) return i + 1;

        /* check if token is section */
        for(int i = 0; i < NUMBER_OF_SECTIONS; i++){
            bool isSection = true;
            for(unsigned int j = 0; j < strlen(sections[i]); j++){
                if(sections[i][j] != token[j + 1]){
                    isSection = false;
                    break;
                }
            }
            if(isSection) return SECTION;
        }

        throw Error(2);
    }

    /* check if token is valid mnemonic */
    for(int i = 0; i < NUMBER_OF_MNEMONICS; i++) if(strcmp(token, mnemonics[i]) == 0) return MNEMONIC;

    /* check if token is register */
    if(token[0] == 'r' || token[0] == 'R'){
        int x = atoi(token + 1);
        if(x < 0 || x > 19) throw Error(13);
        return REGISTER;
    }
    /* check if token is constant */
    if(token[0] == '#'){
        unsigned int i;
        if(token[1] == '-') i = 2;
        else i = 1;
        if(i == 2 && token[2] == '\0') throw Error(14);
        for(; i < strlen(token); i++) if(!isdigit(token[i])) throw Error(14);
        return CONSTANT;
    }
    return 0;
}

/*
 * This method goes through input file
 * and creates symbol table and everything
 * else that should be created in the first
 * pass of assembly.
 */
void Assembly::firstPass(){

    int section = 0;
    int locationCounter = 0;
    char *line = readLine();
    bool firstInLine = true;

    while(line && !endOfProgram){

        StringTokenizer *st = new StringTokenizer(line);
        bool endOfLine = false;
        firstInLine = true;

        while(!endOfLine){

            char *token = st->getNextToken();
            if(token == nullptr) break;
            if(st->wasLastTokenLabel()){
                if(!firstInLine) throw Error(4);
                if(symbolTable->findSymbol(token) == nullptr){
                    symbolTable->addSymbol(token, section, locationCounter, 'l');
                    firstInLine = false;
                    continue;
                }
                else throw Error(5);
            }

            int type = determineTypeOfToken(token);
            switch(type){

            case 1:
                endOfLine = true;
                break;
            case 2:
                while(true){
                    char *symbol = st->getNextToken();
                    if(symbol){
                        if(symbolTable->findSymbol(symbol) == nullptr) symbolTable->addSymbol(symbol, 0, 0, 'g');
                        else throw Error(5);
                    }
                    else break;
                }
                endOfLine = true;
                break;
            case 3:
                while(st->getNextToken()) locationCounter += 1;
                endOfLine = true;
                break;
            case 4:
                while(st->getNextToken()) locationCounter += 2;
                endOfLine = true;
                break;
            case 5:
                {
                    int numOfArgs = st->calculateNumberOfArguments();
                    locationCounter += 4 * (numOfArgs + 1);
                    endOfLine = true;
                    break;
                }

            case 6:
                {
                    char *x = st->getNextToken();
                    int arg = atoi(x);
                    int modulo = locationCounter % arg;
                    if( modulo != 0) locationCounter += arg - modulo;
                    delete x;
                    endOfLine = true;
                    break;
                }
            case 7:
                {
                    char *y = st->getNextToken();
                    if(st->getNextToken()) throw Error(12);
                    int arg = atoi(y);
                    locationCounter += arg;
                    delete y;
                    endOfLine = true;
                    break;
                }
            case 8:
                endOfProgram = true;
                endOfLine = true;
                break;
            case 9:
                if(symbolTable->findSymbol(token) == nullptr){
                    symbolTable->addSection(token);
                    token = nullptr;
                    locationCounter = 0;
                    section = symbolTable->getLastSectionID();
                    endOfLine = true;
                    break;
                } else throw Error(5);

            case 10:
                locationCounter += 4;
                if(token[0] == 'l' && token[1] == 'd' && token[2] == 'c') locationCounter += 4;
                endOfLine = true;
                break;
            default:
                throw Error(20);
                endOfLine = true;
                break;
            }
            delete token;
            firstInLine = false;
        }

        delete st;
        line = readLine();
    }
    if(inputFileStream.is_open()) inputFileStream.close();
}

/*
 * This method converts decimal number
 * to hexadecimal version.
 */
string Assembly::convertDecimalToHex(unsigned long long decimal, int b){

    string result = "";
    for(int i = 0; i < 2 * b; i++){
        unsigned long long tmp = decimal & 15;
        tmp += tmp >= 10 ? 55 : 48;
        result = (char)tmp + result;
        decimal >>= 4;
    }
    return result;
}

/* This method creates machine code for
 * given instruction.
 */
unsigned long long Assembly::createMachineCode(char* mnemonic, StringTokenizer *st, RelocationTable *rt, int pc){

    int instructionNo = 0;
    for(int i = 0; i < NUMBER_OF_MNEMONICS; i++)
        if(strcmp(mnemonic, mnemonics[i]) == 0){
            instructionNo = i;
            break;
        }

    int condCode = instructionNo % 7;
    if(condCode == 6) condCode = 7;
    unsigned long long machineCode = condCode;
    switch(instructionNo / 7){
    case 0:
        {   /* int instruction */
            machineCode <<= 9;
            char *interrupt = st->getNextToken();
            if(interrupt == nullptr) throw Error(6);
            int intNum = atoi(interrupt);
            if(intNum < 0 || intNum > 15) throw Error(8);
            machineCode += intNum;
            delete [] interrupt;
            if(st->getNextToken()) throw Error(7);
            return machineCode << 20;
        }
    case 1:
    case 2:
        {   /* add and sub instruction */
            machineCode <<= 1;
            machineCode += 1;
            machineCode <<= 4;
            machineCode += instructionNo / 7;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            machineCode <<= 5;
            int x = atoi(t + 1);
            if(x == 19) throw Error(16);
            machineCode += x;
            delete [] t;
            machineCode <<= 1;
            t = st->getNextToken();
            int type = determineTypeOfToken(t);
            if(type == REGISTER){
                machineCode <<= 5;
                x = atoi(t + 1);
                if(x == 19) throw Error(16);
                machineCode += x;
                machineCode <<= 13;
            }
            else {
                if(type == CONSTANT){
                    machineCode += 1;
                    machineCode <<= 18;
                    machineCode += atoi(t + 1);
                }else throw Error(15);
            }
            delete [] t;
            if(st->getNextToken()) throw Error(17);
            return machineCode;
        }
    case 3:
    case 4:
    case 5:
        {   /* mul, div and cmp instruction */
            machineCode <<= 1;
            machineCode += 1;
            machineCode <<= 4;
            machineCode += instructionNo / 7;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            machineCode <<= 5;
            int x = atoi(t + 1);
            if(x > 15) throw Error(16);
            machineCode += x;
            delete [] t;
            machineCode <<= 1;
            t = st->getNextToken();
            int type = determineTypeOfToken(t);
            if(type == REGISTER){
                machineCode <<= 5;
                x = atoi(t + 1);
                if(x > 15) throw Error(16);
                machineCode += x;
                machineCode <<= 13;
            }
            else {
                if(type == CONSTANT){
                    machineCode += 1;
                    machineCode <<= 18;
                    machineCode += atoi(t + 1);
                }else throw Error(15);
            }
            delete [] t;
            if(st->getNextToken()) throw Error(17);
            return machineCode;
        }

    case 6:
    case 7:
    case 8:
    case 9:
        {   /* and, or, not and test instruction */
            machineCode <<= 1;
            machineCode += 1;
            machineCode <<= 4;
            machineCode += instructionNo / 7;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            machineCode <<= 5;
            int x = atoi(t + 1);
            if(x == 16 || x == 17 || x == 19) throw Error(16);
            machineCode += x;
            delete [] t;
            t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            machineCode <<= 5;
            x = atoi(t + 1);
            if(x == 16 || x == 17 || x == 19) throw Error(16);
            machineCode += x;
            delete [] t;
            if(st->getNextToken()) throw Error(17);
            machineCode <<= 14;
            return machineCode;
        }
    case 10:
    case 11: /* ldr and str instructions */
        {
            machineCode <<= 5;
            machineCode += 10;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            machineCode <<= 5;
            int x = atoi(t + 1);
            delete [] t;
            t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER && !st->wasLastTokenLabel()) throw Error(15);
            if(st->wasLastTokenLabel()){
                machineCode += 16;
                machineCode <<= 5;
                machineCode += x;
                machineCode <<= 4;
                if(instructionNo / 7 == 10) machineCode += 1;
                machineCode <<= 10;
                if(st->getNextToken()) throw Error(15);
                Symbol *s = symbolTable->findSymbol(t);
                x = s->getOffset() - pc;
                delete [] t;
                x = x & 1023;
                machineCode += x;
                return machineCode;
            }else{
                int y = atoi(t + 1);
                if(y == 19) throw Error(16);
                delete [] t;
                machineCode += y;
                machineCode <<= 5;
                machineCode += x;
                t = st->getNextToken();
                if(determineTypeOfToken(t) != CONSTANT) throw Error(15);
                y = atoi(t + 1);
                if(y < 2 || y > 5) throw Error(15);
                if(x == 16 && y != 0) throw Error(15);
                machineCode <<= 3;
                machineCode += y;
                machineCode <<= 1;
                if(instructionNo / 7 == 10) machineCode += 1;
                machineCode <<= 10;
                delete [] t;
                t = st->getNextToken();
                if(determineTypeOfToken(t) != CONSTANT) throw Error(15);
                machineCode += atoi(t + 1);
                delete [] t;
                return machineCode;
            }
        }
    case 12: /* call instruction */
        {
            machineCode <<= 5;
            machineCode += 12;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER && symbolTable->findSymbol(t) == nullptr) throw Error(15);
            machineCode <<= 5;
            if(symbolTable->findSymbol(t) != nullptr){
                if(st->getNextToken()) throw Error(15);
                machineCode += 16;
                machineCode <<= 19;
                Symbol *s = symbolTable->findSymbol(t);
                if(s == nullptr) throw Error(17);
                int x = s->getOffset() - pc;
                x = x & 524287;
                machineCode += x;
            }else{
                int x = atoi(t + 1);
                machineCode += x;
                delete [] t;
                t = st->getNextToken();
                if(determineTypeOfToken(t) != CONSTANT) throw Error(15);
                x = atoi(t + 1);
                delete [] t;
                machineCode <<= 19;
                machineCode += x;
                if(st->getNextToken()) throw Error(15);
            }
            return machineCode;
        }
    case 13:
    case 14: /* in and out instructions */
        {
            machineCode <<= 5;
            machineCode += 13;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            int x = atoi(t + 1);
            delete [] t;
            machineCode <<= 4;
            machineCode += x;
            t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            x = atoi(t + 1);
            delete [] t;
            machineCode <<= 4;
            machineCode += x;
            machineCode <<= 1;
            if(instructionNo / 7 == 13) machineCode += 1;
            machineCode <<= 15;
            if(st->getNextToken()) throw Error(15);
            return machineCode;
        }
    case 15:
    case 16:
    case 17: /* mov, shr, shl instructions */
        {
            machineCode <<= 1;
            machineCode += 1;
            machineCode <<= 4;
            machineCode += 14;
            machineCode <<= 5;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            int x = atoi(t + 1);
            machineCode += x;
            machineCode <<= 5;
            delete [] t;
            t = st->getNextToken();
            if(instructionNo / 7 == 15){
                if(determineTypeOfToken(t) != REGISTER && determineTypeOfToken(t) != CONSTANT) throw Error(15);
                if(determineTypeOfToken(t) == REGISTER){
                    machineCode += atoi(t + 1);
                    delete [] t;
                    if(st->getNextToken()) throw Error(15);
                    machineCode <<= 14;
                    return machineCode;
                }
                if(determineTypeOfToken(t) == CONSTANT){
                    machineCode <<= 5;
                    machineCode += atoi(t + 1);
                    delete [] t;
                    if(st->getNextToken()) throw Error(15);
                    machineCode <<= 9;
                    return machineCode;
                }
            }
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            machineCode += atoi(t + 1);
            delete [] t;
            machineCode <<= 5;
            t = st->getNextToken();
            if(determineTypeOfToken(t) != CONSTANT) throw Error(15);
            machineCode += atoi(t + 1);
            delete [] t;
            if(st->getNextToken()) throw Error(15);
            machineCode <<= 1;
            if(instructionNo / 7 == 17) machineCode += 1;
            machineCode <<= 8;
            return machineCode;
        }
    case 18:
    case 19: /* ldch and ldcl instructions */
        {
            machineCode <<= 5;
            machineCode += 15;
            machineCode <<= 4;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            machineCode += atoi(t + 1);
            delete [] t;
            machineCode <<= 1;
            if(instructionNo / 7 == 18) machineCode += 1;
            machineCode <<= 19;
            t = st->getNextToken();
            if(determineTypeOfToken(t) != CONSTANT) throw Error(15);
            machineCode += atoi(t + 1);
            delete [] t;
            if(st->getNextToken()) throw Error(15);
            return machineCode;
        }
    case 20:
        {
            machineCode <<= 5;
            machineCode += 15;
            machineCode <<= 4;
            char *t = st->getNextToken();
            if(determineTypeOfToken(t) != REGISTER) throw Error(15);
            machineCode += atoi(t + 1);
            delete [] t;
            machineCode <<= 1;
            unsigned long long macode = machineCode;
            machineCode += 1;
            machineCode <<= 19;
            macode <<= 19;
            t = st->getNextToken();
            if(determineTypeOfToken(t) != CONSTANT && symbolTable->findSymbol(t) == nullptr) throw Error(15);
            long x = 0;
            long y = 0;
            if(determineTypeOfToken(t) == CONSTANT){
                x = atol(t + 1);
                y = x >> 16;
            }
            else{
                Symbol *s = symbolTable->findSymbol(t);
                if(s == nullptr) throw Error(18);
                if(s->getVisibility() == 'l'){
                    x = s->getOffset();
                    y = x >> 16;
                    rt->insertNewEntry(pc - 2, "R_16_high", s->getSection());
                    rt->insertNewEntry(pc + 2, "R_16_low", s->getSection());
                }else{
                    x = 0;
                    y = 0;
                    rt->insertNewEntry(pc - 1, "R_16_high", s->getSymbolNo());
                    rt->insertNewEntry(pc + 3, "R_16_low", s->getSymbolNo());
                }
            }
            x = x & 65535;
            machineCode += y & 65535;
            macode += x;
            delete [] t;
            if(st->getNextToken()) throw Error(15);
            return (machineCode << 32) | macode;
        }
    default:
        throw Error(19);
    }

    return 0;
}

/*
 * This method does the second pass
 * of the assembly, meaning it creates
 * machine code and reallocation entries.
 */
void Assembly::secondPass(){

    inputFileStream.open(inputFileName, fstream::in);

    endOfProgram = false;
    int section = 0;
    int locationCounter = 0;
    string machineCode = "";
    char *line = readLine();
    rTables = new RelocationTable*[symbolTable->getLastSectionID()];
    Symbol *s = nullptr;

    while(line && !endOfProgram){

        StringTokenizer *st = new StringTokenizer(line);
        bool endOfLine = false;

        while(!endOfLine){

            char *token = st->getNextToken();

            if(token == nullptr) break;

            if(st->wasLastTokenLabel()) {
                    delete token;
                    token = nullptr;
                    continue;
            }
            int type = determineTypeOfToken(token);
            switch(type){

            case 1: /* .public */
                {
                    char *s = st->getNextToken();
                    while(s){
                        Symbol *symbol = symbolTable->findSymbol(s);
                        if(symbol == nullptr) throw Error(9);
                        else symbol->setVisibility('g');
                        s = st->getNextToken();
                    }
                    endOfLine = true;
                    break;
                }
            case 2: /* .extern */
                endOfLine = true;
                break;
            case 3: /* .char */
                {
                    char *t = st->getNextToken();
                    while(t){
                        if(strlen(t) > 1) throw Error(10);
                        if((t[0] >= '0' && t[0] <= '9') || (t[0] >= 'a' && t[0] <= 'z') || (t[0] >= 'A' && t[0] <= 'Z')){
                            locationCounter += 1;
                            machineCode = machineCode + convertDecimalToHex((unsigned long)t[0], 1);
                        }else throw Error(11);
                        t = st->getNextToken();
                    }
                    endOfLine = true;
                    break;
                }
            case 4: /* .word */
                {
                    char *t = st->getNextToken();
                    while(t){
                        int arg = atoi(t);
                        locationCounter += 2;
                        ostringstream oss;
                        oss << arg;
                        string s = oss.str();
                        if(s.length() == 1) s = "000" + s;
                        else{
                            if(s.length() == 2) s = "00" + s;
                            else{
                                if(s.length() == 3) s = "0" + s;
                            }
                        }
                        machineCode = machineCode + s[2] + s[3] + s[0] + s[1];
                        t = st->getNextToken();
                    }
                    endOfLine = true;
                    break;
                }
            case 5: /* .long */
                {
                    char *t = st->getNextToken();
                    while(t){
                        string code = "";
                        if(t == nullptr) throw Error(15);
                        if(determineTypeOfToken(t) != CONSTANT && symbolTable->findSymbol(t) == nullptr) throw Error(18);
                        if(determineTypeOfToken(t) == CONSTANT){
                            long x = atol(t + 1);
                            code += convertDecimalToHex(x, 4);
                            delete [] t;
                            t = st->getNextToken();
                            if(t != nullptr && (t[0] == '+' || t[0] == '-')) throw Error(21);
                        }else{
                            Symbol *s = symbolTable->findSymbol(t);
                            delete [] t;
                            t = st->getNextToken();
                            if(t != nullptr && (t[0] == '-' || t[0] == '+')){
                                bool addition = t[0] == '+' ? true : false;
                                delete [] t;
                                t = st->getNextToken();
                                if(t == nullptr || symbolTable->findSymbol(t) == nullptr) throw Error(18);
                                Symbol *s2 = symbolTable->findSymbol(t);
                                delete [] t;
                                if(addition){
                                    if(s->getVisibility() == 'l' && s2->getVisibility() == 'l'){
                                        code += convertDecimalToHex(s->getOffset() + s2->getOffset(), 4);
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSection());
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s2->getSection());
                                    }
                                    if(s->getVisibility() == 'g' && s2->getVisibility() == 'g'){
                                        code += "00000000";
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSymbolNo());
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s2->getSymbolNo());
                                    }
                                    if(s->getVisibility() == 'l' && s2->getVisibility() == 'g'){
                                        code += convertDecimalToHex(s->getOffset(), 4);
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSection());
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s2->getSymbolNo());
                                    }
                                    if(s->getVisibility() == 'g' && s2->getVisibility() == 'l'){
                                        code += convertDecimalToHex(s2->getOffset(), 4);
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s2->getSection());
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSymbolNo());
                                    }
                                }else{
                                    if(s->getVisibility() == 'l' && s2->getVisibility() == 'l'){
                                        code += convertDecimalToHex(s->getOffset() - s2->getOffset(), 4);
                                    }
                                    if(s->getVisibility() == 'g' && s2->getVisibility() == 'g'){
                                        code += "00000000";
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSymbolNo());
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32_negative", s2->getSymbolNo());
                                    }
                                    if(s->getVisibility() == 'l' && s2->getVisibility() == 'g'){
                                        code += convertDecimalToHex(s->getOffset(), 4);
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSection());
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32_negative", s2->getSymbolNo());
                                    }
                                    if(s->getVisibility() == 'g' && s2->getVisibility() == 'l'){
                                        code += convertDecimalToHex(-s2->getOffset(), 4);
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32_negative", s2->getSection());
                                        rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSymbolNo());
                                    }
                                }
                            }else{
                                long x;
                                if(s->getVisibility() == 'l'){
                                    x = s->getOffset();
                                    rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSection());
                                    code += convertDecimalToHex(x, 4);
                                }else{
                                    rTables[section - 1]->insertNewEntry(locationCounter, "R_32", s->getSymbolNo());
                                    code += "00000000";
                                }
                            }
                            t = st->getNextToken();
                        }
                        machineCode += code[6];
                        machineCode += code[7];
                        machineCode += code[4];
                        machineCode += code[5];
                        machineCode += code[2];
                        machineCode += code[3];
                        machineCode += code[0];
                        machineCode += code[1];
                        locationCounter += 4;

                    }
                    endOfLine = true;
                    break;
                }
            case 6: /* .align */
                {
                    char *x = st->getNextToken();
                    int arg = atoi(x);
                    int modulo = locationCounter % arg;
                    if( modulo != 0){
                        locationCounter += arg - modulo;
                        for(int i = 0; i < arg - modulo; i++) machineCode += "00";
                    }
                    delete x;
                    endOfLine = true;
                    break;
                }
            case 7: /* .skip */
                {
                    char *y = st->getNextToken();
                    int arg = atoi(y);
                    locationCounter += arg;
                    for(int i = 0; i < arg; i++) machineCode += "00";
                    delete y;
                    endOfLine = true;
                    break;
                }
            case 8: /* .end */
                writeMachineCodeToFile(machineCode, s->getName());
                endOfProgram = true;
                endOfLine = true;
                break;
            case 9:
                {
                    if(section != 0) writeMachineCodeToFile(machineCode, s->getName());
                    locationCounter = 0;
                    machineCode = "";
                    s = symbolTable->findSymbol(token);
                    section = s->getSymbolNo();
                    rTables[section - 1] = new RelocationTable(token);
                    token = nullptr;
                    endOfLine = true;
                    break;
                }
            case 10: /* instructions */
                {
                    locationCounter += 4;
                    endOfLine = true;
                    unsigned long long x = createMachineCode(token, st, rTables[section - 1], locationCounter);
                    if(token[0] == 'l' && token[1] == 'd' && token[2] == 'c' && strlen(token) == 5){
                        locationCounter += 4;
                        machineCode += convertDecimalToHex(x, 8);
                    }else{
                        machineCode += convertDecimalToHex(x, 4);
                    }
                    break;
                }
            default:
                throw Error(20);
                endOfLine = true;
                break;
            }
            delete token;
        }

        delete st;
        line = readLine();
    }
    for(int i = 0; i < symbolTable->getLastSectionID(); i++) rTables[i]->writeTableToFile(outputFileStream);
    symbolTable->saveToFile(outputFileStream);
}

/*
 * This method formats and writes machine code
 * to a given file.
 */
 void Assembly::writeMachineCodeToFile(string code, char *section){

    outputFileStream << endl << endl << '#';
    for(unsigned int i = 0; i < strlen(section); i++) outputFileStream << section[i];
    outputFileStream << endl;
    for(unsigned int i = 1, j = 0; i <= code.length(); i++){
        outputFileStream << code[i - 1];
        j++;
        if(i % 2 == 0 && i != 1){
            outputFileStream << " ";
            j++;
        }
        if(j == 24){
            outputFileStream << endl;
            j = 0;
        }
    }
 }
