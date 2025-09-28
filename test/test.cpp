#define CATCH_CONFIG_MAIN // CRITICAL: This generates the main() function for Catch2
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <string>

#include "AVL.h"
#include <vector>


using namespace std;

// --- 1. Test at least five incorrect commands / Invalid Data ---
TEST_CASE("1. Invalid Commands and Data Validation", "[invalid]") {
    AVL tree;
    string n = "Valid Name";
    string id_short = "1234567";
    string id_alpha = "1234AB78";
    string name_num = "N4me With 4 Num";
    string name_sym = "Name!";
    string id_remove_invalid = "1234ABCD";
    string id_search_short = "123";
    
    // Invalid UFID 1: Not 8 digits
    REQUIRE(tree.insert(n, id_short) == "unsuccessful"); 
    
    // Invalid UFID 2: Contains non-digit
    REQUIRE(tree.insert(n, id_alpha) == "unsuccessful"); 
    
    // Invalid Name 1: Contains number
    REQUIRE(tree.insert(name_num, id_alpha) == "unsuccessful"); 
    
    // Invalid Name 2: Contains symbol
    REQUIRE(tree.insert(name_sym, id_alpha) == "unsuccessful"); 

    // Invalid remove ID (non-digit)
    REQUIRE(tree.remove(id_remove_invalid) == "unsuccessful");

    // // Invalid search ID (not 8 digits)
    REQUIRE(tree.search_ID(id_search_short) == "unsuccessful"); 
}

// --- 2. Test three edge cases for various functions ---
TEST_CASE("2. Edge Cases", "[edge]") {
    AVL tree;
    string n = "John Doe";
    string id = "10000000";
    string id_nonexistent = "99999999";
    
    // Edge Case 1: Inserting the same ID twice (Duplicate UFID)
    REQUIRE(tree.insert(n, id) == "successful");
    REQUIRE(tree.insert(n, id) == "unsuccessful");

    // Edge Case 2: Removing a node that doesn't exist (ID not found)
    REQUIRE(tree.remove(id_nonexistent) == "unsuccessful");
    
    // Edge Case 3: Removing an element by inorder index that is out of bounds
    REQUIRE(tree.removeInorder(5) == "unsuccessful"); 
    REQUIRE(tree.removeInorder(0) == "successful"); // Removing the only node
    REQUIRE(tree.removeInorder(0) == "unsuccessful"); // Tree is now empty
}

// --- 3. Test all four rotation cases ---
TEST_CASE("3. All Four Rotations", "[rotation]") {
    AVL tree;
    string n = "TestName";
    
    // RR (Left Rotation) - Insertion Order: 10, 20, 30
    SECTION("RR Rotation") {
        string id10 = "10000000"; string id20 = "20000000"; string id30 = "30000000";
        tree.insert(n, id10); 
        tree.insert(n, id20); 
        tree.insert(n, id30); // Triggers RR at 10
         REQUIRE(tree.printInorder() == "TestName, TestName, TestName");
        REQUIRE(tree.printLevelCount() == "2");  
    }
    
    // LL (Right Rotation) - Insertion Order: 30, 20, 10
    SECTION("LL Rotation") {
        // Need to reset root because Catch2 sections run sequentially within a TEST_CASE scope
        AVL fresh_tree; 
        string id10 = "10000000"; string id20 = "20000000"; string id30 = "30000000";
        fresh_tree.insert(n, id30); 
        fresh_tree.insert(n, id20); 
        fresh_tree.insert(n, id10); // Triggers LL at 30
         REQUIRE(fresh_tree.printInorder() == "TestName, TestName, TestName");
        REQUIRE(fresh_tree.printLevelCount() == "2"); 
    }

    // LR (Left-Right Rotation) - Insertion Order: 30, 10, 20
    SECTION("LR Rotation") {
        AVL fresh_tree;
        string id10 = "10000000"; string id20 = "20000000"; string id30 = "30000000";
        fresh_tree.insert(n, id30); 
        fresh_tree.insert(n, id10); 
        fresh_tree.insert(n, id20); // Triggers LR at 30
         REQUIRE(fresh_tree.printInorder() == "TestName, TestName, TestName");
        REQUIRE(fresh_tree.printLevelCount() == "2"); 
    }

    // RL (Right-Left Rotation) - Insertion Order: 10, 30, 20
    SECTION("RL Rotation") {
        AVL fresh_tree;
        string id10 = "10000000"; string id20 = "20000000"; string id30 = "30000000";
        fresh_tree.insert(n, id10); 
        fresh_tree.insert(n, id30); 
        fresh_tree.insert(n, id20); // Triggers RL at 10
        REQUIRE(fresh_tree.printInorder() == "TestName, TestName, TestName");
         REQUIRE(fresh_tree.printLevelCount() == "2"); 
    }
}

