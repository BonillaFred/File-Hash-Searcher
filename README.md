# Warning 
I may have made a mistake so please don't run this on system where you don't have backups. The program might corrupt files. Though I havent encountered this problem it is possible 
so please use this at your own risk. I can't gurentee things wont break.  

# File Hasher

The goal of the this repo is to search for a file using a sha-256 hash. Currently only works for linux system that have openssl and dirent installed. 

From my home dir:

|SPEED | MEMORY USE| MEM-LEAKS|
|------|-----------|----------|
|1ms   | 45MB      | None Using make check|

## Building From Source:
  make

## Running 
  ## Note where the program runs is where things are indexed. 
  ./scanner
  
  
# TODO:
  1. Replace all STRCPY with STRNCPY : need to prevent buffer overflow 
  2. Add better error check when malloc failes to find memory
  3. Add a windows devlopment stream 
  4. Add ability to decide where we should index all files
  5. Add GIF showing common use case 
