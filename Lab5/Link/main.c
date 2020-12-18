#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>

#include <string.h>

#include "queue.h"

#define true 1

void help() {
    printf("push + number - to push\npop - to get elem\nprint - to print que\nexit - to exit prog\n");
    return;
}

int main(int argv, char ** argc) {
    char * com = (char *)malloc(sizeof(char) * 10);
    char * value = (char*)malloc(sizeof(char) * 255);
    Queue * que = createQ();
    int number, res; 
 //   printf("dont forget to 'help' first\n");
    while (true) {
        scanf("%s", com);
        //
        if (strcmp("push", com) == 0 || strcmp("pu", com) == 0) {
            scanf("%s", value);
            number = atoi(value);
            pushQ(que, number); //redo
        }  else if (strcmp("pop", com) == 0 || strcmp("po", com) == 0) {
            if (res = popQ(que)) {
                printf("res : %d\n", res);
            } else {
                printf("no number\n");
            }
        } else if (strcmp("print", com) == 0 || strcmp("p", com) == 0) {
            printf("que : \n");
            printQ(que);
        } else if (strcmp("exit", com) == 0 || strcmp("e", com) == 0) {
            break;
        } else if (strcmp("help", com) == 0 || strcmp("h", com) == 0) {
            help();
        }
    }
    printf("exit\n");
   // deleteQ(&que); 
    free(com);
    return 0;
}