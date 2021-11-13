#ifndef Classes_h
#define Classes_h

#include <functional>   // function
#include <string>
#include <utility>
#include <vector>

class Item {
public:
    Item();
    Item(std::string);

    std::string _stallName();
    std::string _itemName();
    float       _price();
    bool        _deliverable();
    std::string _dishType();
    std::string _meatType();

    void display();
    void list(int);

private:
    std::string stallName   = "";
    std::string itemName    = "";
    float       price       = 0;
    bool        deliverable = false;
    std::string dishType    = "";
    std::string meatType    = "";
};

class Sentence {
public:
    Sentence();
    Sentence(std::string);

    void clear ();
    void read  (std::string);

    std::string str();

    void println();
    void println(std::function<bool(std::string)>);

    bool is      (std::string);
    bool contains(std::string);
    bool contains(std::vector<std::string>);
    bool search  (std::string);
    
    std::vector<std::string> parse(std::vector<std::string>);

    std::vector<std::string> operator()();
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

private:
    std::string sentence_string;
    std::vector<std::string> sentence;
};

#endif