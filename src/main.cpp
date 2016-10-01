#include <iostream>
#include "StringTokenizer.h"
#include <cstring>
#include "Assembly.h"
#include "Error.h"
#include <sstream>

using namespace std;

int main(int argc, char* argv[]){

    try{
        Assembly *a = new Assembly(argv[1], argv[2]);
        a->firstPass();
        a->secondPass();
        delete a;
    }catch(Error &e){
        cout << e.toString() << endl;
    }

    return 0;
}
