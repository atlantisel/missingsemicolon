#include <algorithm>    // find_if
#include <exception>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "Classes.h"
#include "Functions.h"

using namespace std;

bool   conversation();
string prompt      (string = "");
void   menu        (vector<Item>);
bool   test        (int, char**);

namespace init {
    ifstream chk_openFile(string);
    void     generateList(string, vector<string>&);
};

struct Items {
    // Initialise menu
    Items() {
        ifstream file(init::chk_openFile("Food List.csv"));
        string line;
        getline(file, line);
        while (getline(file, line)) {
            if (line.back() == '\r') line.pop_back();
            items.push_back(Item(line));
        }
    }

    enum field {stallName, dishType, meatType, price, deliverable};

    vector<Item> filter(field f, string str = "") {
        vector<Item> res;
        switch (f) {
        case stallName:
            for (Item item : items) 
                if (Sentence(item.stallName()).search(str))
                    res.push_back(item);
            break;

        case dishType:
            for (Item item : items) 
                if (Sentence(item.dishType()).search(str))
                    res.push_back(item);
            break;

        case meatType:
            for (Item item : items) 
                if (Sentence(item.meatType()).search(str))
                    res.push_back(item);
            break;                    

        case price:
            for (Item item : items)
                if (item.price() <= stof(str))
                    res.push_back(item);
            break;

        case deliverable:
            for (Item item : items)
                if (item.deliverable())
                    res.push_back(item);
            break;                    
        }
        return res;
    }

    size_t       size()               { return items.size();  }
    Item         operator[](size_t i) { return items[i];      }
    vector<Item> operator()()         { return items;         }
    vector<Item>::iterator begin()    { return items.begin(); }
    vector<Item>::iterator end()      { return items.end();   }

private:
    vector<Item> items;
} items;

struct Order {
    Item item;
    int  quantity;

    void list(int i) {
        cout << fixed << setprecision(2)
             << right << setw(3) << i << ". "
             << item.itemName() << endl
             << setw(7) << quantity << " x "
             << item.price() << " = "
             << (quantity * item.price()) << endl;
    }
};

namespace lists {
    vector<string> stallNames;
    vector<string> dishTypes;
    vector<string> meatTypes;
    vector<Tag>    tags;
};

namespace session {
    static bool   start = true;
    Order         order;
    vector<Order> orders;
    vector<Item>  results;
};

namespace keys {
    const vector<string> yes = {
        "affirmative", "alright", "correct", "okay", "pos", "positive", "right", "sure", "true", "y", "ya",  "yah","ye", "yeah", "yeh", "yep", "yes", "yup"};
    const vector<string> no = {
        "n", "nah", "neg", "negative", "neh", "nein", "no", "nop", "nope", "not", "nu"};
    const vector<string> cancel = {
        "cancel", "close", "escape", "exit", "leave", "stop", "quit"};
    const vector<string> previous = {
        "back", "backward", "backwards", "before", "earlier", "former", "left", "prev", "previous"};
    const vector<string> next = {
        "after", "forward", "forwards", "later", "next", "right"};
    const vector<string> page = {
        "go", "goto", "jump", "move", "navigate", "page", "pg", "to"};
};

Sentence sentence;

int main(int argc, char** argv) {
    try {
        // Generate lists
        for (Item item : items) {
            init::generateList(item.stallName(), lists::stallNames);
            init::generateList(item.dishType(),  lists::dishTypes);
            init::generateList(item.meatType(),  lists::meatTypes);
        }
        sort(lists::stallNames.begin(), lists::stallNames.end());
        sort(lists::dishTypes.begin(),  lists::dishTypes.end());
        sort(lists::meatTypes.begin(),  lists::meatTypes.end());

        // Initialise keywords and tags
        ifstream file(init::chk_openFile("tags.csv"));
        string line;
        getline(file, line);
        while (getline(file, line)) {
            string tag, checkStr, check, keywordStr, keyword;
            vector<string> checks, keywords;

            if (line.back() == '\r') line.pop_back(); 

            stringstream ss(line);
            getline(ss, tag,        ',');
            getline(ss, checkStr,   ',');
            getline(ss, keywordStr, ',');

            ss = stringstream(keywordStr);
            while (getline(ss, keyword, ';'))
                keywords.push_back(keyword);

            if (checkStr.empty()) {
                lists::tags.push_back(Tag(tag, keywords));
            } else {
                ss = stringstream(checkStr);
                while (ss >> check)
                    checks.push_back(check);
                lists::tags.push_back(Tag(tag, checks, keywords));
            }
        }
        
        if (test(argc, argv)) return 0;

        while (conversation());
    }

    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }
    
    return 0;
}

