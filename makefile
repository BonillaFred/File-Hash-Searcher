hashFile = -I/opt/ssl/include/ -L/opt/ssl/lib/ -lcrypto 

all:
	gcc -c -o3 -Wall searcher.c -o avlTreeLib
	gcc  -o3 -Wall dirScanner.c -o scanner $(hashFile) avlTreeLib 
	./scanner

searcher:
	gcc -c -o3 searcher.c -o avlTreeLib

scanner:
	gcc -g dirScanner.c -o scanner $(hashFile)
	./scanner

clean:
	rm -rf main.c~ out scanner dirScanner.c~ searcher searcher.c~ main.c avlTreeLib fileDB.txt

check:
	valgrind --show-leak-kinds=all --track-origins=yes --leak-check=full -s ./scanner
