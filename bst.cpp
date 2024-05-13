/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

// BST tree depth algorithm 
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        
        int left, right; 
        left = maxDepth(root->left);
        right = maxDepth(root->right);
        return (left > right) ? (left + 1) : (right + 1);
    }
};

// BST symmetric check algorithm 
class Solution {
public:

    bool compareNode(TreeNode* left, TreeNode* right){
        if(left == nullptr && right == nullptr) return true; 
        else if(left != nullptr && right == nullptr) return false;
        else if(left == nullptr && right != nullptr) return false;

        if(right->val != left->val) return false; 
        
        if(!compareNode(left->left, right->right)) return false; 
        if(!compareNode(left->right, right->left)) return false; 
        return true;
    }
    
    bool isSymmetric(TreeNode* root) {
        if (root == nullptr) return true;
        
        return (compareNode(root->left, root->right));
    }
};

// BST is it valid BST? values on the left has to be smaller than the right, including parent.
// the idea is to put them in a serial vector and check one after another 
// if the previous one is smaller than the next
class Solution {
public:
    bool isValidBST(TreeNode* root) {
        if (root == nullptr) return true;
        
        vector<int> vec_val; 
        addValueToVector(root, vec_val);
        
        int prev = vec_val.front();
        for (vector<int>::iterator it = vec_val.begin() ; it != vec_val.end(); ++it)
        {
            if(it == vec_val.begin()) continue; 
            if(prev >= *it) return false; 
            prev = *it;
        }
        return true;
    }
    
    void addValueToVector(TreeNode* root, vector<int>& vec_val){
        if(root == nullptr) return;
        
        addValueToVector(root->left, vec_val);
        vec_val.push_back(root->val);
        addValueToVector(root->right, vec_val);
    }
    
};

// add node value, parent value first, and then add left, right at the same level
// make sure all values at the same depth have to be in the same vector<int>
class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (root == nullptr) return 0;
        
        int left, right; 
        left = maxDepth(root->left);
        right = maxDepth(root->right);
        return (left > right) ? (left + 1) : (right + 1);
    }
    
    void addToOrder(TreeNode* root, vector<vector<int>>& orderedTree, int level){
        if (root == nullptr) return;
        
        orderedTree[level].push_back(root->val);
        addToOrder(root->left, orderedTree, level+1);
        addToOrder(root->right, orderedTree, level+1);
    }
    
    vector<vector<int>> levelOrder(TreeNode* root) {
        int depth = maxDepth(root);
        vector<vector<int>> orderedTree(depth);
        if (root == nullptr) return orderedTree;

        orderedTree[0].push_back(root->val);
        addToOrder(root->left, orderedTree, 1);
        addToOrder(root->right, orderedTree, 1);
        
        return orderedTree; 
    }
};

// convert sorted array to BST, make sure the BST is height-balanced.
class Solution {
public:
    void assignToTree(TreeNode** root, vector<int>& nums){
        if(nums.size() < 1) return; 

        int mid = floor(nums.size()/2);
        *root = new TreeNode(nums[mid]);
        if(nums.size() == 1) return;  

        TreeNode* newnode = *root; 
        vector<int> firsthalf(nums.begin(), nums.begin() + mid);
        if(firsthalf.size() < 1) return;
        assignToTree(&newnode->left, firsthalf);

        vector<int> secondhalf(nums.begin() + (mid+1), nums.end());
        if(secondhalf.size() < 1) return;
        assignToTree(&newnode->right, secondhalf);
    }

    TreeNode* sortedArrayToBST(vector<int>& nums) {
        TreeNode* root = nullptr;
        assignToTree(&root, nums);

        return root;
    }
};