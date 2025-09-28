#include <iostream>
#include <sstream>
#include <string>
#include <cctype> 
#include "AVL.h"

using namespace std;

// Function to handle command line parsing and execution
void processCommand(AVL& tree, const string& line) {
    istringstream in(line);
    string command;
    in >> command;

    if (command == "insert") {
        string name;
        string ufid;

        // Skip leading whitespace until the first quote
        char first_quote;
        if (!(in >> first_quote) || first_quote != '"') {
             cout << "unsuccessful" << endl;
             return;
        }

        // Read the name content until the closing quote
        if (!getline(in, name, '"')) {
             cout << "unsuccessful" << endl;
             return;
        }

        // Read the UFID after the quoted name
        in >> ufid;
        
        // Use non-const string variables to satisfy the AVL.h function signature (string &)
        cout << tree.insert(name, ufid) << endl;

    } else if (command == "remove") {
        string ufid;
        in >> ufid;

        if (ufid.empty()) {
            cout << "unsuccessful" << endl;
            return;
        }

        cout << tree.remove(ufid) << endl;

    } else if (command == "search") {
        string searchParam;
        
        char peek_char = in.peek();

        if (peek_char == '"') {
            // Search by NAME
            in.get(); // Consume the opening quote
            if (!getline(in, searchParam, '"')) {
                cout << "unsuccessful" << endl;
                return;
            }
            cout << tree.search_NAME(searchParam) << endl;

        } else if (std::isdigit(peek_char)) {
            // Search by ID
            in >> searchParam;
            cout << tree.search_ID(searchParam) << endl;

        } else {
            cout << "unsuccessful" << endl;
            return;
        }

    } else if (command == "printInorder") {
        cout << tree.printInorder() << endl;

    } else if (command == "printPreorder") {
        cout << tree.printPreorder() << endl;

    } else if (command == "printPostorder") {
        cout << tree.printPostorder() << endl;

    } else if (command == "printLevelCount") {
        cout << tree.printLevelCount() << endl;

    } else if (command == "removeInorder") {
        int n;
        if (in >> n) {
            cout << tree.removeInorder(n) << endl;
        } else {
            cout << "unsuccessful" << endl; 
        }
    } else {
        cout << "unsuccessful" << endl;
    }
}


int main(){
    AVL tree;
    string numOfCommandsStr;

    if (!getline(cin, numOfCommandsStr)) return 0;
    
    for (char c : numOfCommandsStr) {
        if (!std::isdigit(c)) return 0;
    }

    int numOfCommands = 0;
    try {
        numOfCommands = stoi(numOfCommandsStr);
    } catch (...) {
        return 0;
    }

    for (int i = 0; i < numOfCommands; i ++){
        string line;
        if (!getline(cin, line)) break;
        processCommand(tree, line);
    }
    
    return 0;
}