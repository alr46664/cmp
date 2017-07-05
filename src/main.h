
#ifndef MAIN_H
#define MAIN_H

// C INCLUDES
// #include <cstdio> //freopen

// C++ INCLUDES
#include <iostream> // cin, getline
#include <string> // string
#include <exception> // exception class

// custom includes
#include "classes/parser.h" // Parser class
#include "classes/sintatical.h" // Sintatical class
#include "classes/codegen.h" // codegen class
#include "classes/node.h" // Node class
#include "classes/error.h" // error exception class
#include "classes/utility.h" // Utility class

// lexer prototype
int yylex(void);

#endif // MAIN_H
