#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

Queue * createQ() {
    Queue * que = (Queue *)malloc(sizeof(Queue));
    que->head = NULL;
    que->tail = NULL;
    que->size = 0;
    return que;
}

void pushQ(Queue * que, int value) {
    Node * tmp = (Node *)malloc(sizeof(Node));
  //  tmp->value = (int *)malloc(sizeof(int));
    if (que->size == 0) {
        que->head = tmp;
        que->tail = tmp;
        tmp->next = NULL;
        tmp->prev = NULL;
    } else {
        tmp->next = NULL;
        tmp->prev = que->tail;
        que->tail->next = tmp;
        que->tail = tmp;
    }
    que->size++;
    tmp->value = value;
}

int popQ(Queue * que) {
    int value;
    if (que->size == 0) {
        return 0;
    } else if (que->size == 1) {
        value = que->head->value;
    //    free(que->head->value);
        free(que->head);
        que->head = NULL;
        que->tail = NULL;
        que->size--;
    } else {
        value = que->head->value;
        que->head = que->head->next;
  //      free(que->head->prev->value);
        free(que->head->prev);
        que->head->prev = NULL;
        que->size--;
    }
    return value;
}

void printQ(Queue * que) {
    if (que->size != 0) {
        Node * tmp;
        tmp = que->head;
        for (int i = 0; i < que->size; i++) {
            printf("%d : %d\n", i, tmp->value);
            tmp = tmp->next;
        }
    }
}

void deleteQ(Queue ** que) {
    Node * tmp = (*que)->head;
    for (size_t i = 0; i < (*que)->size - 1; i++) {
  //      free(tmp->value);
        tmp = tmp->next;
        free(tmp->prev);
    }
    if ((*que)->size != 0) {
  //      free(tmp->value);
        free(tmp);
    }
    free(*que);
}