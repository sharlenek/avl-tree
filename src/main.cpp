#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include "AVL.h"

using namespace std;

// handle command line parsing and execution
void parseCommand(AVL &tree, const string &line)
{
    istringstream in(line);
    string command;
    in >> command;

    if (command == "insert")
    {
        string name;
        string ufid;

        // go to first quote
        char first_quote;
        if (!(in >> first_quote) || first_quote != '"')
        {
            cout << "unsuccessful" << endl;
            return;
        }

        // read the name content in the quotes
        if (!getline(in, name, '"'))
        {
            cout << "unsuccessful" << endl;
            return;
        }

        // read ufid after the quotes
        in >> ufid;

        // insert command
        cout << tree.insert(name, ufid) << endl;
    }
    else if (command == "remove")
    {
        // read ufid
        string ufid;
        in >> ufid;

        // validate ufid
        if (ufid.empty())
        {
            cout << "unsuccessful" << endl;
            return;
        }

        // remove function
        cout << tree.remove(ufid) << endl;
    }
    else if (command == "search")
    {
        string rest;
        getline(in, rest); // everything after "search"

        // find first character
        size_t start = rest.find_first_not_of(" \t\r\n");
        if (start == string::npos)
        {
            cout << "unsuccessful" << endl;
            return;
        }

        if (rest[start] == '"')
        {
            // name search, so find closing quote
            size_t close = rest.find('"', start + 1);
            if (close == string::npos || close == start + 1)
            {
                cout << "unsuccessful" << endl;
                return;
            }
            string name = rest.substr(start + 1, close - (start + 1));
            cout << tree.search_NAME(name) << endl;
        }
        else
        {
            // ufid search, read the token starting at 'start'
            size_t end = rest.find_first_of(" \t\r\n", start);
            string ufid = (end == string::npos) ? rest.substr(start)
                                                : rest.substr(start, end - start);

            // validate 8 digits
            if (ufid.size() != 8)
            {
                cout << "unsuccessful" << endl;
                return;
            }
            for (char c : ufid)
            {
                if (!isdigit(static_cast<unsigned char>(c)))
                {
                    cout << "unsuccessful" << endl;
                    return;
                }
            }
            cout << tree.search_ID(ufid) << endl;
        }
    }
    else if (command == "printInorder")
    {
        cout << tree.printInorder() << endl;
    }
    else if (command == "printPreorder")
    {
        cout << tree.printPreorder() << endl;
    }
    else if (command == "printPostorder")
    {
        cout << tree.printPostorder() << endl;
    }
    else if (command == "printLevelCount")
    {
        cout << tree.printLevelCount() << endl;
    }
    else if (command == "removeInorder")
    {
        int n;
        if (in >> n)
        {
            cout << tree.removeInorder(n) << endl;
        }
        else
        {
            cout << "unsuccessful" << endl;
        }
    }
    else
    {
        cout << "unsuccessful" << endl;
    }
}

int main()
{
    AVL tree;
    string numOfCommandsStr;

    // get num of commands from first line
    getline(cin, numOfCommandsStr);
    int numOfCommands = 0;
    numOfCommands = stoi(numOfCommandsStr);

    // parse each line for commands
    for (int i = 0; i < numOfCommands; i++)
    {
        string line;
        if (!getline(cin, line))
            break;
        parseCommand(tree, line);
    }

    return 0;
}