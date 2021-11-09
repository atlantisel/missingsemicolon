#ifndef Classes_h
#define Classes_h

#include <string>
#include <utility>
#include <vector>

class Item {
public:
    Item(std::string);

    std::string get_stallName();
    std::string get_itemName();
    float get_price();
    bool is_deliverable();
    std::string get_dishType();
    std::string get_meatType();

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
    void key(std::string);

    std::string get_string();
    std::vector<std::string> get();

    void printnl();

    bool is(std::string);

private:
    std::string sentence_string;
    std::vector<std::string> sentence;
    std::vector<std::string> keywords;
};

#endif