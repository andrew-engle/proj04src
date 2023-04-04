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

    return max_breadth;
}

/*
 * return true if the tree is empty. Return false otherwise
 */
template<typename T>
bool BET<T>::empty()
{
    return (root == nullptr);
}

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
    /*
    if (n != nullptr) {
        printInfixExpression(n->left);
        cout << n=n->element << " "; //ISSUES WITH OPERATOR?? <<
        printInfixExpression(n->right);
    }*/

    if(t->left !=nullptr)
    {
        if(presedance(t,t->left))
        {
            cout<<"(";
            printInfixExpression(t->left);
            cout<<")";
        }
        else
        {
            printInfixExpression(t->left);
        }
    }

    cout << t->element.getValue() << " ";


    if(t->right !=nullptr)
    {
        if(presedance(t,t->right) || presedance2(t,t->right))
        {
            cout<<"(";
            printInfixExpression(t->right);
            cout<<")";
        }
        else
        {
            printInfixExpression(t->right);
        }
    }
}


/*
 * delete all nodes in the subtree pointed to by t
 */
template<typename T>
void BET<T>::makeEmpty(BinaryNode* &t) {
    if (t != nullptr) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}
//ISSUES WITH MAKE EMPTY FUNCTION

/*
 * clone all nodes in the subtree pointed to by t. Can be called by functions such as the assignment operator=.
 */
template<typename T>
typename BET<T>::BinaryNode * BET<T>::clone(BinaryNode *t) {
    if (t == nullptr) {
        return nullptr;
    }
    return new BinaryNode(t->element, clone(t->left), clone(t->right));
}

/*
 * print to the standard output the corresponding postfix expression.
 */
template<typename T>
void BET<T>::printPostfixExpression(BinaryNode *n)
{
    if (n != nullptr) {
        printPostfixExpression(n->left);
        printPostfixExpression(n->right);
        cout << n->element.getValue() << " ";
    }
}

/*
 *
 */
/*
 * return the number of nodes in the subtree pointed to by t.
 */
template<typename T>
size_t BET<T>::size(BinaryNode *node){
    if (node == nullptr) {
        return 0;
    }
    return 1 + size(node->left) + size(node->right);
}

/*
 * return the number of leaf nodes in the subtree pointed to by t.
 */
template <typename T>
int BET<T>::leaves(BinaryNode *t) {
    if (t == nullptr) {
        return 0;
    }
    if (t->left == nullptr && t->right == nullptr) {
        return 1;
    }
    return leaves(t->left) + leaves(t->right);
}

/*
 *  return the depth of the subtree pointed to by t.
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


template <typename T>
bool BET<T>::presedance(BinaryNode *t1, BinaryNode *t2)                                                             //Literaral just checking nodes are combo of any two operators
{
    if((t1->element.getValue() == "*" || t1->element.getValue() == "/") && (t2->element.getValue() == "+" || t2->element.getValue() == "-"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <typename T>                                                                                               // Checking if they are certain combo of + and -
bool BET<T>::presedance2(BinaryNode *t1, BinaryNode *t2)
{
    if(t1->element.getValue() == t2->element.getValue())
        return true;
    if(t1->element.getValue()=="+" && t2->element.getValue()=="-" )
        return true;
    if(t1->element.getValue()=="-" && t2->element.getValue()=="+" )
        return true;

    return false;

}


#endif //PROJ04SRC_BET_HPP
