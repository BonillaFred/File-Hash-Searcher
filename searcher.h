#ifndef SEARCHER_H
#define SEARCHER_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//The current sources helped me build the code.
//No one source helped with evrything but they did clear things for me 
//When coding and provided a good refrence for when things broke. 
//SRC: https://www.cs.swarthmore.edu/~brody/cs35/f14/Labs/extras/08/avl_pseudo.pdf
//SRC: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/

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

typedef struct Node* NodePtr;

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