// --- 4. Test all three deletion cases ---
TEST_CASE("4. Three Deletion Cases", "[deletion]") {
    AVL t;
    string n = "A";
    string id50 = "50000000"; string id30 = "30000000"; string id70 = "70000000";
    string id20 = "20000000"; string id40 = "40000000"; string id75 = "75000000";
    string n_child = "Child";

    // Setup a small balanced tree
    t.insert(n, id50); 
    t.insert(n, id30);
    t.insert(n, id70);
    t.insert(n, id20); 
    t.insert(n, id40);
    
    // Case 1: Node with 0 children (leaf - ID 40000000)
     REQUIRE(t.remove(id40) == "successful");
     REQUIRE(t.printInorder() == "A, A, A, A"); 
    
//     // Case 2: Node with 1 child (ID 70000000)
     t.insert(n_child, id75); // Add right child to 70
     REQUIRE(t.remove(id70) == "successful");
     REQUIRE(t.printInorder() == "A, A, A, Child"); 

//     // Case 3: Node with 2 children (ID 50000000, the root)
     REQUIRE(t.remove(id50) == "successful"); 
     REQUIRE(t.printInorder() == "A, A, Child"); 
}


// --- 5. Bulk Insert/Remove and Check Inorder ---
// --- 5. Bulk Insert/Remove and Check Inorder ---
TEST_CASE("5. Bulk Operations and Verification", "[bulk]") {
    AVL t;
    // Insert 100 nodes with unique UFIDs
    for (int i = 1; i <= 100; ++i) {
        stringstream ufid_ss;
        ufid_ss << 50000000 + i;  // 8-digit UFIDs (50000001, 50000002, ...)

        // Use valid names without digits (e.g., "StudentA", "StudentB"...)
        string name = "Student " + string(1, 'A' + (i % 26));

        string ufid = ufid_ss.str();
        
        REQUIRE(t.insert(name, ufid) == "successful");
    }
    
    // Check level count (should be balanced: height <= log2(100) + 1 ≈ 7.6, so max 8)
    REQUIRE(stoi(t.printLevelCount()) <= 8); 

    // Remove 10 nodes using removeInorder
    for (int i = 0; i < 10; ++i) {
        REQUIRE(t.removeInorder(5) == "successful");
    }
    
    // After 10 removals, the tree size should be 90.
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


TEST_CASE("5. Traversal Orders with Distinct Names", "[traversal]") {
    AVL t;

    // Insert nodes with distinct names so order matters
    t.insert("Root",  "50000000");
    t.insert("Left",  "30000000");
    t.insert("Right", "70000000");
    t.insert("Leaf",  "20000000");


    REQUIRE(t.printInorder()   == "Leaf, Left, Root, Right");     // L, Root, R
    REQUIRE(t.printPreorder()  == "Root, Left, Leaf, Right");     // Root first
    REQUIRE(t.printPostorder() == "Leaf, Left, Right, Root");     // Root last
}

TEST_CASE("6. search_NAME behavior", "[search_name]") {
    AVL t;

    // Empty tree -> unsuccessful
    REQUIRE(t.search_NAME("Alice") == "unsuccessful");

    // Insert a few nodes (note: UFID determines tree order)
    REQUIRE(t.insert("Alice", "10000000") == "successful");
    REQUIRE(t.insert("Bob",   "20000000") == "successful");
    REQUIRE(t.insert("Alice", "15000000") == "successful"); // second Alice, different UFID

    // Multiple matches: should return UFIDs newline-joined in inorder (sorted by UFID here)
    // Inorder by UFID: 10000000 (Alice), 15000000 (Alice), 20000000 (Bob)
    REQUIRE(t.search_NAME("Alice") == "10000000\n15000000");

    // Single match
    REQUIRE(t.search_NAME("Bob") == "20000000");

    // No match
    REQUIRE(t.search_NAME("Carol") == "unsuccessful");

    // After deletion of one "Alice", the remaining result should update accordingly
    REQUIRE(t.remove("10000000") == "successful");
    REQUIRE(t.search_NAME("Alice") == "15000000");

    // Delete remaining Alice -> now unsuccessful
    REQUIRE(t.remove("15000000") == "successful");
    REQUIRE(t.search_NAME("Alice") == "unsuccessful");
}

TEST_CASE("7. PrintLevelCount basics", "[levelcount]") {
    AVL t;
    REQUIRE(t.printLevelCount() == "0");

    REQUIRE(t.insert("A", "10000000") == "successful");
    REQUIRE(t.printLevelCount() == "1");

    t.insert("B", "20000000");
    t.insert("C", "30000000"); // should rebalance
    REQUIRE(stoi(t.printLevelCount()) <= 2); // height is balanced
}




