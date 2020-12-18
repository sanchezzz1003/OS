#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pthread.h>
#include <dlfcn.h>

#include <string.h>

#include "queue.h"

#define true 1

void help() {
    printf("push + number - to push\npop - to get elem\nprint - to print que\nexit - to exit prog\n");
    return;
}

Queue * create_Q()
{
    void *dl_handle;
    Queue *(*func)();
    char *error;
    dl_handle = dlopen("libqueue.so", RTLD_LAZY);
    if (!dl_handle) {
        printf("#%s\n", dlerror());
        return NULL;
    }
    func = (Queue* (*)())dlsym(dl_handle, "createQ");
    error = dlerror();
    if (error != NULL) {
        printf("#%s\n", error);
        return NULL;
    }
    Queue * out = func();
    dlclose(dl_handle);
    return out;
}

void push_Q(Queue * queue, int arg)
{
    void *dl_handle;
    void (*func)(Queue*, int);
    char *error;
    dl_handle = dlopen("libqueue.so", RTLD_LAZY);
    if (!dl_handle) {
        printf("# %s\n", dlerror());
        return;
    }
    func = (void (*)(Queue*, int))dlsym(dl_handle, "pushQ");
    error = dlerror();
    if (error != NULL) {
        printf("# %s\n", error);
        return;
    }
    func(queue, arg);
    dlclose(dl_handle);
    return;
}

int pop_Q(Queue * queue)
{
    void *dl_handle;
    int (*func)(Queue*);
    char *error;
    dl_handle = dlopen("libqueue.so", RTLD_LAZY);
    if (!dl_handle) {
        printf("# %s\n", dlerror());
        return 0;
    }
    func = (void (*)(Queue*))dlsym(dl_handle, "popQ");
    error = dlerror();
    if (error != NULL) {
        printf("# %s\n", error);
        return 0;
    }
    int res = func(queue);
    dlclose(dl_handle);
    return res;
}

void print_Q(Queue * queue)
{
    void *dl_handle;
    void (*func)(Queue*);
    char *error;
    dl_handle = dlopen("libqueue.so", RTLD_LAZY);
    if (!dl_handle) {
        printf("# %s\n", dlerror());
        return;
    }
    func = (void (*)(Queue*))dlsym(dl_handle, "printQ");
    error = dlerror();
    if (error != NULL) {
        printf("# %s\n", error);
        return;
    }
    func(queue);
    dlclose(dl_handle);
    return;
}

void delete_Q(Queue ** queue)
{
    void *dl_handle;
    void (*func)(Queue**);
    char *error;
    dl_handle = dlopen("libqueue.so", RTLD_LAZY);
    if (!dl_handle) {
        printf("# %s\n", dlerror());
        return;
    }
    func = (void (*)(Queue*))dlsym(dl_handle, "deleteQ");
    error = dlerror();
    if (error != NULL) {
        printf("# %s\n", error);
        return;
    }
    func(queue);
    dlclose(dl_handle);
    return;
}


int main(int argv, char ** argc) {
    char * com = (char *)malloc(sizeof(char) * 10);
    char * value = (char*)malloc(sizeof(char) * 255);
    Queue * que = create_Q();
    int number, res; 
 //   printf("dont forget to 'help' first\n");
    while (true) {
        scanf("%s", com);
        //
        if (strcmp("push", com) == 0 || strcmp("pu", com) == 0) {
            scanf("%s", value);
            number = atoi(value);
            push_Q(que, number); //redo
        }  else if (strcmp("pop", com) == 0 || strcmp("po", com) == 0) {
            if (res = pop_Q(que)) {
                printf("res : %d\n", res);
            } else {
                printf("no number\n");
            }
        } else if (strcmp("print", com) == 0 || strcmp("p", com) == 0) {
            printf("que : \n");
            print_Q(que);
        } else if (strcmp("exit", com) == 0 || strcmp("e", com) == 0) {
            break;
        } else if (strcmp("help", com) == 0 || strcmp("h", com) == 0) {
            help();
        }
    }
    printf("exit\n");
    deleteQ(&que); 
    free(com);
    return 0;
}