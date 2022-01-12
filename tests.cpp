#include <iostream>
using namespace std;

#include "library2.h"

int main() {
    cout << "Tests: " << endl;

    int k = 5;
    int scale = 10;

    void* DS = nullptr;
    DS = Init(k, scale);

    //pID, gID, score
    AddPlayer(DS, 1, 1, 2);
    AddPlayer(DS, 2, 2, 2);

    
    


    Print(DS);


    Quit(&DS);

    return 0;
}

