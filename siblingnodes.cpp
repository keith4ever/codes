//
// Created by keith on 11/9/22.
//

#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

using namespace std;

class BST {
    int data;
    BST *left, *right;

public:
    // Default constructor.
    BST();

    // Parameterized constructor.
    BST(int);

    // Insert function.
    BST* Insert(BST*, int);

    // Inorder traversal.
    void Inorder(BST*);
    int maxDepth(BST* root);
    void addNodesToLevel(BST* root, vector<vector<BST*>>* pvect, int level);
};

// Default Constructor definition.
BST ::BST()
        : data(0)
        , left(NULL)
        , right(NULL)
{
}

// Parameterized Constructor definition.
BST::BST(int value)
{
    data = value;
    left = right = NULL;
}

// Insert function definition.
BST* BST::Insert(BST* root, int value)
{
    if (!root) {
        // Insert the first node, if root is NULL.
        return new BST(value);
    }

    // Insert data.
    if (value > root->data) {
        // Insert right node data, if the 'value'
        // to be inserted is greater than 'root' node data.
        // Process right nodes.
        root->right = Insert(root->right, value);
    }
    else {
        // Insert left node data, if the 'value'
        // to be inserted is greater than 'root' node data.
        // Process left nodes.
        root->left = Insert(root->left, value);
    }

    // Return 'root' node, after insertion.
    return root;
}

// Inorder traversal function.
// This gives data in sorted order.
void BST::Inorder(BST* root)
{
    if (!root) {
        return;
    }
    Inorder(root->left);
    cout << root->data << endl;
    Inorder(root->right);
}

int BST::maxDepth(BST* root) {
    if (root == nullptr) return 0;

    int left, right;
    left = maxDepth(root->left);
    right = maxDepth(root->right);
    return (left > right) ? (left + 1) : (right + 1);
}

void BST::addNodesToLevel(BST* root, vector<vector<BST*>>* pvect, int level){
    if(root == nullptr || (int)pvect->size() < level + 1)
        return;

    (*pvect)[level].push_back(root);
    addNodesToLevel(root->left, pvect, level+1);
    addNodesToLevel(root->right, pvect, level+1);
}

int main(){
    BST b, *root = nullptr;
    root = b.Insert(root, 5);
    b.Insert(root, 3);
    b.Insert(root, 2);
    b.Insert(root, 4);
    b.Insert(root, 7);
    b.Insert(root, 6);
    b.Insert(root, 8);
    b.Insert(root, 14);
    b.Insert(root, 17);
    b.Insert(root, 16);
    b.Insert(root, 13);
    b.Insert(root, 22);
    b.Insert(root, 18);

    int depth = b.maxDepth(root);

    vector<vector<BST*>> treevect(depth);
    treevect.reserve(depth);
    b.addNodesToLevel(root, &treevect, 0);
    for(vector<vector<BST*>>::iterator iter = treevect.begin(); iter != treevect.end(); iter++){
        cout << "[" << (iter - treevect.begin()) << "]" << iter->size() << endl;
    }
}