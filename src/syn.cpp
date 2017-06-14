#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>
#include "classes/node.h"

using namespace std;

int main(int argc, char **argv) {
    Node program("program");
    Node n1("n1_test"), n2("n2_test");
    Node n("test_n", {n1, n2});
    // n.add(n1).add(n2);
    cout << n << endl;
    return 0;
}
