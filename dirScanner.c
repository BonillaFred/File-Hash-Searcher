#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <openssl/sha.h>
#include <ctype.h>
#include "searcher.h"

const int MAKE_HASH_OPTION = 1;
const int SEARCH_HASH_OPTION = 2;
const int QUIT_OPTION = 3;

/**
 * hashes the current file that is found at the given 
 * file name. 
 * 
 * const char* currentFileName: the file name of the given file. 
 **/
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

/**
 * get the string value of the sha-256 hash. 
 * IMPROVEMENT:  probaly using more memory than I need too :(
 * 
 * const char* fileName: the file name of the file we want to hash. 
**/
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

/**
 * Simple recusive scan starting from the programs 
 * given dir.
 *
 * char* initalPath: the starting path 
 * 
 * NodePtr* head: this is the avl-trees head. 
 * 
 **/
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

void badMemoryError()
{
    printf("System does not enough memory to run\n");
    exit(-1);
}

void makeHashDriver()
{
    char* currentFile = NULL;
    char* inputBuffer = malloc(sizeof(char) * 128);
    if(inputBuffer == NULL)
    {
        badMemoryError();
    }
    printf("Please Enter logical file location with name(MAX 128): ");
    scanf("%128s", inputBuffer);
    currentFile = getStringHash(inputBuffer);
    if(currentFile != NULL)
    {
        printf("HASH = %s\n", currentFile);
        free(currentFile);
        free(inputBuffer);
    }
    else
    {
        printf("File not found sorry\n");

    }
}

void searchHashDriver(NodePtr head)
{
    char* input = malloc(sizeof(char) * 128);
    if(input == NULL)
    {
        badMemoryError(); 
    }
    printf("Please Enter SHA-256 hash(MAX-128): ");
    scanf("%128s", input);
    NodePtr snc = searchTree(head, input);
    if(snc == NULL)
    {
        printf("Not Found Sorry :(\n");
        return;
    }
    else
    {
        for(int i = 0; i < snc->arraySize; i++)
        {
            printf("Found at: %s\n", snc->fileNames[i]);
        }
    }
    free(input);
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
            makeHashDriver();
        }
        else if(option == SEARCH_HASH_OPTION)
        {
            searchHashDriver(*head);
        }
        else if(option == QUIT_OPTION)
        {
            printf("STOPING\n");
            continueLoop = -1;
        }
        else
        {
            printf("Error: Input option not found :(\n");
            // CELVER SOLN: https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { }
        }
    }
}

int main(int argc, char* argv[])
{
    NodePtr head = NULL;
    runThroughDirs(".\0", &head);
    driverFunction(&head);
    deleteTree(&head);
    return 0; 
}