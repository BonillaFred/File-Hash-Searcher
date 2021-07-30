#include "searcher.h"


//The current sources helped me build the code.
//No one source helped with evrything but they did clear things for me 
//When coding and provided a good refrence for when things broke. 
//SRC: https://www.cs.swarthmore.edu/~brody/cs35/f14/Labs/extras/08/avl_pseudo.pdf
//SRC: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/

//TODO: Might add delete but I don't see a need for it

NodePtr makeNode(char* hash, char* fileName)
{
    NodePtr newNode = malloc(sizeof(Node));
    newNode->hash = hash;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->height = 1; 
    newNode->arraySize = 1;
    newNode->maxCount = 3;
    newNode->fileNames = (char**) malloc(newNode->maxCount * sizeof(char *)); 
    newNode->fileNames[0] = fileName;
    return newNode;
}

int getCmp(NodePtr x, NodePtr y)
{
    if(x == NULL || y == NULL)
    {
        return 0;
    }
    else
    {
        return strcmp(x->hash, y->hash);
    }
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int getHeightSlow(NodePtr current)
{
    if(current == NULL)
        return 0;
    else
        return 1 + max(getHeightSlow(current->left), 
                       getHeightSlow(current->right));
}

void updateNodeHeight(NodePtr* node)
{
    NodePtr current = *node;
    if(current == NULL)
        return;
    
    int leftHeight = getNodeHeight(current->left);
    int rightHeight = getNodeHeight(current->right);
    
    //TODO: Make inplace
    (*node)->height = 1 + max(leftHeight, rightHeight); 
}

int getNodeHeight(NodePtr node)
{
    if(node == NULL)
        return 0;
    return node->height;
}

void rightRotate(NodePtr* root,NodePtr* current)
{
    NodePtr leftChild = (*current)->left;
    (*current)->left = leftChild->right;
    if(leftChild->right != NULL)
        (*current)->left->parent = *current;
    leftChild->right = NULL;
    leftChild->parent = (*current)->parent;
    leftChild->right = (*current);
    (*current)->parent = leftChild;
    if(leftChild->parent != NULL)
    {
        int cmp = getCmp(leftChild->parent, *current);
        if(cmp > 0)
            leftChild->parent->left = leftChild;
        else if (cmp < 0)
            leftChild->parent->right = leftChild;
    }
    updateNodeHeight(&leftChild);
    updateNodeHeight(current);
    *current = leftChild;
    *root = leftChild;
}

void leftRotate(NodePtr* root,NodePtr* current)
{
    NodePtr rightChild = (*current)->right;
    (*current)->right = rightChild->left;
    if(rightChild->left != NULL)
        (*current)->right->parent = *current;
    rightChild->left = NULL;
    rightChild->parent = (*current)->parent;
    rightChild->left = (*current);
    (*current)->parent = rightChild;
    if(rightChild->parent != NULL)
    {
        int cmp = getCmp(rightChild->parent, *current);
        if(cmp > 0)
            rightChild->parent->left = rightChild;
        else if (cmp < 0)
            rightChild->parent->right = rightChild;
    }
    updateNodeHeight(&rightChild);
    updateNodeHeight(current);
    *current = rightChild;
    *root = rightChild;
}

void rightLeftRotate(NodePtr* root,NodePtr* current)
{
    rightRotate(root, &((*current)->right)); 
    leftRotate(root, current);
}

void leftRightRotate(NodePtr* root, NodePtr* current)
{
    leftRotate(root, &(*current)->left);
    rightRotate(root, current);
}

void bstBalence(NodePtr* root, NodePtr current)
{
    if(current == NULL)
    {
        *root = current;
    }
    updateNodeHeight(&current);
    updateNodeHeight(&current);
    updateNodeHeight(&(current->left));
    updateNodeHeight(&(current->right));
    int leftHeight = getNodeHeight(current->left);
    int rightHeight = getNodeHeight(current->right);

    if(leftHeight > rightHeight + 1)
    {
        NodePtr currentLeft = current->left;
        if(currentLeft != NULL)
        {
            updateNodeHeight(&(currentLeft->left));
            updateNodeHeight(&(currentLeft->right));
            int leftLeftHeight = getNodeHeight(currentLeft->left);
            int leftRightHeight = getNodeHeight(currentLeft->right);

            if(leftLeftHeight >= leftRightHeight)
            {
                rightRotate(root, &current);
            }
            else
            {
                leftRightRotate(root, &current);
            }
        }
    }

    if(rightHeight > leftHeight + 1)
    {
        NodePtr currentRight = current->right;
        if(currentRight != NULL)
        {

            updateNodeHeight(&(currentRight->left));
            updateNodeHeight(&(currentRight->right));
            int rightLeftHeight = getNodeHeight(currentRight->left);
            int rightRightHeight = getNodeHeight(currentRight->right);

            if(rightRightHeight >= rightLeftHeight)
            {
                leftRotate(root, &current);
            }
            else
            {
                rightLeftRotate(root, &current);
            }
        }
    }
    *root = current;
    return;
}

void bstInsert(NodePtr* root, NodePtr parent, NodePtr newNode)
{
    NodePtr current = *root;
    if(current == NULL)
    {
        current = newNode;
        current->parent = parent;
    }
    else
    {
        int cmp = getCmp(current, newNode);
        if(cmp > 0)
        {
            bstInsert(&(current->left), current, newNode);
        }
        if(cmp < 0)
        {
            bstInsert(&(current->right), current, newNode);
        }
        if(cmp == 0)
        {
            if(current->arraySize <= current->maxCount )
            {
                current->maxCount += 3;
                current->fileNames = (char**)realloc(current->fileNames,     
                                            sizeof(*current->fileNames) * current->maxCount);
            }
            if(current->fileNames == NULL)
                exit(-1);
            current->fileNames[current->arraySize] = newNode->fileNames[0];
            current->arraySize++; 
            
            free(newNode->fileNames);
            free(newNode->hash);
            free(newNode);
        }
    }
    *root = current;
    bstBalence(root, current);
}

void avlInsert(NodePtr* root, NodePtr newNode)
{
    bstInsert(root, NULL, newNode);
}

void printTree(NodePtr root)
{
    if(root != NULL)
    {
        printf("%s\n", root->hash);
        printTree(root->left);
        printTree(root->right);
    }
}

void deleteTree(NodePtr* root)
{
    if(*root != NULL)
    {
        deleteTree(&((*root)->left));
        deleteTree(&((*root)->right));
        
        for(int i = 0; i < (*root)->arraySize; i++)
            free((*root)->fileNames[i]);

        free((*root)->fileNames);
        free((*root)->hash);
        free(*root);
    }
}

NodePtr searchTree(NodePtr root, char* hash)
{
    if(root == NULL)
        return NULL;
    else
    {
        int cmp = strcmp(root->hash, hash);
        if(cmp == 0)
            return root;
        else if(cmp > 0)
            return searchTree(root->left, hash);
        else
            return searchTree(root->right, hash);
    }
}