#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "Classes.h"

using namespace std;

Item::Item(string fields[]) {
    stallName = fields[0];
    itemName = fields[1];
    price = stod(fields[2]);
    deliverable = fields[3] == "yes" ? true : false;
}

string Item::getStallName() {
    return stallName;
}

string Item::getItemName() {
    return itemName;
}

double Item::getPrice() {
    return price;
}

bool Item::isDeliverable() {
    return deliverable;
}

string Item::display() {
    stringstream ss; 
    ss << fixed << setprecision(2) << price;
    string str = "Stall: " + stallName + "\n"
               + "Item:  " + itemName  + "\n"
               + "Price: " + ss.str()  + "\n"
               + (deliverable ? "" : "Not ") + "Deliverable\n";
    return str;
}

Sentence::Sentence() {}

void Sentence::read(string str) {
    istringstream iss(str);
    while (iss) {
        string word;
        iss >> word;
        sentence.push_back(word);
    }

    // Remove trailing \n
    sentence.pop_back();

    // Clean punctuation
    for (string& word : sentence)
        while (!isalnum(word.back()))
            word.pop_back();
}

void Sentence::print() {
    for (string str : sentence)
        cout << str << endl;
}