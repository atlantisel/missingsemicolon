#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include "Classes.h"

using namespace std;

Item::Item(string fields[]) {
    stallName = fields[0];
    itemName = fields[1];
    price = stod(fields[2]);
    deliverable = fields[3] == "yes" ? true : false;
}

string Item::getStallName(){
    return stallName;
}

string Item::getItemName(){
    return itemName;
}

double Item::getPrice(){
    return price;
}

bool Item::isDeliverable(){
    return deliverable;
}

string Item::display(){
    stringstream ss; 
    ss << fixed << setprecision(2) << price;
    string str = "Stall: " + stallName + "\n"
               + "Item:  " + itemName  + "\n"
               + "Price: " + ss.str()  + "\n"
               + (deliverable ? "" : "Not ") + "Deliverable\n";
    return str;
}
