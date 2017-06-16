#include <iostream>
#include <string>
#include <initializer_list>

class Utility {
public:
    static bool check(std::string s, std::initializer_list<char*> cond){
        bool res = (cond.begin() == cond.end());
        for (std::initializer_list<char*>::iterator it = cond.begin(); it != cond.end(); it++){
            res |= (s == *it);
            if (res) break;
        }
        return res;
    }
};
