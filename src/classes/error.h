#ifndef ERROR_H
#define ERROR_H

#include <string>

// define error codes
#define ERR_TOKEN_UNDEF 1
#define ERR_ID 2
#define ERR_KEY 3
#define ERR_DEC 4
#define ERR_SYM 5

class Error {
private:
    std::string msg;
    int line;
    int code;
public:
    //   MEMBER
    Error(std::string m, int l, int c): msg(m), line(l), code(c) {};

    void print();
};


#endif // ERROR_H
