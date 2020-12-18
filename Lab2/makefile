cc = gcc
flags = -Wall -pedantic -std=c99 -pthread
###___###
res 	 : main sort               ;
main 	 : main.c stringW.h	       ; $(cc) $(flags) main.c -o main
sort	 : sort.c stringW.h	qSort.h; $(cc) $(flags) sort.c -o sort -lm
clean 	 : 				           ; rm main sort printFile
###___###