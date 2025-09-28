#include "AVL.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <queue>

using namespace std;

// checks if ufid is 8 digits
static bool isDigits8(const string &s)
{
    if (s.size() != 8)
        return false;
    for (char c : s)
        if (c < '0' || c > '9')
            return false;
    return true;
}

// makes sure name only has letters, no numbers or symbols
static bool isValidName(const string &s)
{
    if (s.empty())
        return false;
    for (char c : s)
    {
        if (!((c >= 'A' && c <= 'Z') ||
              (c >= 'a' && c <= 'z') ||
              c == ' '))
            return false;
    }
    return true;
}

// finds height of a node
int AVL::height(Node *node)
{
    if (node == nullptr)
        return 0;
    return node->height;
}

// gets balance factor of a node
int AVL::getBalanceFactor(Node *node)
{
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

// right rotation
Node *AVL::rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    // rotation
    x->right = y;
    y->left = T2;

    // update heights
    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    // new root
    return x;
}

Node *AVL::leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    // rotation
    y->left = x;
    x->right = T2;

    // update heights
    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    // new root
    return y;
}

// left right rotation
Node *AVL::leftRightRotate(Node *node)
{
    node->left = leftRotate(node->left);
    return rightRotate(node);
}

// right left rotation
Node *AVL::rightLeftRotate(Node *node)
{
    node->right = rightRotate(node->right);
    return leftRotate(node);
}

// rebalance node
Node *AVL::rebalance(Node *node)
{
    int balanceFactor = getBalanceFactor(node);

    // left heavy
    if (balanceFactor > 1)
    {
        // left left case
        if (getBalanceFactor(node->left) >= 0)
        {
            return rightRotate(node);
        }
        // left right case
        return leftRightRotate(node);
    }

    // right heavy
    if (balanceFactor < -1)
    {
        // right right case
        if (getBalanceFactor(node->right) <= 0)
        {
            return leftRotate(node);
        }
        // right left case
        return rightLeftRotate(node);
    }

    // balanced node
    return node;
}

// public insert function
string AVL::insert(const string &name, const string &ufid)
{
    // check if input is valid
    if (!isValidName(name) || !isDigits8(ufid))
        return "unsuccessful";
    bool success = false;
    this->root = insertHelper(this->root, name, ufid, success);
    return success ? "successful" : "unsuccessful";
}

// real insert function, recursive
Node *AVL::insertHelper(Node *node, const string &name, const string &ufid, bool &success)
{
    // base case
    if (node == nullptr)
    {
        success = true;
        return new Node(name, ufid);
    }

    // binary search tree insert. left if less, right if more
    if (ufid < node->ufid)
    {
        node->left = insertHelper(node->left, name, ufid, success);
    }
    else if (ufid > node->ufid)
    {
        node->right = insertHelper(node->right, name, ufid, success);
    }
    else
    {
        // duplicate ufid
        success = false;
        return node;
    }

    // update height + rebalance
    node->height = 1 + max(height(node->left), height(node->right));
    success = true;
    return rebalance(node);
}

// find inorder successor
static Node *minNode(Node *node)
{
    Node *cur = node;
    while (cur && cur->left)
        cur = cur->left;
    return cur;
}

// real remove function, recursive
Node *AVL::removeHelper(Node *node, const string &ufid)
{
    if (node == nullptr)
        return nullptr;

    // binary search tree delete
    if (ufid < node->ufid)
    {
        node->left = removeHelper(node->left, ufid);
    }
    else if (ufid > node->ufid)
    {
        node->right = removeHelper(node->right, ufid);
    }
    else
    {
        // node found
        if (!node->left || !node->right)
        {
            Node *child = node->left ? node->left : node->right;
            delete node;
            return child;
        }
        else
        {
            // remove with two children, replace with inorder successor
            Node *succ = minNode(node->right);
            node->name = succ->name;
            node->ufid = succ->ufid;
            node->right = removeHelper(node->right, succ->ufid);
        }
    }

    // update height + rebalance
    node->height = 1 + max(height(node->left), height(node->right));

    return rebalance(node);
}

