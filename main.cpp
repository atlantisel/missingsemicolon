#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>      // pair
#include <vector>
#include "Classes.h"
#include "Functions.h"

using namespace std;

struct {
    bool needsDelivery;
    float budget;
    vector<string> mentionedStalls;
    vector<string> mentionedTypes;
} session;

bool conversation();
string prompt(string);
ifstream chk_openFile(string);
void test(string, string);

Sentence sentence;
static bool start = true;

vector<Item> items;
vector<string> stallName_list;
vector<string> dishType_list;
vector<string> meatType_list;
vector<string> keyword_list;
vector<pair<string, vector<string>>> tagDict;

int main(int argc, char** argv) {
    switch (argc) {
        case 1: break;
        case 2: break;
        case 3: test(argv[1], argv[2]); return 0;
    }

    try {
        {
            // Initialise menu
            ifstream file(chk_openFile("Food List.csv"));
            string line;
            getline(file, line);
            while (getline(file, line)) {
                if (line.back() == '\r')
                    line.pop_back();
                items.push_back(Item(line));
            }
            
            // Generate lists
            for (Item item : items) {
                if (!isMember(item.get_stallName(), stallName_list))
                    stallName_list.push_back(item.get_stallName());
                if (!isMember(item.get_dishType(), dishType_list))
                    dishType_list.push_back(item.get_dishType());
                if (!isMember(item.get_meatType(), meatType_list))
                    meatType_list.push_back(item.get_meatType());
            }
        }

        {   // Initialise keywords and tags
            ifstream file(chk_openFile("parse/responseTags.csv"));
            string line;
            while (getline(file, line)) {
                string keyword, tagStr;
                stringstream ss(line);
                if (line.back() == '\r') line.pop_back(); 
                getline(ss, keyword, ',');
                ss >> tagStr;
                keyword_list.push_back(keyword);
                
                // Parse tags
                string tag;
                ss = stringstream(tagStr);
                vector<string> tags;
                while (getline(ss, tag, ';'))
                    tags.push_back(tag);
                
                // Add entry
                pair<string, vector<string>> entry(keyword, tags);
                tagDict.push_back(entry);
            }
        }
        
        while (conversation());
        
    }

    catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return -1;
    }
    
    return 0;
}

bool conversation() {
    if (start) {
        // Initial state
        sentence.read(prompt("Hi, I'm Ken. How can I help you out today?"));
        start = false;
    } else {
        // Subsequent state
        sentence.read(prompt("Is there anything else I can help you with?"));
    }
    
    /*
    for all words, search through
    check keywords
    then 
        check each non-keyword word with remainng lists
        for words that return results, take intersection
    */

    // Parse keywords
    for (string word : sentence.get()) {
        if (isMember(word, keyword_list))
            sentence.key(word);
    }
    

    if (sentence.is("bye")) { // temp end condition
        cout << "Goodbye. Have a nice day!" << endl;
        return false;
    }

    return true;
}

string prompt(string str) {
    cout << str << endl;
    cout << ">> ";
    string response;
    getline(cin, response);
    return response;
}

ifstream chk_openFile(string fileName) {
    ifstream file(fileName);
    if (file.is_open()) {
        cout << fileName << " opened successfully." << endl;
    } else {
        throw runtime_error("\"" + fileName + "\" cannot be opened.");
    }
    return file;
}

void test(string arg1, string arg2) {
    switch (::hash(arg1.c_str())) {
        case ::hash("test"):
            switch (::hash(arg2.c_str())) {

            }
            break;

        case ::hash("print"):
            switch (::hash(arg2.c_str())) {
                case ::hash("lists"):
                    {
                        // Initialise menu
                        ifstream file(chk_openFile("Food List.csv"));
                        string line;
                        getline(file, line);
                        while (getline(file, line)) {
                            if (line.back() == '\r')
                                line.pop_back();
                            items.push_back(Item(line));
                        }
                        
                        // Generate lists
                        for (Item item : items) {
                            if (!isMember(item.get_stallName(), stallName_list))
                                stallName_list.push_back(item.get_stallName());
                            if (!isMember(item.get_dishType(), dishType_list))
                                dishType_list.push_back(item.get_dishType());
                            if (!isMember(item.get_meatType(), meatType_list))
                                meatType_list.push_back(item.get_meatType());
                        }
                    }

                    { // Initialise keywords and tags
                        ifstream file(chk_openFile("parse/responseTags.csv"));
                        string line;
                        while (getline(file, line)) {
                            string keyword, tagStr;
                            stringstream ss(line);
                            if (line.back() == '\r')
                                line.pop_back();
                            getline(ss, keyword, ',');
                            ss >> tagStr;
                            keyword_list.push_back(keyword);

                            // Parse tags
                            string tag;
                            ss = stringstream(tagStr);
                            vector<string> tags;
                            while (getline(ss, tag, ';'))
                                tags.push_back(tag);

                            // Add entry
                            pair<string, vector<string>> entry(keyword, tags);
                            tagDict.push_back(entry);
                        }
                    }

                    // Print lists
                    cout << "stallName_list: ";
                    for (string str : stallName_list)
                        cout << str << ",";
                    cout << endl << "dishType_list: ";
                    for (string str : dishType_list)
                        cout << str << ",";
                    cout << endl << "meatType_list: ";
                    for (string str : meatType_list)
                        cout << str << ",";
                    cout << endl << "keyword_list: ";
                    for (string str : keyword_list)
                        cout << str << ",";
                    cout << endl;
                    break;
            }
    }
}