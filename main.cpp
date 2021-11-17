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

    enum field {stallName, dishType, meatType, price, deliverable, recommended};
    enum recommend {yes, no};

    vector<Item> filter(field f, string str = "", recommend r = no) {
        vector<Item> res;

        auto checkrecommended = [r, &res](Item item) {
            switch (r) {
            case yes:
                if (!item.recommended()) break;
                
            case no:
                res.push_back(item);
            }
        };
        
        switch (f) {
        case stallName:
            for (Item item : items) 
                if (Sentence(item.stallName()).search(str))
                    checkrecommended(item);
            break;

        case dishType:
            for (Item item : items) 
                if (Sentence(item.dishType()).search(str))
                    checkrecommended(item);
            break;

        case meatType:
            for (Item item : items) 
                if (Sentence(item.meatType()).search(str))
                    checkrecommended(item);
            break;

        case price:
            for (Item item : items)
                if (item.price() <= stof(str))
                    checkrecommended(item);
            break;

        case deliverable:
            for (Item item : items)
                if (item.deliverable())
                    checkrecommended(item);
            break;

        case recommended:
            for (Item item : items)
                if (item.recommended())
                    checkrecommended(item);
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
             << setw(5) << "" << quantity << " x "
             << item.price() << " = "
             << (quantity * item.price()) << endl;
    }

    void display() {
        item.display();
        cout << fixed << setprecision(2)
             << quantity << " x "
             << item.price() << " = "
             << (quantity * item.price()) << endl;
    }

    void clear() {
        item.clear();
        quantity = 0;
    }
};

namespace lists {
    vector<string> stallNames;
    vector<string> dishTypes;
    vector<string> meatTypes;
    vector<Tag>    tags;
};

enum state {start, empty, next, skip};

namespace session {
    state         nowstate = start;
    Order         order;
    vector<Order> orders;

    void clear() {
        order.clear();
        orders.clear();
    }
};

namespace keys {
    const vector<string> yes = {
        "affirmative", "alright", "correct", "ok", "okay", "oke", "okeh", "okey", "pos", "positive", "right", "sure", "true", "y", "ya", "yah", "ye", "yeah", "yeh", "yep", "yes", "yup"};
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
    const vector<string> remove = {
        "delete", "deleted", "deletes", "deleting", "erase", "erased", "erases", "erasing", "remove", "removed", "removes", "removing"};
};

enum menumode {select, view};

template <class T>
void   t_menu(vector<T>, T&, string s = "", menumode mode = menumode::select);
void   menu  (vector<Item>);
void   menu  (vector<Item>,  Item&,  string, menumode);
void   menu  (vector<Order>, Order&, string, menumode);

