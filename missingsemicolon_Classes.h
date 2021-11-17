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
    bool        recommended();

    bool empty();
    void clear();

    void display();
    void list   (int);

    bool       operator==(Item);
    const bool operator==(Item) const;

private:
    struct {
        std::string stallName;
        std::string itemName;
        float       price;
        bool        deliverable;
        std::string dishType;
        std::string meatType;
        bool        recommended;
    } priv;
};

std::vector<Item> merge(std::vector<Item>, std::vector<Item>);
std::vector<Item> merge(std::vector<std::vector<Item>>);

class Tag {
public:
    Tag();
    Tag(std::string, std::vector<std::string>);
    Tag(std::string, std::vector<std::string>, std::vector<std::string>);

    std::string              tag();
    std::vector<std::string> checks();
    std::vector<std::string> keywords();

private:
    struct {
        std::string              tag;
        std::vector<std::string> checks;
        std::vector<std::string> keywords;
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
    
    std::vector<Tag> parse(std::vector<Tag>);

    std::vector<std::string> operator()();
    std::vector<std::string>::iterator begin();
    std::vector<std::string>::iterator end();

private:
    std::string sentence_string;
    std::vector<std::string> sentence;
};

#endif