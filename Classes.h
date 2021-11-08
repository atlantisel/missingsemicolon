#ifndef Classes_h
#define Classes_h

#include <string>

class Item {
public:
    Item(std::string[]);
    std::string getStallName();
    std::string getItemName();
    double getPrice(); 
    bool getIsDeliverable(); 
    

private:
    std::string stallName;
    std::string itemName;
    double price;
    bool deliverable;
};

#endif