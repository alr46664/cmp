
#include <iostream> // cin, getline
#include <string> // string
#include "error.h"

using namespace std;

const int Error::getCode(){
    return code;
}

const string Error::what(){
    return string("\nERROR: (Line \"") + to_string(line) + "\") - " + token + " - " + msg + "\n";
}
