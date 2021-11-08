#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Classes.h"
using namespace std;

struct {
    vector<string> mentionedStalls;
} session;

bool conversation();
string prompt(string);
vector<Item> items;

int main() {    
    try {
        // Initialise items
        string fileName = "Food List.csv";

        ifstream menu(fileName);
        if (menu.is_open()) {
            cout << fileName << " opened successfully." << endl;
        } else {
            throw runtime_error("\"" + fileName + "\" cannot be opened.");
        }
        
        string line;
        getline(menu, line);
        while (getline(menu, line)) { 
            string field;
            string fields[4];
            stringstream ss(line);
            if (line.back() == '\r') line.pop_back(); 
            for (int i = 0; getline(ss, field, ','); i++)
                fields[i] = field;
            items.push_back(Item(fields));
        }
        
        // Conversation
        Sentence sentence;
        sentence.read(prompt("Hi, I'm Ken. How can I help you out today?"));
        sentence.print();
    }

    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }
    
    return 0;
}

string prompt(string str) {
    cout << str << endl;
    cout << ">> ";
    string response;
    getline(cin, response);
    return response;
}