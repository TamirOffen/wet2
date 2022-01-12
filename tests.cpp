#include <iostream>
using namespace std;

#include "library2.h"

int main() {
    cout << "Tests: " << endl;

    int k = 5;
    int scale = 100;

    void* DS = nullptr;
    DS = Init(k, scale);
    

    Print(DS);


    Quit(&DS);

    return 0;
}

