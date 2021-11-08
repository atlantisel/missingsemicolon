#ifndef Classes_h
#define Classes_h

#include <iostream>
#include <string>
#include <vector>

class Item {
public:
    Item(std::string[]);
    std::string getStallName();
    std::string getItemName();
    double getPrice();
    bool isDeliverable();
    std::string display();

private:
    std::string stallName;
    std::string itemName;
    double price;
    bool deliverable;
};

class Sentence {
public:
    Sentence();
    void read(std::string);
    void print();

private:
    std::vector<std::string> sentence;
    std::vector<std::string> keywords;
};

#endif