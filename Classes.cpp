#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "Classes.h"

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

string Item::get_stallName() {
    return stallName;
}

string Item::get_itemName() {
    return itemName;
}

float Item::get_price() {
    return price;
}

bool Item::is_deliverable() {
    return deliverable;
}

string Item::get_dishType() {
    return dishType;
}

string Item::get_meatType() {
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

void Sentence::key(string str) {
    keywords.push_back(str);
}

string Sentence::get_string() {
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
    return sentence_string == str;
}