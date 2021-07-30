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

int getCmp(NodePtr x, NodePtr y);

int max(int a, int b);

int getHeightSlow(NodePtr current);

void updateNodeHeight(NodePtr* node);

int getNodeHeight(NodePtr node);

void rightRotate(NodePtr* root,NodePtr* current);

void leftRotate(NodePtr* root,NodePtr* current);

void rightLeftRotate(NodePtr* root,NodePtr* current);

void leftRightRotate(NodePtr* root, NodePtr* current);

void bstBalence(NodePtr* root, NodePtr current);

void bstInsert(NodePtr* root, NodePtr parent, NodePtr newNode);

void avlInsert(NodePtr* root, NodePtr newNode);

void printTree(NodePtr root);

void deleteTree(NodePtr* root);

NodePtr searchTree(NodePtr root, char* hash);

#endif