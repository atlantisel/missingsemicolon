#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


using namespace std;

int main() {
    string fileName = "Food List.csv";
    const int fieldCount = 4;
    ifstream menu(fileName);
    
    string line;
    while (getline(menu, line)) {
        string field;
        string fields[fieldCount];
        stringstream ss(line);
        for (int i = 0; getline(ss, field, ','); i++) {
            fields[i] = field;
        }
        for (string str : fields) {
            cout << str << " | ";
        }
        
    } 
    
    return 0; 
}