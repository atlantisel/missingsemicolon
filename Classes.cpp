#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "Classes.h"
#include "Functions.h"

using namespace std;

Item::Item(string line) {
    string temp;
    stringstream ss(line);
    getline(ss, stallName, ',');
    getline(ss, itemName, ',');
    getline(ss, temp, ',');
    price = stod(temp);
    getline(ss, temp, ',');
    deliverable = temp == "yes" ? true : false;
    getline(ss, dishType, ',');
    getline(ss, meatType, ',');
}

string Item::_stallName() {
    return stallName;
}

string Item::_itemName() {
    return itemName;
}

float Item::_price() {
    return price;
}

bool Item::_deliverable() {
    return deliverable;
}

string Item::_dishType() {
    return dishType;
}

string Item::_meatType() {
    return meatType;
}

string Item::display() {
    stringstream ss; 
    ss << fixed << setprecision(2) << price;
    string str = "Stall: " + stallName + "\n"
               + "Item:  " + itemName  + "\n"
               + "Price: " + ss.str()  + "\n"
               + (deliverable ? "" : "Not ") + "Deliverable\n"
               + "Type:  " + dishType + "\n"
               + "Meat:  " + meatType + "\n";
    return str;
}

Sentence::Sentence() {}

void Sentence::read(string str) {
    sentence_string = str;
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

void Sentence::addKey(string str) {
    keywords.push_back(str);
}

string Sentence::str() {
    return sentence_string;
}

vector<string> Sentence::get() {
    return sentence;
}

void Sentence::printnl() {
    for (string str : sentence)
        cout << str << endl;
}

bool Sentence::is(string str) {
    return ciequal(str, sentence_string);
}