#include <fstream>
#include <functional>
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
    getline(ss, priv.stallName, ',');
    getline(ss, priv.itemName, ',');
    getline(ss, temp, ',');
    priv.price = stod(temp);
    getline(ss, temp, ',');
    priv.deliverable = temp == "yes" ? true : false;
    getline(ss, priv.dishType, ',');
    getline(ss, priv.meatType, ',');
}

string Item::stallName() {
    return priv.stallName;
}

string Item::itemName() {
    return priv.itemName;
}

float Item::price() {
    return priv.price;
}

bool Item::deliverable() {
    return priv.deliverable;
}

string Item::dishType() {
    return priv.dishType;
}

string Item::meatType() {
    return priv.meatType;
}

void Item::display() {
    cout << fixed << setprecision(2)
         << "Item:  " << priv.itemName  << endl
         << "Price: " << priv.price     << endl
         << "Stall: " << priv.stallName << endl
         << "Type:  " << priv.dishType  << endl
         << "Meat:  " << priv.meatType  << endl
         << "Delivery" << (priv.deliverable ? " " : " not ") << "available" << endl
         << endl;
}

void Item::list(int i) {
    stringstream p;
    p << fixed << " " << setprecision(2) << priv.price;
    cout << right                 << setw(3)  << i << ". "
         << left  << setfill('.') << setw(45) << (priv.itemName + " ")
         << right                 << setw(6)  << p.str() << endl
                  << setfill(' ') << setw(6)  << "(" << priv.stallName
         << " - " << priv.dishType << ")" << endl;
}

Label::Label() {}

Label::Label(string _keyword, vector<string> _tags) {
    priv.keyword = _keyword;
    priv.tags = _tags;
}

Label::Label(string _keyword, vector<string> _tags, vector<string> _checks) {
    priv.keyword = _keyword;
    priv.tags = _tags;
    priv.checks = _checks;
}

string Label::keyword() {
    return priv.keyword;
}

vector<string> Label::tags() {
    return priv.tags;
}

vector<string> Label::checks() {
    return priv.checks;
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
        word.erase(find_if(word.begin(), word.end(),
                           [](char c) {
                               return !isalnum(c);
                           }));
    sentence.erase(remove_if(sentence.begin(), sentence.end(), mem_fn(&string::empty)), sentence.end());
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

bool Sentence::anyof(vector<string> words) {
    for (string word : words) {
        if (util::contains(word, sentence))
            return true;
    }
    return false;
}

// locate phrase in sentence; distinct from "contiguous" util::contains()
bool Sentence::search(string target) {
    if (!util::hasspaces(target)) {
        return this->contains(target);
    } else {
        vector<string> phrase = Sentence(target)();
        for (string word : phrase) {
            if (!util::contains(word, sentence))
                return false;
        }
    }
    return true;
}

// return tags associated with string
vector<string> Sentence::parse(vector<Label> labels) {
    vector<string> res;
    for (Label label : labels) {
        string keyword = label.keyword();
        if (this->search(keyword))
            for (string tag : label.tags())
                res.push_back(tag);
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