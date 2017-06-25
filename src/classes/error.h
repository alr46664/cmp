#ifndef ERROR_H
#define ERROR_H

#include <string>
#include <exception>

// define error codes
#define ERR_TOKEN_UNDEF 1
#define ERR_ID 2
#define ERR_KEY 3
#define ERR_DEC 4
#define ERR_SYM 5
#define ERR_UNK 6

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
