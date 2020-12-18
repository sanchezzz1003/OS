CC = gcc
LD = gcc
CCFLAGS = -Wall -pedantic -std=c99
LDFLAGS =
###___###
main		: main.o queue.o libqueue.so 	; $(LD) $(LDFLAGS) -rdynamic -o main main.o  -L. -lqueue
main.o		: main.c 						; $(CC) $(CCFLAGS) -c main.c
libqueue.so : queue.o 						; $(LD) -shared -o libqueue.so queue.o
queue.o		: queue.c queue.h 				; $(CC) $(CCFLAGS) -c queue.c -fPIC
clean		: 								; rm *.o ; rm libqueue.so ; rm main
###___###