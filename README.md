# Warning 
I may have made a mistake so please don't run this on system where you don't have backups. The program might corrupt files. Though I havent encountered this problem is it possible 
so please use this at your own risk. 

# File Hasher

The goal of the this repo is to search for a file using a sha-256 hash. Currently only works for linux system that have openssl and dirent installed. 

## Building From Source:
  make

## Running 
  ./scanner file-hash
  
  
# TODO:
  1. Replace all STRCPY with STRNCPY : need to prevent buffer overflow 
  2. Add better error check when malloc failes to find memory 
  3. Add better documentation for all functions 
  4. Add a windows devlopment stream 
