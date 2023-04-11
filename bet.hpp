//
// Created by Andrew Engle on 3/30/23.
//

#ifndef PROJ04SRC_BET_HPP
#define PROJ04SRC_BET_HPP
#include <queue>
#include <iostream>
#include <cstdlib>


using namespace std;

//############## Public Functions ###########################

/*
 * default zero-parameter constructor.
 * Builds an empty tree.
 */
template <typename T>
BET<T>::BET()
{
    root = nullptr;
}

/*
 *  one-parameter constructor, where parameter "postfix"
 *  is a list representing a postfix expression.
 *  The tree should be built based on the postfix expression.
 */
template <typename T>
BET<T>::BET(const list<Token> & postfix){
    root = nullptr;
    buildFromPostfix(postfix);
}

/*
 *  copy constructor -- makes appropriate deep copy of the tree
 */
template <typename T>
BET<T>::BET(const BET&t){
    root=  clone(t.root);

}

/*
 * destructor -- cleans up all dynamic space in the tree
 */
template <typename T>
BET<T>::~BET(){
    makeEmpty(root);
}

/*
 * parameter "postfix" is a list representing a postfix expression.
 * A tree should be built based on each postfix expression
 * Tokens in the postfix expression are separated by spaces.
 * If the tree contains nodes before the function is called, you need to first delete the existing nodes.
 * Return true if the new tree is built successfully.
 * Return false if an error is encountered.
 */
template <typename T>
bool BET<T>::buildFromPostfix(const list<Token>& postfix) {
    // Delete existing nodes
    makeEmpty();

    // Initialize variables
    int numOperators = 0;
    int numOperands = 0;
    vector<BinaryNode*> myVector;

    // Iterate through each Token in the postfix expression
    for (auto itr = postfix.begin(); itr != postfix.end(); itr++) {
        if (itr->getType() == 1 || itr->getType() == 2 || itr->getType() == 3) {
            // If the Token is an operand, create a new node and add it to the vector
            BinaryNode* newNode = new BinaryNode(itr->getValue(), nullptr, nullptr);
            myVector.push_back(newNode);
            numOperands++;
        } else {
            // If the Token is an operator, check if there are at least two nodes in the vector
            if (myVector.size() >= 2) {
                // Create a new node with the operator Token's value and set its left and right children to the last two nodes in the vector
                numOperators++;
                BinaryNode* newNode = new BinaryNode(itr->getValue(), nullptr, nullptr);
                newNode->right = myVector[myVector.size() - 1];
                myVector.pop_back();
                newNode->left = myVector[myVector.size() - 1];
                myVector.pop_back();
                // Add the new node to the vector
                myVector.push_back(newNode);
            } else {
                // If there are not enough nodes in the vector, print an error message and return false
                if (numOperators == numOperands) {
                    cout << "Error: Unpaired opcode " << myVector[0]->element.getValue() << endl;
                } else if (numOperators < numOperands) {
                    cout << "Error: Operator " << myVector[0]->element.getValue() << " has only one operand " << endl;
                } else {
                    cout << "Other error " << myVector[0]->element.getValue() << endl;
                }
                return false;
            }
        }
    }

    // Check if there is exactly one node left in the vector
    if (myVector.size() == 1) {
        // Set the last node in the vector as the root of the tree and return true
        root = myVector[0];
        myVector.empty();
        return true;
    } else if (myVector.size() > 1) {
        // If there are too many nodes left in the vector, print an error message and return false
        if (numOperators == numOperands) {
            cout << "Error: Operator " << myVector[0]->element.getValue() << " has only one operand" << endl;
        } else if (numOperators < numOperands) {
            cout << "Error: Unpaired opcode " << myVector[0]->element.getValue() << endl;
        } else {
            cout << "Other error " << myVector[0]->element.getValue() << endl;
        }
        return false;
    } else {
        // If the vector is empty, return false
        return false;
    }
}

/*
 * assignment operator -- makes appropriate deep copy.
 */
template <typename T>
const BET<T> & BET<T>::operator= (const BET<T> & t){
    if (this == &t)  // check for self-assignment
        return *this;

    makeEmpty();  // clear current tree

    if (t.root == nullptr)  // if rhs is empty, return empty BET
        return *this;

    root = clone(t.root);  // clone rhs's tree

    return *this;
}