string prompt(string = "");
bool   test  (int, char**);

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

        while ([&]() -> bool {
            switch (session::nowstate) {
            case state::start:
                sentence.read(prompt("Hi, I'm Walter. How can I help you out today?"));
                session::nowstate = state::next;
                break;

            case state::next:
                sentence.read(prompt("Is there anything else I can help you with?"));
                break;

            case state::empty:
                sentence.read(prompt());
                session::nowstate = state::next;
                break;

            case state::skip:
                session::nowstate = state::next;
                break;
            }

            vector<Tag> tags = sentence.parse(lists::tags);
            
            // check for tag in user input
            // includes dependent tag check processing
            const auto tagged = [&tags](string str) -> bool {
                auto f = [&tags](string str, auto &&tagged_ref) mutable -> bool {
                    for (Tag tag : tags) {
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
                    }
                    return false;
                };
                return f(str, f);
            };

            vector<string> filtertags;

            // check for tags associated with filters
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

            // return recommended subsets of filtered menus if specified in user input
            Items::recommend recommend = tagged("recommend") ? Items::recommend::yes : Items::recommend::no;

            auto filtermenu = [&filtertags, recommend](Items::field f) {
                if (filtertags.size() == 1) {
                    cout << "Here's what I found:" << endl;
                    menu(items.filter(f, filtertags[0], recommend));
                } else {
                    vector<vector<Item>> filtereditems;
                    for (string filtertag : filtertags)
                        filtereditems.push_back(items.filter(f, filtertag, recommend));
                    menu(merge(filtereditems));
                }
            };

            // order confirmation after user has selected an item from menu
            auto confirmorder = [&]() -> bool {
                if (session::order.item.empty()) return true;

                cout << "You have selected:" << endl;
                session::order.item.display();

                cout << "How many would you like to order?" << endl;
                
                while ([&]() -> bool {
                    sentence.read(prompt());

                    vector<string>::iterator it = find_if(sentence.begin(), sentence.end(), util::isnumber);
                    bool hasnumber = it != sentence.end();

                    auto removeorder = [&]() -> bool {
                        sentence.read(prompt("You're removing your order. Are you sure?"));
                        if (sentence.anyof(keys::yes)) {
                            session::order.clear();
                            cout << "Order removed." << endl;
                            return false;
                        }
                        return true;
                    };

                    if (hasnumber) {
                        if (stoi(*it) == 0) {
                            return removeorder();
                        } else {
                            session::order.quantity = stoi(*it);
                            cout << "You have ordered " << session::order.quantity
                                    << " x " << session::order.item.itemName() << endl;
                            session::orders.push_back(session::order);
                            session::order.clear();
                            return false;
                        }
                    } else {
                        if (sentence.anyof(keys::cancel)) {
                            return removeorder();
                        } else {
                            cout << "Please give me an amount." << endl;
                        }
                    }
                    return true;
                }());
            
                return true;
            };
            
            if (tagged("bye") || (sentence.anyof(keys::no) && !tagged("vegetarian"))) {
                cout << "Goodbye. Have a nice day!" << endl;
                return false;
            }

            if (sentence.anyof(keys::yes)) {
                cout << "How can I help you?" << endl;
                session::nowstate = state::empty;
                return true;
            }

            if (tagged("beverage")) {
                cout << "Here's what I found:" << endl;
                menu(merge(items.filter(Items::field::dishType, "hot"),
                        items.filter(Items::field::dishType, "cold")));
                return confirmorder();
            }

            if (tagged("meat")) {
                cout << "Here's what I found:" << endl;
                menu(merge(vector<vector<Item>>({
                        items.filter(Items::field::meatType, "beef"),
                        items.filter(Items::field::meatType, "chicken"),
                        items.filter(Items::field::meatType, "fish"),
                        items.filter(Items::field::meatType, "seafood")})));
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

            if (recommend == Items::recommend::yes) {
                filtermenu(Items::field::recommended);
                return confirmorder();
            }

            if (tagged("vegetarian")) {
                cout << "Here's what I found:" << endl;
                menu(items.filter(Items::field::meatType, "none"));
                return confirmorder();
            }

            if (tagged("cheap")) {
                cout << "What's your budget? (Decimals not currently supported)" << endl;

                while ([confirmorder]() -> bool {
                    sentence.read(prompt());

                    vector<string>::iterator it = find_if(sentence.begin(), sentence.end(), util::isnumber);
                    bool hasnumber = it != sentence.end();
                    float price;

                    if (sentence.anyof(keys::cancel)) {
                        return false;
                    } else if (hasnumber) {
                        cout << "Here's what I found under "
                             << fixed << setprecision(2) << stof(*it) << ":" << endl;
                        menu(items.filter(Items::field::price, *it));
                        confirmorder();
                        return false;
                    } else {
                        cout << "Please give me an amount." << endl;
                        return true;
                    }
                }());

                return true;
            }

            // return filter categories
            // checked for after all filter tags to prevent conflict
            if (tagged("list")) {
                auto list = [](vector<string> v) {
                    for (string str : v) {
                        cout << str;
                        if (str != v.back()) cout << ", ";
                    }
                    cout << endl;
                };

                cout << "These are the categories that you can filter by:" << endl;
                // Print lists
                cout << "Stall names: "; list(lists::stallNames);
                cout << "Dish types:  "; list(lists::dishTypes);
                cout << "Meat types:  "; list(lists::meatTypes);
                return true;
            }

            // if "order" tag found in user input, ask for specification
            if (tagged("order")) {
                cout << "What would you like to do with your orders?" << endl;
                cout << "- Make order" << endl;
                cout << "- Edit orders" << endl;
                cout << "- View orders" << endl;
                session::nowstate = state::empty;
                return true;
            }

            // make order session
            if (tagged("makeorder")) {
                cout << "Here's what we have:" << endl;
                menu(items());
                return confirmorder();
            }

            // view order session
            if (tagged("vieworders")) {
                if (session::orders.empty()) {
                    cout << "You haven't made any orders yet." << endl;
                    return true;
                }

                cout << "Here are your orders:" << endl;
                menu(session::orders, session::order, "You can edit your orders, proceed to payment, or go back.", menumode::view);
                session::nowstate = state::skip;
                return true;
            }

            // edit order session
            if (tagged("editorders")) {
                if (session::orders.empty()) {
                    cout << "You haven't made any orders yet." << endl;
                    return true;
                }

                Order ordertoedit;

                cout << "Here are your orders:" << endl;
                menu(session::orders, ordertoedit, "Which one would you like to edit?", menumode::select);
                
                if (ordertoedit.item.empty()) return true;

                cout << "You have selected:" << endl;
                ordertoedit.display();

                cout << "How many would you like to order?" << endl;

                while ([&]() -> bool {
                    sentence.read(prompt());

                    vector<string>::iterator numit = find_if(sentence.begin(), sentence.end(), util::isnumber);
                    bool hasnumber = numit != sentence.end();
                    
                    vector<Order>::iterator odrit = find_if(session::orders.begin(), session::orders.end(),
                                                            [ordertoedit](Order order) {
                                                                return order.item == ordertoedit.item;
                                                            });

                    auto removeorder = [&]() -> bool {
                        sentence.read(prompt("You're removing your order. Are you sure?"));
                        if (sentence.anyof(keys::yes)) {
                            session::orders.erase(odrit);
                            cout << "Order removed." << endl;
                            return false;
                        }
                        return true;
                    };

                    if (hasnumber) {
                        if (stoi(*numit) == 0) {
                            return removeorder();
                        } else {
                            odrit->quantity = stoi(*numit);
                            cout << "Your order is now:" << endl;
                            odrit->display();
                            return false;
                        }
                    } else {
                        if (sentence.anyof(keys::cancel)) {
                            return false;
                        } else if (sentence.anyof(keys::remove)) {
                            return removeorder();
                        } else {
                            cout << "Please give me an amount." << endl;
                        }
                    }
                    return true;
                }());
                return true;
            }
            
            // checkout session
            if (tagged("checkout")) {
                // show order summary, with delivery tag
                if (session::orders.empty()) {
                    cout << "You haven't made any orders yet." << endl;
                    return true;
                }

                float totalPrice     = 0;
                bool  deliverable    = false;
                bool  alldeliverable = true;
                bool  deliver        = false;
                bool  cancel         = false;
                
                for (Order order : session::orders) {
                    totalPrice += order.item.price() * order.quantity;
                    order.item.deliverable() ? (deliverable = true) : (alldeliverable = false);
                }
                
                
                if (alldeliverable) { // if all orders are deliverable
                    cout << "Your orders can be delivered. Do you want it delivered?" << endl;
                    while ([&]() -> bool {
                        sentence.read(prompt());

                        if (sentence.anyof(keys::cancel)) {
                            cout << "Checkout cancelled." << endl;
                            cancel = true;
                            return false;
                        }

                        if (sentence.anyof(keys::yes)) { 
                            sentence.read(prompt("Please enter your address."));
                            if (sentence.anyof(keys::cancel)) {
                                cout << "Checkout cancelled." << endl;
                                cancel = true;
                                return false;
                            }
                            deliver = true;
                            return false;
                        } else if (sentence.anyof(keys::no)) {
                            return false;
                        } else {
                            cout << "Please confirm (yes/no) whether you would like your orders delivered." << endl;
                            return true;
                        }
                    }());

                    if (cancel) return true;

                } else if (deliverable) { // if only some orders are deliverable
                    cout << "While some of your orders are deliverable, there are still orders that require you to pick up." << endl;
                    cout << "We kindly request that you arrive at the canteen to pick up all of your orders." << endl;
                }
                
                // final order confirmation
                cout << "Please confirm your orders:" << endl;
                menu(session::orders, session::order, "", menumode::view);
                while ([&]() -> bool {
                    if (sentence.anyof(keys::no) || sentence.anyof(keys::cancel)) {
                        cout << "Checkout cancelled." << endl;\
                        cancel = true;
                        return false;
                    } else if (!sentence.anyof(keys::yes)) {
                        sentence.read(prompt("Please confirm (yes/no) your orders."));
                        return true;
                    } else {
                        return false;
                    }
                }());

                if (cancel) return true;

                // payment
                cout << "Your total price is " << totalPrice << "." << endl;
                
                cout << "Do you want to pay with cash or card?" << endl;

                while ([&]() -> bool {
                    sentence.read(prompt());

                    if (sentence.anyof(keys::cancel)) {
                        cout << "Checkout cancelled." << endl;
                        cancel = true;
                        return false;
                    }
                    
                    // card processing
                    if (sentence.anyof(vector<string>({"card", "cards", "credit", "debit"}))) {
                        cout << "Please enter your card number." << endl;
                        while ([&cancel]() -> bool {
                            string in = prompt();

                            if (Sentence(in).anyof(keys::cancel)) {
                                cout << "Checkout cancelled." << endl;
                                cancel = true;
                                return false;
                            }

                            in.erase(remove_if(in.begin(), in.end(),
                                            [](char c) {
                                                return !isdigit(c);
                                            }), in.end());
                            
                            if (in.empty()) {
                                cout << "Please enter a card number." << endl;
                            } else if (in.size() != 16) {
                                cout << "Please enter a valid card number." << endl;
                            } else {
                                return false;
                            }
                            return true;
                        }());
                        return false;
                    } else if (sentence.anyof(vector<string>({"bill", "bills", "cash", "note", "notes"}))) {
                        return false;
                    } else {
                        cout << "Please confirm (cash/card) your preferred payment method." << endl;
                        return true;
                    }
                }());

                if (cancel) return true;

                // thank you message
                deliver
                    ? cout << "The food will be delivered to you once it's out of the kitchen." << endl
                    : cout << "Please remember to pick up your orders within 20-40 minutes." << endl;
                cout << "Thank you and have a nice day. :)" << endl;
                
                // empty menu buffer
                session::clear();
                return true;
            }

            // FAQs
            // hours of operation
            if (tagged("operation")) {
                cout << "We are open from 8am-4pm on weekdays, closed on public holidays." << endl;
                return true;
            }

            // location of cafeteria
            if (tagged("location")) {
                cout << "Our cafeteria is located on our campus in Subang Jaya." << endl;
                return true;
            }

            // delivery area
            if (tagged("delivery")) {
                cout << "We currently only deliver to addresses in the Klang Valley." << endl;
                return true;
            }

            // if user is lost
            if (tagged("help")) {
                cout << "Here's what I can do:"    << endl;
                cout << "- Make order"             << endl;
                cout << "- Edit orders"            << endl;
                cout << "- View orders"            << endl;
                cout << "- Checkout"               << endl;
                cout << "- Meal recommendations"   << endl;
                cout << "Or you can ask me about:" << endl;
                cout << "- Location"               << endl;
                cout << "- Delivery Areas"         << endl;
                cout << "- Operation Hours"        << endl;
                return true;
            }

            // if no match
            cout << "Sorry, I didn't understand that." << endl;
            return true;
        }());
    }

    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }
    
    return 0;
}

