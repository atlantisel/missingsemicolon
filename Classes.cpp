#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include "Classes.h"
#include "Functions.h"

using namespace std;

Item::Item() {};

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

void Item::display() {
    cout << fixed << setprecision(2)
         << "Item:  " << itemName  << endl
         << "Price: " << price     << endl
         << "Stall: " << stallName << endl
         << "Type:  " << dishType  << endl
         << "Meat:  " << meatType  << endl
         << "Delivery" << (deliverable ? " " : " not ") << "available" << endl
         << endl;
}

void Item::list(int i) {
    stringstream p;
    p << fixed << " " << setprecision(2) << price;
    cout << right                 << setw(3)  << i << ". "
         << left  << setfill('.') << setw(45) << (itemName + " ")
         << right                 << setw(6)  << p.str() << endl
                  << setfill(' ') << setw(6)  << "(" << stallName
         << " - " << dishType << ")" << endl;
}

Sentence::Sentence() {}

Sentence::Sentence(string str) {
    this->read(str);
}

void Sentence::clear() {
    sentence.clear();
}

void Sentence::read(string str) {
    sentence.clear();
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

string Sentence::str() {
    return sentence_string;
}

void Sentence::println() {
    for (string str : sentence)
        cout << str << endl;
}

void Sentence::println(function<bool(string)> f) {
    int size = 1;
    for (string str : sentence) {
        if (str.size() > size)
            size = str.size();
    }
    for (string str : sentence)
        cout << left << setw(size + 1) << str << boolalpha << f(str) << endl;
}

bool Sentence::is(string str) {
    return util::iequals(str, sentence_string);
}

bool Sentence::contains(string str) {
    return util::contains(str, sentence);
}

bool Sentence::contains(vector<string> words) {
    for (string word : words) {
        if (util::contains(word, sentence))
            return true;
    }
    return false;
}

bool Sentence::search(string str) {
    vector<string> target = Sentence(str)();
    if (target.size() == 1) {
        return this->contains(target);
    } else {
        for (string word : target) {
            if (!util::contains(word, sentence)) {}
                return false;
        }
    }
    return true;
}

vector<string> Sentence::parse(vector<string> keywords) {
    vector<string> res;
    for (string word : sentence) {
        if (util::contains(word, keywords))
            res.push_back(word);
    }
    return res;
}

vector<string> Sentence::operator()() {
    return sentence;
}

vector<string>::iterator Sentence::begin() {
    return sentence.begin();
}

vector<string>::iterator Sentence::end() {
    return sentence.end();
}