/*
 * Print out the infix expression. Should do this by making use of the private (recursive) version
 */
template <typename T>
void BET<T>::printInfixExpression()
{
    printInfixExpression(root);
    cout << endl;

}

/*
 * Print the postfix form of the expression. Use the private recursive function to help
 */
template <typename T>
void BET<T>::printPostfixExpression()
{
    printPostfixExpression(root);
    cout << endl;
}

/*
 * Return the number of nodes in the tree (using the private recursive function)
 */
template<typename T>
size_t BET<T>::size(){
    return size(root);

}

/*
 * Return the number of leaf nodes in the tree. (Use the private recursive function to help)
 */
template<typename T>
int BET<T>::leaves (){
    return leaves(root);
}

/*
 * return the depth of the tree.
 */
template<typename T>
int BET<T>::depth(){
    return depth(root);
}

/*
 * return the breadth of the tree.
 */
template<typename T>
int BET<T>::breadth() {
    if (root == nullptr) {  // empty tree
        return 0;
    }

    int max_breadth = 1;  // initialize the maximum breadth as 1 (i.e., the root)
    queue<BinaryNode*> q;
    q.push(root);

    while (!q.empty()) {
        int level_size = q.size();
        max_breadth = std::max(max_breadth, level_size);  // update max_breadth

        for (int i = 0; i < level_size; i++) {
            BinaryNode* node = q.front();
            q.pop();
            if (node->left != nullptr) {
                q.push(node->left);
            }
            if (node->right != nullptr) {
                q.push(node->right);
            }
        }
    }

    return max_breadth;}

/*
 * return true if the tree is empty.
 * Return false otherwise
 */
template<typename T>
bool BET<T>::empty()
{
    return (root == nullptr);
}

/*
 * free up the memory used by all the nodes in the binary expression tree.
 * It can be used to clear the tree before building a new expression or
 * when the tree is no longer needed to avoid memory leaks.
 */
template<typename T>
void BET<T>::makeEmpty()
{
    makeEmpty(root);
}

//############## Private Functions ###########################

/*
 * print to the standard output the corresponding infix expression.
 * Note that you may need to add parentheses depending on the precedence of operators.
 * You should not have unnecessary parentheses.
 */
template <typename T>
void BET<T>::printInfixExpression(BinaryNode *t){
    if(t->left !=nullptr) // if t has a left child
    {
        if(priority(t,t->left)) // if the left child has higher precedence than t
        {
            cout<<"("; // print an opening parenthesis
            printInfixExpression(t->left); // recursively print the left subtree in infix form
            cout<<")"; // print a closing parenthesis
        }
        else // if the left child has lower or equal precedence than t
        {
            printInfixExpression(t->left); // recursively print the left subtree in infix form without parentheses
        }
    }
    cout << t->element.getValue() << " "; // print the value of t
    if(t->right !=nullptr) // if t has a right child
    {
        if(priority(t,t->right) || priority2(t,t->right)) // if the right child has higher precedence than t, or equal precedence with right associativity
        {
            cout<<"("; // print an opening parenthesis
            printInfixExpression(t->right); // recursively print the right subtree in infix form
            cout<<")"; // print a closing parenthesis
        }
        else // if the right child has lower precedence than t, or equal precedence with left associativity
        {
            printInfixExpression(t->right); // recursively print the right subtree in infix form without parentheses
        }
    }
}

/*
 * delete all nodes in the subtree pointed to by t
 */
template<typename T>
void BET<T>::makeEmpty(BinaryNode* &t) {
    if (t != nullptr) {
    // Recursively delete the left and right children of t
        makeEmpty(t->left);
        makeEmpty(t->right);
    // Delete the current node t
        delete t;
    }
    // Set the value of the given node t to nullptr
    t = nullptr;
}

/*
 * clone all nodes in the subtree pointed to by t. Can be called by functions such as the assignment operator=.
 */
template<typename T>
typename BET<T>::BinaryNode * BET<T>::clone(BinaryNode *t) {
    // If t is nullptr, the function returns nullptr.
    if (t == nullptr) {
        return nullptr;
    }
    return new BinaryNode(t->element, clone(t->left), clone(t->right));
    // The function is recursive, as it calls itself to clone the left and right children of t.
}

