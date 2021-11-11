#ifndef Classes_h
#define Classes_h

#include <string>
#include <utility>
#include <vector>

class Item {
public:
    Item(std::string);

    std::string _stallName();
    std::string _itemName();
    float _price();
    bool _deliverable();
    std::string _dishType();
    std::string _meatType();

    std::string display();

private:
    std::string stallName;
    std::string itemName;
    float price;
    bool deliverable;
    std::string dishType;
    std::string meatType;
};

class Sentence {
public:
    Sentence();
    void read(std::string);
    void addKey(std::string);

    std::string str();
    std::vector<std::string> get();

    void printnl();

    bool is(std::string);

private:
    std::string sentence_string;
    std::vector<std::string> sentence;
    std::vector<std::string> keywords;
};

#endif