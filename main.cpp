#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Classes.h"

using namespace std;

int main() {
    ifstream menu("Food List.csv");
    vector<Item> items;

    string line;
    getline(menu, line);
    while (getline(menu, line)) { 
        string field;
        string fields[4];
        stringstream ss(line);
        if (line.back() == '\r') line.pop_back(); 
        for (int i = 0; getline(ss, field, ','); i++) {
            fields[i] = field;
        }
        items.push_back(Item(fields));
    }
    
    for (Item item : items) {
        cout << item.display() << endl;
    }
    
    return 0;
}