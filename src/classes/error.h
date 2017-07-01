#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <exception>

// define error codes
#define ERR_TOKEN_UNDEF 1
#define ERR_UNK 2

#define ERR_ID 3
#define ERR_KEY 4
#define ERR_DEC 5
#define ERR_SYM 6

#define ERR_LEX 7
#define ERR_SIN 8
#define ERR_SEM 9

class Error: public std::exception {
private:
    std::string msg, token;
    int line;
    int code;
public:
    //   MEMBER
    Error(std::string m, std::string t, int l, int c): msg(m), token(t), line(l), code(c) {};

    const int getCode();
    const std::string what();
};


#endif // ERROR_H
