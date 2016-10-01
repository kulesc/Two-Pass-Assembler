#ifndef ERROR
#define ERROR

#include <string>

using namespace std;



class Error{

public:

    Error(int);

    ~Error();

    string toString();

private:

    static const int NUMBER_OF_MESSAGES = 23;

    static string errorMessages[NUMBER_OF_MESSAGES];

    int messageCode;
};

#endif
