#ifndef DEFINED_H
#define DEFINED_H

// C++ INCLUDES
#include <iostream> // cin, getline, endl
#include <string> // string

#define DEFINED_VARIABLE "variable"
#define DEFINED_FUNCTION "function"

class Defined {
private:
    // store the scope of the Defined
    std::string name, type;
    int arg_size;
public:

    Defined(){
        name = type = "";
        arg_size = 0;
    }

    Defined (std::string n, std::string t, int s = 0): Defined() {
        setName(n);
        setType(t);
        setArgSize(s);
    }

    void setName(std::string n);
    void setType(std::string t);
    void setArgSize(int s);

    std::string getName() const;
    std::string getType() const;
    int getArgSize() const;
};


#endif // DEFINED_H
