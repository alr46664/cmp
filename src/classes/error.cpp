
#include <iostream> // cin, getline
#include <string> // string
#include "error.h"

using namespace std;

void Error::print(){
    cerr << "\nERROR: (Line \"" << line << "\") - " << msg << endl << endl;
    exit(code);
}