bool conversation() {
    if (session::start) {
        // Initial state
        sentence.read(prompt("Hi, I'm Ken. How can I help you out today?"));
        session::start = false;
    } else {
        // Subsequent state
        sentence.read(prompt("Is there anything else I can help you with?"));
    }

    //  pseudocode
/*  
    - "order"
            • replies what to order if there's no food specified
            • show's the menu and recommendation
    - picks food with ID(implement some type of ID system) or food name
            • adds food to cart
            • asks user if there's anything else to add (becomes a loop unless proceed to checkout/quit)
    - Chooses checkout
            • Shows order summary
            • option to add or remove foods from cart
            • option to confirm order
    - Confirms order
            • If there's food that's deliverable, asks if user wants it delivered.
                • Prompt delivery address
            • Sends receipt
*/

    vector<Tag> tags = sentence.parse(lists::tags);
    
    // includes dependent tag check processing
    const auto tagged = [&tags](string str) -> bool {
        auto f = [&tags](string str, auto&& tagged_ref) mutable -> bool {
            for (Tag tag : tags)
                if (util::iequals(str, tag.tag())) {
                    if (tag.checks().empty()) {
                        return true;
                    } else {
                        for (string check : tag.checks())
                            if (tagged_ref(check, tagged_ref))
                                return false;
                        return true;
                    }
                }
            return false;
        };
        return f(str, f);
    };

    vector<string> filtertags;

    auto filtertagged = [tagged, &filtertags](vector<string> v) -> bool {
        bool ret = false;
        for (string str : v) {
            if (tagged(str)) {
                filtertags.push_back(str);
                ret = true;
            }
        }
        return ret;
    };

    auto filtermenu = [&](Items::field f) {
        if (filtertags.size() == 1) {
            cout << "Here's what I found:" << endl;
            menu(items.filter(f, filtertags[0]));
        } else {
            vector<vector<Item>> filtereditems;
            for (string filtertag : filtertags)
                filtereditems.push_back(items.filter(f, filtertag));
            menu(merge(filtereditems));
        }
    };

    auto confirmorder = [&]() -> bool {
        if (!session::order.item.empty()) {
            cout << "You have selected:" << endl;
            session::order.item.display();

            cout << "How many would you like to order?" << endl;
            
            while ([&]() -> bool {
                sentence.read(prompt());

                vector<string>::iterator it = find_if(sentence.begin(), sentence.end(), util::isnumber);
                bool hasnumber = it != sentence.end();

                auto cancelorder = [&]() -> bool {
                    sentence.read(prompt("You're removing your order. Are you sure?"));
                    if (sentence.anyof(keys::yes)) {
                        session::order.item.clear();
                        cout << "Order removed." << endl;
                        return false;
                    }
                    return true;
                };

                if (hasnumber) {
                    if (stoi(*it) < 0) {
                        cout << "You can't order negative items!" << endl;
                    } else if (stoi(*it) == 0) {
                        return cancelorder();
                    } else {
                        session::order.quantity = stoi(*it);
                        cout << "You have ordered " << session::order.quantity << " x " << session::order.item.itemName() << endl;
                        session::orders.push_back(session::order);
                        return false;
                    }
                } else {
                    if (sentence.anyof(keys::cancel)) {
                        return cancelorder();
                    } else {
                        cout << "Please give me an amount." << endl;
                    }
                }
                return true;
            }());
        }
        return true;
    };
    
    if (tagged("bye") || sentence.anyof(keys::no)) {
        cout << "Goodbye. Have a nice day!" << endl;
        return false;
    }

    if (sentence.anyof(keys::yes)) {
        cout << "How can I help you?" << endl;
        return true;
    }

    if (tagged("beverage")) {
        cout << "Here's what I found:" << endl;
        menu(merge(items.filter(Items::field::dishType, "hot"),
                   items.filter(Items::field::dishType, "cold")));
        return confirmorder();
    }

    if (filtertagged(lists::stallNames)) {
        filtermenu(Items::field::stallName);
        return confirmorder();
    }

    if (filtertagged(lists::dishTypes)) {
        filtermenu(Items::field::dishType);
        return confirmorder();
    }

    if (filtertagged(lists::meatTypes)) {
        filtermenu(Items::field::meatType);
        return confirmorder();
    }

    if (tagged("makeorder")) {
        cout << "Here's what we have:" << endl;
        menu(items());
        return confirmorder();
    }

    if (tagged("vieworders")) {
        int i = 1;
        for (Order order : session::orders)
            order.list(i++);
        sentence.read(prompt("You can edit your orders, proceed to payment, or go back."));
        
        // if (tagged("editorders"))
        tags = sentence.parse(lists::tags);
        for (Tag t : tags)
            cout << t.tag() << endl;
        cout << tagged("checkout") << endl;
        // if (!tagged("checkout"))
        //     return true;
            // go back to conversation
        
        // implement editorders inside
    }
    
    if (tagged("checkout")) {
        // show order summary, with delivery tag
        if (session::orders.empty()) {
            cout << "You haven't made any orders yet." << endl;
            return true;
        }

        // if there are items that are deliverable, prompt user
        double totalPrice = 0;
        bool deliver = false;
        for (Order order : session::orders) {
            totalPrice += order.item.price() * order.quantity;
            if (!deliver && order.item.deliverable() == true)
                deliver = true;
        }
        if (deliver == true) {
            sentence.read(prompt("There are foods that can be delivered. Do you want it delivered?"));
            if (sentence.anyof(keys::yes)){
                // prompt address
                sentence.read(prompt("Please input your address here.")); 
                cout << "Okay. The food will be delivered to you once it's done cooking :)" << endl;
            }

                // optional, delivery fees?
                
        
        // payment/receipt : preference for cash 
            int i = 1;
            for (Order order : session::orders)
                order.list(i++);
            cout << "Total price: " << totalPrice << endl;
            sentence.read(prompt("Do you want to pay with cash or card."));
            cout << "Alright. Thanks for buying our meals." << endl;
            
        }
        return true;
    }

    if (tagged("recommend")) {
        // random recommendations
    }

    if (tagged("operation")) {
        cout << "We are open from 8am-4pm on weekdays." << endl;
    }

    return true;
}

