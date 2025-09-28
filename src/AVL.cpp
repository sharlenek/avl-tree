#include "AVL.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <queue>

using namespace std;

static bool isDigits8(const std::string& s) {
    if (s.size() != 8) return false;
    for (char c : s) if (c < '0' || c > '9') return false;
    return true;
}

static bool isValidName(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!( (c >= 'A' && c <= 'Z') ||
               (c >= 'a' && c <= 'z') ||
               c == ' ' )) return false;
    }
    return true;
}

int AVL::height(Node* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int AVL::getBalanceFactor(Node* node) {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

Node* AVL::rightRotate(Node* y) {
    Node* x  = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left  = T2;

    // Update heights
    y->height = 1 + std::max(height(y->left), height(y->right));
    x->height = 1 + std::max(height(x->left), height(x->right));

    // New root
    return x;
}

Node* AVL::leftRotate(Node* x) {
    Node* y  = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left  = x;
    x->right = T2;

    // Update heights
    x->height = 1 + std::max(height(x->left), height(x->right));
    y->height = 1 + std::max(height(y->left), height(y->right));

    // New root
    return y;
}

// Helpers for double rotations (in case they weren't implemented yet)
Node* AVL::leftRightRotate(Node* node) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
}

Node* AVL::rightLeftRotate(Node* node) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
}

Node* AVL::rebalance(Node* node) {
    int balanceFactor = getBalanceFactor(node);

    // Left heavy
    if (balanceFactor > 1) {
        // Left-Left
        if (getBalanceFactor(node->left) >= 0) {
            return rightRotate(node);
        }
        // Left-Right
        return leftRightRotate(node);
    }

    // Right heavy
    if (balanceFactor < -1) {
        // Right-Right
        if (getBalanceFactor(node->right) <= 0) {
            return leftRotate(node);
        }
        // Right-Left
        return rightLeftRotate(node);
    }

    // Balanced
    return node;
}

string AVL::insert(const string &name, const string &ufid) {
    if (!isValidName(name) || !isDigits8(ufid)) return "unsuccessful";
    bool success = false;
    this->root = insertHelper(this->root, name, ufid, success);
    return success ? "successful" : "unsuccessful";
}

Node* AVL::insertHelper(Node* node, const string &name, const string &ufid, bool& success) {
    if (node == nullptr) {
        success = true;
        return new Node(name, ufid);
    }

    if (ufid < node->ufid) {
        node->left = insertHelper(node->left, name, ufid, success);
    } else if (ufid > node->ufid) {
        node->right = insertHelper(node->right, name, ufid, success);
    } else {
        // Duplicate ufid; do not insert
        success = false;
        return node;
    }

    // Update height and rebalance
    node->height = 1 + std::max(height(node->left), height(node->right));
    success = true;
    return rebalance(node);
}

// utility: find the smallest node in a subtree
static Node* minNode(Node* node) {
    Node* cur = node;
    while (cur && cur->left) cur = cur->left;
    return cur;
}

Node* AVL::removeHelper(Node* node, const string& ufid) {
    if (node == nullptr) return nullptr;

    if (ufid < node->ufid) {
        node->left = removeHelper(node->left, ufid);
    } else if (ufid > node->ufid) {
        node->right = removeHelper(node->right, ufid);
    } else {
        // Found the node
        if (!node->left || !node->right) {
            Node* child = node->left ? node->left : node->right;
            delete node;
            return child; // may be nullptr if leaf
        } else {
            // Two children: replace with inorder successor
            Node* succ = minNode(node->right);
            node->name = succ->name;
            node->ufid = succ->ufid;
            node->right = removeHelper(node->right, succ->ufid);
        }
    }

    // Update height
    node->height = 1 + max(height(node->left), height(node->right));

    return rebalance(node);
}

string AVL::remove(const string& ufid) {
    if (!isDigits8(ufid)) return "unsuccessful";

    Node* cur = root;
    bool found = false;
    while (cur) {
        if (ufid < cur->ufid) cur = cur->left;
        else if (ufid > cur->ufid) cur = cur->right;
        else { found = true; break; }
    }
    if (!found) return "unsuccessful";

    root = removeHelper(root, ufid);
    return "successful";
}


string AVL::search_ID(const string& ufid) {
    if (!isDigits8(ufid)) return "unsuccessful";
    Node* cur = root;
    while (cur) {
        if (ufid < cur->ufid) cur = cur->left;
        else if (ufid > cur->ufid) cur = cur->right;
        else return cur->name;
    }
    return "unsuccessful";
}

void AVL::searchNameHelper(Node* node, const string& name, vector<string>& matches) {
    if (!node) return;
    searchNameHelper(node->left, name, matches);
    if (node->name == name) {
        matches.push_back(node->ufid);
    }
    searchNameHelper(node->right, name, matches);
}

string AVL::search_NAME(const string& name) {
    vector<string> matches;
    searchNameHelper(root, name, matches);

    if (matches.empty()) return "unsuccessful";

    string result = matches[0];
    for (size_t i = 1; i < matches.size(); i++) {
        result += "\n" + matches[i];
    }
    return result;
}


void AVL::printInorder(Node* node, vector<string>& out) {
    if (!node) return;
    printInorder(node->left, out);
    out.push_back(node->name);
    printInorder(node->right, out);
}

string AVL::printInorder() {
    vector<string> names;
    printInorder(root, names);

    if (names.empty()) return "";

    string s = names[0];
    for (size_t i = 1; i < names.size(); ++i) {
        s += ", " + names[i];
    }
    return s;
}

void AVL::printPreorder(Node* node, vector<string>& out) {
    if (!node) return;
    out.push_back(node->name);
    printPreorder(node->left, out);
    printPreorder(node->right, out);
}

string AVL::printPreorder() {
    vector<string> names;
    printPreorder(root, names);

    if (names.empty()) return "";

    string s = names[0];
    for (size_t i = 1; i < names.size(); ++i) {
        s += ", " + names[i];
    }
    return s;
}

void AVL::printPostorder(Node* node, vector<string>& out) {
    if (!node) return;
    printPostorder(node->left, out);
    printPostorder(node->right, out);
    out.push_back(node->name);
}

string AVL::printPostorder() {
    vector<string> names;
    printPostorder(root, names);

    if (names.empty()) return "";

    string s = names[0];
    for (size_t i = 1; i < names.size(); ++i) {
        s += ", " + names[i];
    }
    return s;
}

string AVL::printLevelCount() {
    if (root == nullptr) return "0";

    queue<Node*> q;
    q.push(root);
    int height = 0;

    while (!q.empty()) {
        int levelSize = q.size();
        height++;
        for (int i = 0; i < levelSize; i++) {
            Node* node = q.front();
            q.pop();
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }

    return to_string(height);
}


static void collectInorder(Node* node, vector<Node*>& nodes) {
    if (!node) return;
    collectInorder(node->left, nodes);
    nodes.push_back(node);
    collectInorder(node->right, nodes);
}

string AVL::removeInorder(int n) {
    if (root == nullptr) return "unsuccessful";

    vector<Node*> nodes;
    collectInorder(root, nodes);

    if (n < 0 || n >= (int)nodes.size()) {
        return "unsuccessful";
    }

    string ufidToRemove = nodes[n]->ufid;

    return remove(ufidToRemove);
}