// public remove function
string AVL::remove(const string &ufid)
{
    if (!isDigits8(ufid))
        return "unsuccessful";

    Node *cur = root;
    bool found = false;
    // check if ufid exists
    while (cur)
    {
        if (ufid < cur->ufid)
            cur = cur->left;
        else if (ufid > cur->ufid)
            cur = cur->right;
        else
        {
            found = true;
            break;
        }
    }
    if (!found)
        return "unsuccessful";

    // ufid exists, remove
    root = removeHelper(root, ufid);
    return "successful";
}

// public search by ufid function
string AVL::search_ID(const string &ufid)
{
    // check if ufid is valid
    if (!isDigits8(ufid))
        return "unsuccessful";

    Node *cur = root;

    // binary search tree search
    while (cur)
    {
        if (ufid < cur->ufid)
            cur = cur->left;
        else if (ufid > cur->ufid)
            cur = cur->right;
        else
            return cur->name;
    }
    return "unsuccessful";
}

// helper for search by name
void AVL::searchNameHelper(Node *node, const string &name, vector<string> &matches)
{
    if (!node)
        return;
    // recursive inorder traversal
    searchNameHelper(node->left, name, matches);
    if (node->name == name)
    {
        matches.push_back(node->ufid);
    }
    searchNameHelper(node->right, name, matches);
}

// public search by name function
string AVL::search_NAME(const string &name)
{
    vector<string> matches;
    searchNameHelper(root, name, matches);

    // no matches found
    if (matches.empty())
        return "unsuccessful";

    // concatenate matches with newlines
    string result = matches[0];
    for (size_t i = 1; i < matches.size(); i++)
    {
        result += "\n" + matches[i];
    }
    return result;
}

// inorder traversal helper
void AVL::printInorder(Node *node, vector<string> &out)
{
    if (!node)
        return;
    printInorder(node->left, out);
    out.push_back(node->name);
    printInorder(node->right, out);
}

// public inorder print function
string AVL::printInorder()
{
    vector<string> names;
    printInorder(root, names);

    if (names.empty())
        return "";

    string s = names[0];
    for (size_t i = 1; i < names.size(); ++i)
    {
        s += ", " + names[i];
    }
    return s;
}

// preorder traversal helper
void AVL::printPreorder(Node *node, vector<string> &out)
{
    if (!node)
        return;
    out.push_back(node->name);
    printPreorder(node->left, out);
    printPreorder(node->right, out);
}

// public preorder print function
string AVL::printPreorder()
{
    vector<string> names;
    printPreorder(root, names);

    if (names.empty())
        return "";

    string s = names[0];
    for (size_t i = 1; i < names.size(); ++i)
    {
        s += ", " + names[i];
    }
    return s;
}

// postorder traversal helper
void AVL::printPostorder(Node *node, vector<string> &out)
{
    if (!node)
        return;
    printPostorder(node->left, out);
    printPostorder(node->right, out);
    out.push_back(node->name);
}

// public postorder print function
string AVL::printPostorder()
{
    vector<string> names;
    printPostorder(root, names);

    if (names.empty())
        return "";

    string s = names[0];
    for (size_t i = 1; i < names.size(); ++i)
    {
        s += ", " + names[i];
    }
    return s;
}

// public level count function
string AVL::printLevelCount()
{
    if (root == nullptr)
        return "0";

    queue<Node *> q;
    q.push(root);
    int height = 0;

    while (!q.empty())
    {
        int levelSize = q.size();
        height++;
        for (int i = 0; i < levelSize; i++)
        {
            Node *node = q.front();
            q.pop();
            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);
        }
    }

    return to_string(height);
}

// helper to collect nodes in inorder
static void collectInorder(Node *node, vector<Node *> &nodes)
{
    if (!node)
        return;
    collectInorder(node->left, nodes);
    nodes.push_back(node);
    collectInorder(node->right, nodes);
}

// public remove inorder function
string AVL::removeInorder(int n)
{
    if (root == nullptr)
        return "unsuccessful";

    vector<Node *> nodes;
    collectInorder(root, nodes);

    if (n < 0 || n >= (int)nodes.size())
    {
        return "unsuccessful";
    }

    string ufidToRemove = nodes[n]->ufid;

    return remove(ufidToRemove);
}