string prompt(string str) {
    if (str != "")
        cout << str << endl;
    cout << ">> ";
    string response;
    getline(cin, response);
    return response;
}

void menu(vector<Item> items) {
    int  off, range;
    int  cur  = 1;
    int  tot  = (items.size() - 1) / 10 + 1;
    bool pgin = false;

    auto menupg = [&]() {
        off = (cur - 1) * 10 + 1;
        range = items.size() - off + 1;
        for (int i = off; i < off + (range < 10 ? range : 10); i++) {
            items[i - 1].list(i);
        }
        cout << "Page " << cur << "/" << tot << endl;
        pgin = false;
    };

    auto iteminbounds = [&](int i) -> bool {
        if (i < off || i >= off + (range < 10 ? range : 10)) {
                cout << "Item not on page!" << endl;
                return true;
            } else {
                session::order.item = items[i - 1];
                return false;
            }
    };
 
    auto searchitem = [&]() -> bool {
        vector<Item> listed = vector<Item>(items.begin() + off - 1, items.begin() + off - 1 + (range < 10 ? range : 10));
        vector<Item> found;

        // for each item on page
        for (Item item : listed) {
            // if sentence is found in item name
            if (Sentence(item.itemName()).search(sentence.str()))
                found.push_back(item);
        }
        if (found.size() > 1) {
            cout << "Here's what we found:" << endl;
            menu(found);
        } else if (found.empty()) {
            cout << "Item not found!" << endl;
            return true;
        } else {   
            session::order.item = found[0];
        }
        return false;
    };

    menupg(); // Always display first page

    if (items.size() <= 10) {
        while ([&]() -> bool {
            sentence.read(prompt());
            if (sentence.anyof(keys::cancel)) {
                return false;
            } else if (sentence.anyof(keys::previous)) {
                cout << "No previous page!" << endl;
            } else if (sentence.anyof(keys::next)) {
                cout << "No next page!" << endl;
            } else {
                vector<string>::iterator it = find_if(sentence.begin(), sentence.end(), util::isnumber);
                bool number = it != sentence.end();

                if (sentence.anyof(keys::page)) {
                    if (number && stoi(*it) == 1) {
                        menupg();
                    } else {
                        cout << "No other pages to go to!" << endl;
                    }
                } else if (number) {
                    return iteminbounds(stoi(*it));
                } else {
                    return searchitem();
                }
            }
            return true;
        }());
    } else {
        while ([&]() -> bool {
            sentence.read(prompt());
            if (sentence.anyof(keys::cancel)) {
                return false;
            } else if (sentence.anyof(keys::previous)) {
                if (cur != 1) {
                    --cur;
                    menupg();
                } else {
                    cout << "No previous page!" << endl;
                }
            } else if (sentence.anyof(keys::next)) {
                if (cur != tot) {
                    ++cur;
                    menupg();
                } else {
                    cout << "No next page!" << endl;
                }
            } else {
                auto pginbounds = [&](vector<string>::iterator it) {
                    if (stoi(*it) < 1 || stoi(*it) > tot) {
                        cout << "Page does not exist!" << endl;
                        pgin = true;
                    } else {
                        cur = stoi(*it);
                        menupg();
                    }
                };

                vector<string>::iterator it = find_if(sentence.begin(), sentence.end(), util::isnumber);
                bool hasnumber = it != sentence.end();

                if (sentence.anyof(keys::page)) {
                    if (!hasnumber) {
                        cout << "Which page would you like to go to?" << endl;
                        pgin = true;
                    } else {
                        pginbounds(it);
                    }
                } else if (hasnumber) {
                    if (pgin) {
                        pginbounds(it);
                    } else {
                        return iteminbounds(stoi(*it));
                    }
                } else {
                    return searchitem();
                }
            }
            return true;
        }());
    }
}

