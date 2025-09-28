#define CATCH_CONFIG_MAIN 
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "AVL.h"
#include <vector>

using namespace std;

// test invalid commands and bad input
TEST_CASE("1. Invalid Commands and Data Validation", "[invalid]") {
    AVL tree;
    string n = "Sharlene Kho";
    string id_short = "1234567";
    string id_alpha = "1234AB78";
    string name_num = "Sharlene111 Kho234";
    string name_sym = "Shart!";
    string id_remove_invalid = "1234ABCD";
    string id_search_short = "123";
    
    // invalid ufid not 8 digits
    REQUIRE(tree.insert(n, id_short) == "unsuccessful"); 
    
    // invalid ufid contains letter
    REQUIRE(tree.insert(n, id_alpha) == "unsuccessful"); 
    
    // invalid name contains number
    REQUIRE(tree.insert(name_num, id_alpha) == "unsuccessful"); 
    
    // invalid name contains symbol
    REQUIRE(tree.insert(name_sym, id_alpha) == "unsuccessful"); 

    // invalid remove id
    REQUIRE(tree.remove(id_remove_invalid) == "unsuccessful");

    // invalid search id
    REQUIRE(tree.search_ID(id_search_short) == "unsuccessful"); 
}

// test edge cases
TEST_CASE("2. Edge Cases", "[edge]") {
    AVL tree;
    string n = "Sharlene Kho";
    string id = "10000000";
    string id_nonexistent = "99999999";
    
    // inserting same id twice
    REQUIRE(tree.insert(n, id) == "successful");
    REQUIRE(tree.insert(n, id) == "unsuccessful");

    // remove id not in tree
    REQUIRE(tree.remove(id_nonexistent) == "unsuccessful");
    
    // remove by inorder index out of bounds
    REQUIRE(tree.removeInorder(5) == "unsuccessful"); 
    REQUIRE(tree.removeInorder(0) == "successful"); 
    REQUIRE(tree.removeInorder(0) == "unsuccessful"); 
}

// test four rotations
TEST_CASE("3. All Four Rotations", "[rotation]") {
    AVL tree;
    string n = "Poop";
    
    SECTION("rr rotation") {
        string id10 = "10000000"; string id20 = "20000000"; string id30 = "30000000";
        tree.insert(n, id10); 
        tree.insert(n, id20); 
        tree.insert(n, id30);
        REQUIRE(tree.printInorder() == "Poop, Poop, Poop");
        REQUIRE(tree.printLevelCount() == "2");  
    }
    
    SECTION("ll rotation") {
        AVL fresh_tree; 
        string id10 = "10000000"; string id20 = "20000000"; string id30 = "30000000";
        fresh_tree.insert(n, id30); 
        fresh_tree.insert(n, id20); 
        fresh_tree.insert(n, id10);
        REQUIRE(fresh_tree.printInorder() == "Poop, Poop, Poop");
        REQUIRE(fresh_tree.printLevelCount() == "2"); 
    }

    SECTION("lr rotation") {
        AVL fresh_tree;
        string id10 = "10000000"; string id20 = "20000000"; string id30 = "30000000";
        fresh_tree.insert(n, id30); 
        fresh_tree.insert(n, id10); 
        fresh_tree.insert(n, id20);
        REQUIRE(fresh_tree.printInorder() == "Poop, Poop, Poop");
        REQUIRE(fresh_tree.printLevelCount() == "2"); 
    }

    SECTION("rl rotation") {
        AVL fresh_tree;
        string id10 = "10000000"; string id20 = "20000000"; string id30 = "30000000";
        fresh_tree.insert(n, id10); 
        fresh_tree.insert(n, id30); 
        fresh_tree.insert(n, id20);
        REQUIRE(fresh_tree.printInorder() == "Poop, Poop, Poop");
        REQUIRE(fresh_tree.printLevelCount() == "2"); 
    }
}

// deletion 
TEST_CASE("4. Three Deletion Cases", "[deletion]") {
    AVL t;
    string n = "A";
    string id50 = "50000000"; string id30 = "30000000"; string id70 = "70000000";
    string id20 = "20000000"; string id40 = "40000000"; string id75 = "75000000";
    string n_child = "Child";

    // small balanced tree
    t.insert(n, id50); 
    t.insert(n, id30);
    t.insert(n, id70);
    t.insert(n, id20); 
    t.insert(n, id40);
    
    // remove leaf
    REQUIRE(t.remove(id40) == "successful");
    REQUIRE(t.printInorder() == "A, A, A, A"); 
    
    // remove node with one child
    t.insert(n_child, id75);
    REQUIRE(t.remove(id70) == "successful");
    REQUIRE(t.printInorder() == "A, A, A, Child"); 

    // remove node with two children
    REQUIRE(t.remove(id50) == "successful"); 
    REQUIRE(t.printInorder() == "A, A, Child"); 
}

// test bulk insert and remove
TEST_CASE("5. Bulk Operations and Verification", "[bulk]") {
    AVL t;
    for (int i = 1; i <= 100; ++i) {
        stringstream ufid_ss;
        ufid_ss << 50000000 + i;
        string name = "Student " + string(1, 'A' + (i % 26));
        string ufid = ufid_ss.str();
        REQUIRE(t.insert(name, ufid) == "successful");
    }
    
    REQUIRE(stoi(t.printLevelCount()) <= 8); 

    for (int i = 0; i < 10; ++i) {
        REQUIRE(t.removeInorder(5) == "successful");
    }
    
    string inorder_output = t.printInorder();
    stringstream ss(inorder_output);
    string segment;
    int count = 0;
    while(getline(ss, segment, ',')) {
        size_t first_char = segment.find_first_not_of(' ');
        if (first_char != string::npos) {
            count++;
        }
    }
    REQUIRE(count == 90);
}

// traversal
TEST_CASE("5. Traversal Orders", "[traversal]") {
    AVL t;
    t.insert("Root",  "50000000");
    t.insert("Left",  "30000000");
    t.insert("Right", "70000000");
    t.insert("Leaf",  "20000000");

    REQUIRE(t.printInorder()   == "Leaf, Left, Root, Right");
    REQUIRE(t.printPreorder()  == "Root, Left, Leaf, Right");
    REQUIRE(t.printPostorder() == "Leaf, Left, Right, Root");
}

// search by name
TEST_CASE("6. search_NAME behavior", "[search_name]") {
    AVL t;
    REQUIRE(t.search_NAME("Alice") == "unsuccessful");

    REQUIRE(t.insert("Alice", "10000000") == "successful");
    REQUIRE(t.insert("Bob",   "20000000") == "successful");
    REQUIRE(t.insert("Alice", "15000000") == "successful");

    REQUIRE(t.search_NAME("Alice") == "10000000\n15000000");
    REQUIRE(t.search_NAME("Bob") == "20000000");
    REQUIRE(t.search_NAME("Carol") == "unsuccessful");

    REQUIRE(t.remove("10000000") == "successful");
    REQUIRE(t.search_NAME("Alice") == "15000000");

    REQUIRE(t.remove("15000000") == "successful");
    REQUIRE(t.search_NAME("Alice") == "unsuccessful");
}

// level count
TEST_CASE("7. PrintLevelCount basics", "[levelcount]") {
    AVL t;
    REQUIRE(t.printLevelCount() == "0");

    REQUIRE(t.insert("A", "10000000") == "successful");
    REQUIRE(t.printLevelCount() == "1");

    t.insert("B", "20000000");
    t.insert("C", "30000000");
    REQUIRE(stoi(t.printLevelCount()) <= 2);
}