// prompt user input with an optional sentence printed
string prompt(string str) {
    if (str != "") {
        cout << endl;
        cout << str << endl;
    }
    cout << ">> ";
    string response;
    getline(cin, response);
    return response;
}

void menu(vector<Item> v) {
    t_menu(v, session::order.item);
}

void menu(vector<Item> v, Item& i, string s, menumode mode) {
    t_menu(v, session::order.item);
}

void menu(vector<Order> v, Order& o, string s, menumode mode) {
    t_menu(v, o, s, mode);
}

// function for listing items and orders
template <class T> void t_menu(vector<T> v, T &t, string s, menumode mode) {
    bool pgin = false;
    int  cur  = 1;
    int  tot  = (v.size() - 1) / 10 + 1;
    int  off, range;

    // lists 10 objects as a menu page
    auto menupg = [&]() {
        off = (cur - 1) * 10 + 1;
        range = v.size() - off + 1;
        for (int i = off; i < off + (range < 10 ? range : 10); i++)
            v[i - 1].list(i);
        cout << "Page " << cur << "/" << tot << endl;
        if (!s.empty()) cout << s << endl;
        pgin = false;
    };

    // select by index
    auto select_by_index = [&](int i) -> bool {
        // if index is shown on page
        if (i >= off && i < off + (range < 10 ? range : 10)) {
            // return selection
            t = v[i - 1];
            return false;
        } else {
            cout << "Item not on page!" << endl;
            return true;
        }
    };

    // search by name, overloaded for Item and Order classes
    struct {
        vector<Item> operator()(vector<Item> v) {
            vector<Item> found;
            // for each item on page
            for (Item item : v) {
                // if sentence is found in item name
                if (Sentence(item.itemName()).search(sentence.str()))
                found.push_back(item);
            }
            // return found items
            return found; 
        }
        vector<Order> operator()(vector<Order> v) {
            vector<Order> found;
            // for each order on page
            for (Order order : v) {
                // if sentence is found in item name
                if (Sentence(order.item.itemName()).search(sentence.str()))
                found.push_back(order);
            }
            // return found orders
            return found;
        }
    } search;

    // select by search
    auto select_by_search = [&]() {
        // construct vector with items on page
        vector<T> listed = vector<T>(v.begin() + off - 1, v.begin() + off - 1 + (range < 10 ? range : 10));
        
        // search by name
        vector<T> found = search(listed);

        if (found.size() > 1) {
            // if multiple matches prompt user to select again
            cout << "Here's what we found:" << endl;
            menu(found, t, s, mode);
        } else if (found.empty()) {
            cout << "Item not found!" << endl;
            return true;
        } else {   
            t = found[0];
        }
        return false;
    };

    menupg(); // always display first page

    // one page
    if (v.size() <= 10) {
        while ([&]() -> bool {
            sentence.read(prompt());

            if (sentence.anyof(keys::cancel)) {
                // exit function
                return false;
            } else if (sentence.anyof(keys::previous)) {
                cout << "No previous page!" << endl;
            } else if (sentence.anyof(keys::next)) {
                cout << "No next page!" << endl;
            } else {
                if (mode == menumode::view) return false; // exit function

                // locate number
                vector<string>::iterator it = find_if(sentence.begin(), sentence.end(), util::isnumber);
                bool hasnumber = it != sentence.end();

                if (sentence.anyof(keys::page)) {
                    // if go to page 1
                    if (hasnumber && stoi(*it) == 1) {
                        menupg();
                    } else {
                        cout << "No other pages to go to!" << endl;
                    }
                } else if (hasnumber) {
                    return select_by_index(stoi(*it));
                } else {
                    return select_by_search();
                }
            }
            // default case prompt user input again
            return true;
        }());
    } else { // multiple pages
        while ([&]() -> bool {
            sentence.read(prompt());

            if (sentence.anyof(keys::cancel)) {
                // exit function
                return false;
            } else if (sentence.anyof(keys::previous)) {
                if (cur == 1) {
                    cout << "No previous page!" << endl;
                } else {
                    --cur;
                    menupg();
                }
            } else if (sentence.anyof(keys::next)) {
                if (cur == tot) {
                    cout << "No next page!" << endl;
                } else {
                    ++cur;
                    menupg();
                }
            } else {
                // check if user input is within page bounds
                auto pginbounds = [&](int i) {
                    if (i < 1 || i > tot) {
                        cout << "Page does not exist!" << endl;
                        pgin = true;
                    } else {
                        cur = i;
                        menupg();
                    }
                };

                // locate number
                vector<string>::iterator it = find_if(sentence.begin(), sentence.end(), util::isnumber);
                bool hasnumber = it != sentence.end();

                if (sentence.anyof(keys::page)) {
                    if (hasnumber) {
                        pginbounds(stoi(*it));
                    } else {
                        cout << "Which page would you like to go to?" << endl;
                        pgin = true;
                    }
                } else if (hasnumber) {
                    // if previous query was page number
                    if (pgin) {
                        pginbounds(stoi(*it));
                    } else {
                        if (mode == menumode::view) return false; // exit function
                        return select_by_index(stoi(*it));
                    }
                } else {
                    if (mode == menumode::view) return false; // exit function
                    return select_by_search();
                }
            }
            // default case prompt user input again
            return true;
        }());
    }
}

// file opening validation
ifstream init::chk_openFile(string fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        throw runtime_error("\"" + fileName + "\" cannot be opened.");
    }
    return file;
}

// generates initialisation lists
void init::generateList(string str, vector<string>& v) {
    if (!util::contains(str, v))
        v.push_back(str);
}

// unit testing
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