ifstream init::chk_openFile(string fileName) {
    ifstream file(fileName);
    if (file.is_open()) {
        cout << fileName << " opened successfully." << endl;
    } else {
        throw runtime_error("\"" + fileName + "\" cannot be opened.");
    }
    return file;
}

void init::generateList(string str, vector<string>& v) {
    if (!util::contains(str, v))
        v.push_back(str);
}

bool test(int argc, char** argv) {
    cout << (argc > 1 ? "[TEST MODE]\n" : "");

    // Declare test functions
    auto tf_list = [](vector<string> v) {
        for (string str : v)
            cout << str << ", ";
        cout << endl;
    };

    // Return true to end program after test
    switch (argc) {
    case 1:
        return false;

    case 2:
        switch (util::hash(argv[1])) {
        // Sandbox test area
        case util::hash("test"): {
            
            return true;
        }

        case util::hash("open"):
            return true;

        case util::hash("list"):
            // Print lists
            cout << "Stall names: "; tf_list(lists::stallNames);
            cout << "Dish types:  "; tf_list(lists::dishTypes);
            cout << "Meat types:  "; tf_list(lists::meatTypes);
            return true;

        case util::hash("tags"):
            for (Tag tag : lists::tags) {
                cout << tag.tag() << endl;
                if (!tag.checks().empty()) {
                    cout << "# ";
                    for (string str : tag.checks())
                    cout << str << ",";
                    cout << endl;
                }
                cout << "/ ";
                for (string str : tag.keywords())
                    cout << str << ",";
                cout << endl;
            }
            return true;

        case util::hash("items"):
            // Display items
            for (Item item : items)
                item.display();
            cout << "Item count: " << items.size() << endl;
            return true;

        case util::hash("menu"):
            // Display menu and result
            menu(items());
            // result.display();
            return true;

        default:
            cout << "Unknown argument" << endl;
            return true;
        }

    default:
        cout << "Invalid argument count" << endl;
    }
    return true;
}