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

    std::string stallName();
    std::string itemName();
    float       price();
    bool        deliverable();
    std::string dishType();
    std::string meatType();

    void display();
    void list   (int);

private:
    struct {
        std::string stallName;
        std::string itemName;
        float       price;
        bool        deliverable;
        std::string dishType;
        std::string meatType;
    } priv;
};

class Label {
public:
    Label();
    Label(std::string, std::vector<std::string>);
    Label(std::string, std::vector<std::string>, std::vector<std::string>);

    std::string              keyword();
    std::vector<std::string> tags();
    std::vector<std::string> checks();

private:
    struct {
        std::string              keyword;
        std::vector<std::string> tags;
        std::vector<std::string> checks;
    } priv;
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
    bool anyof   (std::vector<std::string>);
    bool search  (std::string);
    
    std::vector<std::string> parse(std::vector<Label>);

    std::vector<std::string> operator()();
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

private:
    std::string sentence_string;
    std::vector<std::string> sentence;
};

#endif