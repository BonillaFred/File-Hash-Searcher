#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <openssl/sha.h>
#include <ctype.h>
#include "searcher.h"

const int MAKE_HASH_OPTION = 1;
const int SEARCH_HASH_OPTION = 2;
const int QUIT_OPTION = 3;

unsigned char* hashFile(const char* currentFileName)
{
    unsigned char* fileDigest = malloc(sizeof(unsigned char) * SHA256_DIGEST_LENGTH);

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    FILE* currentFile = fopen(currentFileName, "rb");
    if(currentFile == NULL)
    {
        free(fileDigest);
        return NULL; 
    }

    char* buffer = malloc(sizeof(char) * 1024);
    int readBytes = fread(buffer, 1, sizeof(char) * 1024, currentFile); 
    while(readBytes != 0)
    {
        SHA256_Update(&sha256, buffer, readBytes);
        readBytes = fread(buffer, 1, sizeof(char) * 1024, currentFile);
    }
    fclose(currentFile);
    free(buffer);

    SHA256_Final(fileDigest, &sha256);
    return fileDigest; 
}

// probaly using more memory than I need too :(
char* getStringHash(const char* fileName)
{
    int outputSize = sizeof(char) * 65; // Including null term
    char* result = NULL;
    unsigned char* hash = hashFile(fileName);
    
    if(hash != NULL)
    {
        char* printfBuffer = malloc(sizeof(char) * 64);
        result = malloc(outputSize);
        int read = 0;
        int current = 0;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            read = sprintf(printfBuffer, "%02x", hash[i]);
            read = read/sizeof(char);
            for(int j = 0; j < read; j++)
                result[current++] = printfBuffer[j];
        }  
        result[current++] = '\0';
        free(printfBuffer);
        free(hash);
        result[outputSize-1] = '\0';
    }
    return result;
}

void runThroughDirs(char* initalPath, NodePtr* head)
{
    struct dirent** dirEntries = NULL;
    int numEntries = 0; 

    numEntries = scandir(initalPath, &dirEntries, NULL, NULL);
    for(int i = 0; i < numEntries; i++)
    {
        int cmpSameCheck = strcmp(dirEntries[i]->d_name, ".\0");
        int cmpSameCheck2 = strcmp(dirEntries[i]->d_name, "..\0");
        int initalPathSize = strlen(initalPath) * sizeof(char);
        int direntDirNameSize = strlen(dirEntries[i]->d_name) * sizeof(char);
        char* sPrintfBuffer = malloc(initalPathSize + direntDirNameSize + 2);
        sprintf(sPrintfBuffer, "%s/%s", initalPath, dirEntries[i]->d_name);
        if(dirEntries[i]->d_type == DT_DIR && dirEntries[i]->d_name && cmpSameCheck2 != 0 && cmpSameCheck != 0)
        {
            runThroughDirs(sPrintfBuffer, head);
        }
        else if(cmpSameCheck != 0 && cmpSameCheck2 != 0)
        {
            char* stringHash = getStringHash(sPrintfBuffer);
            char* hashBuffer = malloc(sizeof(char) * 65);
            char* fileNameBuffer = malloc(sizeof(char) * 255);

            sprintf(fileNameBuffer, "%s/%s", initalPath, dirEntries[i]->d_name);
            sprintf(hashBuffer, "%s", stringHash);

            avlInsert(head, makeNode(hashBuffer, fileNameBuffer));

            free(stringHash);
        }
        free(sPrintfBuffer);
        free(dirEntries[i]);
    }
    free(dirEntries);

}

void menuDisplay()
{
    printf("%d: Make a hash for a file\n", MAKE_HASH_OPTION);
    printf("%d: Search for hash\n", SEARCH_HASH_OPTION);
    printf("%d: Quit\n", QUIT_OPTION);
}

void driverFunction(NodePtr* head)
{
    int option = 0; 
    int continueLoop = 1;
    while(continueLoop == 1)
    {
        menuDisplay(); 
        printf("Enter Option: ");
        scanf("%d", &option);
        if(option == MAKE_HASH_OPTION)
        {
            printf("MAKING HASH\n");
        }
        else if(option == SEARCH_HASH_OPTION)
        {
            printf("SEARCHING FOR HASH\n");
        }
        else if(option == QUIT_OPTION)
        {
            printf("STOPING\n");
            continueLoop = -1;
        }
        else
        {
            printf("Error: Input option not found :(\n");
        }
    }
}

int main(int argc, char* argv[])
{
    NodePtr head = NULL;
    driverFunction(&head);
    deleteTree(&head);
    return 0; 
}

/*
    if(argc > 2 || argc == 1)
    {
        printf("Sorry that's not correct!\n");
        printf("Should be like this: scanner sha256-hash\n");
        exit(-1);
    }
    int argumentLength = strnlen(argv[1], 256);
    char hash[argumentLength+1];
    char* unsanHash = argv[1];
    for(int i = 0; i < argumentLength; i++)
        hash[i] = tolower(unsanHash[i]);
    hash[argumentLength] = '\0';

    runThroughDirs(".\0", &head);
    NodePtr snc = searchTree(head, hash);
    if(snc == NULL)
    {
        printf("Hash not found!\n");
    }
    else
    {
        for(int i = 0; i < snc->arraySize; i++)
        {
            printf("Found At %s\n", snc->fileNames[i]);
        }
    }
    */