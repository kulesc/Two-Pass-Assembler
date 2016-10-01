#include "Error.h"

/*
 * Instance of this class is initialized
 * with number of error message.
 */
Error::Error(int mCode){

    messageCode = mCode;
}

Error::~Error(){

}

/*
 * This function prints the error message.
 */
string Error::toString(){

    return "Error: " + errorMessages[messageCode];
}

/*
 * This array contains different error
 * messages.
 */
string Error::errorMessages[] = {
    "Error opening input file.",
    "Error creating output file.",
    "Unknown directive.",
    "Only one instruction per line is permitted.",
    "Label must be first in line.",
    "Multiple definitions of the same symbol.",
    "Int instruction must have argument.",
    "Int instruction can have only one argument.",
    "Int number can only be between 0 and 15.",
    "Trying to export undefined symbol.",
    "Directive .char takes only 1 byte as an argument.",
    "Invalid argument for .char directive.",
    "Too many arguments for .skip directive.",
    "Invalid register number.",
    "Invalid constant.",
    "Invalid syntax in instruction.",
    "Register used in instruction is not allowed.",
    "Only local labels allowed in call instruction.",
    "Undefined symbol.",
    "Unknown instruction.",
    "Incorrect syntax",
    ".long directive must contain at least one argument.",
    "Couldn't parse instruction.",
};