/*
 * print to the standard output the corresponding postfix expression.
 */
template<typename T>
void BET<T>::printPostfixExpression(BinaryNode *n)
{
    // If the current node is not null, recursively call printPostfixExpression on the left and right children, then print the value of the node
    if (n != nullptr) {
        printPostfixExpression(n->left);  // recursively call printPostfixExpression on the left child
        printPostfixExpression(n->right); // recursively call printPostfixExpression on the right child
        cout << n->element.getValue() << " "; // print the value of the current node followed by a space
    }
}

/*
 * return the number of nodes in the subtree pointed to by t.
 */
template<typename T>
size_t BET<T>::size(BinaryNode *node){
    // If the node is null, it has no children and the size is 0
    if (node == nullptr) {
        return 0;
    }
    // If the node is not null, its size is the size of its left subtree + size of its right subtree + 1 (for itself)
    return 1 + size(node->left) + size(node->right);
}

/*
 * return the number of leaf nodes in the subtree pointed to by t.
 */
template <typename T>
int BET<T>::leaves(BinaryNode *t) {
    // If the current node is null, it doesn't have any leaves so return 0
    if (t == nullptr) {
        return 0;
    }
    // If the current node is a leaf, it only has one leaf so return 1
    if (t->left == nullptr && t->right == nullptr) {
        return 1;
    }
    // If the current node is not a leaf, then recursively find the number of leaves
    // in the left and right subtrees and return their sum
    return leaves(t->left) + leaves(t->right);
}

/*
 *  return the depth of the subtree pointed to by t.
 *  This function calculates the depth of the given BinaryNode 't' recursively
 *  If the BinaryNode is empty, then the depth is -1
 *  If the BinaryNode has no children, then the depth is 0
 *  Otherwise, the depth is 1 + the maximum depth of its left and right subtrees
 */
template <typename T>
int BET<T>::depth(BinaryNode* &t){
    if (t == nullptr) {
        return -1;
    }
    int leftDepth = depth(t->left);
    int rightDepth = depth(t->right);
    return 1 + std::max(leftDepth, rightDepth);
}

/*
 * return the breadth of the subtree pointed to by t.
 * Hint: this one requires a helper function for a recursive implementation.
 * But you do not have to have a recursive implementation.
 */
template<typename T>
int BET<T>::breadth(BinaryNode* &t) {
    if (t == nullptr) {
        return 0;
    }
    queue<BinaryNode*> q;
    q.push(t);
    int breadth = 0;
    while (!q.empty()) {
        breadth = max(breadth, (int)q.size());

        for (int i = 0; i < q.size(); i++) {
            BinaryNode* node = q.front();
            q.pop();

            if (node->left != nullptr) {
                q.push(node->left);
            }

            if (node->right != nullptr) {
                q.push(node->right);
            }
        }
    }
    return breadth;
}

/*
 * This function returns true if the operator in t1
 * has higher precedence than the operator in t2, and false otherwise.
 *
 * The function checks whether the operator in t1 is either multiplication (*)
 * or division (/) and the operator in t2 is either addition (+) or subtraction (-).
 * If both conditions are true, it means that the operator in t1 has
 * higher precedence than the operator in t2, and the function returns true.
 * Otherwise, it returns false.
 */
template <typename T>
bool BET<T>::priority(BinaryNode *t1, BinaryNode *t2)
{
    return (t1->element.getValue() == "*" || t1->element.getValue() == "/") && (t2->element.getValue() == "+" || t2->element.getValue() == "-");

}

/*
 * This code checks whether the two nodes have the same value first,
 * and then uses a switch statement to check the cases where t1 is either
 * '+' or '-', and t2 is the opposite operator. If either of these cases
 * is true, the function returns true. Otherwise, it returns false.
 */
template <typename T>
bool BET<T>::priority2(BinaryNode *t1, BinaryNode *t2)
{
    if (t1->element.getValue() == t2->element.getValue()) {
        return true;
    }
    switch (t1->element.getValue()[0]) {
        case '+':
            if (t2->element.getValue()[0] == '-') {
                return true;
            }
            break;
        case '-':
            if (t2->element.getValue()[0] == '+') {
                return true;
            }
            break;
    }
    return false;
}


#endif //PROJ04SRC_BET_HPP
