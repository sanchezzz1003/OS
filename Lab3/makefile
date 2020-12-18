cc = g++
flags = -Wall -pedantic -pthread -std=c++14 -g
###___###
res 	: main2	genTree			;
main2 	: main2.cpp genTree.h	; $(cc) $(flags) main2.cpp -o main2
genTree : genTree.cpp genTree.h ; $(cc) $(flags) genTree.cpp -o genTree -lm
clean 	: 						; rm main2 genTree
###___###