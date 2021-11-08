#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Classes.h"

using namespace std;

int main() {
    string fileName = "Food List.csv";
    const int fieldCount = 4;
    
    ifstream menu(fileName);
    string line;
    getline(menu, line);
    cout << line;
    cout << "why" ; 

    return 0;
} // wait ok lets start from the beginning
// if i were you i would do the same 
// i- you mean liveshare?ok yeah sure