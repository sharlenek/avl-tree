#pragma once

#include <string>
#include <vector>


struct Node {
    std::string name;
    std::string ufid; // keep as string to preserve leading zeros
    int height;
    Node* left;
    Node* right;

    Node(const std::string& name_, const std::string& ufid_) {
        name = name_;
        ufid = ufid_;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

class AVL {
private:
    Node* root = nullptr;

    // core utils
    int height(Node* node);
    int getBalanceFactor(Node* node);
    Node* rebalance(Node* node);
    Node* rightRotate(Node* y);
    Node* leftRotate(Node* x);
    Node* leftRightRotate(Node* node);
    Node* rightLeftRotate(Node* node);

    // helpers
    Node* insertHelper(Node* node, const std::string& name, const std::string& ufid, bool& success);
    Node* removeHelper(Node* node, const std::string& ufid);
    //Node* minNode(Node* node);

    void printInorder(Node* node, std::vector<std::string>& out);
    void printPreorder(Node* node, std::vector<std::string>& out);
    void printPostorder(Node* node, std::vector<std::string>& out);

public:
    std::string insert(const std::string& name, const std::string& ufid);
    std::string remove(const std::string& ufid);

    std::string search_ID(const std::string& ufid); // return NAME or "unsuccessful"
    std::string search_NAME(const std::string& name); 
    void searchNameHelper(Node* node, const std::string& name, std::vector<std::string>& matches);

    std::string printInorder(); // comma-separated NAMES
    std::string printPreorder();
    std::string printPostorder();
    std::string printLevelCount();

    std::string removeInorder(int n);

    ~AVL() = default;
};
