#ifndef SEARCHER_H
#define SEARCHER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * This file provides the needed operation for the AVL tree data struct 
 * that was deemed to be the fastest way to look up data. 
 **/

//The current sources helped me build the code.
//No one source helped with evrything but they did clear things for me 
//When coding and provided a good refrence for when things broke. 
//SRC: https://www.cs.swarthmore.edu/~brody/cs35/f14/Labs/extras/08/avl_pseudo.pdf
//SRC: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/


/**
 * Central Node that hold the needed information for 
 * each file. 
 * 
 * char* hash: the sha-256 or any other key used for the 
 *             tree to cmp. 
 * 
 * int height: the height of each node. 
 * 
 * char** filenames: the 2D array that keeps track of the locations 
 *                  in which the hash was found. 
 * 
 * int arraySize: the index to the file location stored in filenames. 
 * 
 * int maxCount: the current maximum space that is allowed in filenames. 
 * 
 * struct Node* (left, right, parent): the links used for the tree. 
 * 
 **/ 
struct Node
{
    char* hash;
    int height;

    char** fileNames;
    int arraySize;
    int maxCount;

    struct Node* left;
    struct Node* right;
    struct Node* parent;

} Node;

/**
 * The short hand used for struct Node*
 **/
typedef struct Node* NodePtr;

/**
 * Initalizing a Node to default values with the needed 
 * key items being provided to the user. 
 * 
 * char* hash: the current string hash-> pointer will be freed by the system 
 * char* fileName: current file location-> pointer will be free by the system. 
 * 
 * Return:
 *  the newly made pointer that will need to be released. 
 **/ 
NodePtr makeNode(char* hash, char* fileName);

/**
 * gets the cmp value from strcmp for each Nodes hash. 
 * A null is also preformed for both nodes before strcmp is called.
 * 
 * NodePtr x: The node we want to cmp it's hash. 
 * NodePtr y: The second node we want tp cmp 
 *
 * Return:
 *  the cmp value 0 if NULL is found for either node
 **/
int getCmp(NodePtr x, NodePtr y);

/**
 * get the max between two numbers
 * 
 * int a: first num 
 * int b: second num
 * 
 * return:
 *  the largest pf the two nums
 **/
int max(int a, int b);

/**
 * Runs from the current node and count the height. 
 * It shouldn't be used unless needed to confirm the 
 * tasked result. 
 * 
 * NodePtr current: the node we want to find the height of. 
 * 
 * Return:
 *  the height for that node. 
 **/ 
int getHeightSlow(NodePtr current);

/**
 * Updates the node height in place so we don't 
 * need to search from here to the bottom. 
 * 
 * NodePtr* node: the whose height we need to update
 **/
void updateNodeHeight(NodePtr* node);

/**
 * get the node height if null returns 0. 
 * 
 * NodePtr node: the node whose height we need. 
 **/
int getNodeHeight(NodePtr node);

//STANDARD 
void rightRotate(NodePtr* root,NodePtr* current);

//STANDARD 
void leftRotate(NodePtr* root,NodePtr* current);

//STANDARD 
void rightLeftRotate(NodePtr* root,NodePtr* current);

//STANDARD 
void leftRightRotate(NodePtr* root, NodePtr* current);

//STANDARD 
void bstBalence(NodePtr* root, NodePtr current);

//STANDARD 
void bstInsert(NodePtr* root, NodePtr parent, NodePtr newNode);

//STANDARD 
void avlInsert(NodePtr* root, NodePtr newNode);

//STANDARD Pre-order traversal. 
void printTree(NodePtr root);

/**
 * frees all used memory. 
 * 
 * NodePtr* root: the root node that has all the connections. 
 **/
void deleteTree(NodePtr* root);

//STANDARD 
NodePtr searchTree(NodePtr root, char* hash);

#endif