#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

//structure
typedef struct Node {
    int value;
    struct Node * next;
    struct Node * prev;
} Node;

typedef struct Queue {
  Node * head;
  Node * tail;
  int size;
} Queue;

//functions
Queue * createQ();
void pushQ(Queue * que, int value);
int popQ(Queue * que);
void printQ(Queue * que);
void deleteQ(Queue ** que);


#endif