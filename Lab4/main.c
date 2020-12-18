#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "stringW.h"

#define SIZE 300

void printFile(char *name) {
	FILE * ptrFile = fopen(name, "r");
	if (ptrFile == NULL){
		fputs("Ошибка файла\n", stdout);
		_exit(0);
	}
	char n;
	while(!feof(ptrFile)) {
	int res = fscanf(ptrFile,"%c",&n);
	if (res == EOF){
		printf("\n");
	    break;
	}
	printf("%c",n);
	}
	fclose(ptrFile);
}

int main(int argc, char ** argv) {
	//printf("s - start sort\nf - finish sort\nc -choose files\ne - exit\n");
	char *name1, *name2; 
	int fd1[2], fd2[2];
    pid_t pid2, pid;

    name1 = (char*)malloc(SIZE);
    name2 = (char*)malloc(SIZE);

    printf("enter name 1: ");
	scanf("%s", name1);
	printf("enter name 2: ");
	scanf("%s", name2);
//	printf("%s\n", name1);
	if (pipe(fd1) == -1){
		perror ("pipe1");
		exit(-1);
	}
	if (pipe(fd2) == -1){
		perror ("pipe2");
		exit(-1);
	}
    switch(pid = fork()) {	
        case -1:
            perror("fork1");
            return -1;
        case 0:
            // Child1
        	execl("sort", intToChar(fd1[0]), intToChar(fd1[1]), NULL);
            break;
        default:
            // Parent
        	close(fd1[0]);
			write(fd1[1], name1, strlen(name1));
			close(fd1[1]);
            switch(pid2 = fork()) {
				case -1:
					perror("fork2");
				return -1;
				case 0:
					// Child2
						execl("sort", intToChar(fd2[0]), intToChar(fd2[1]), NULL);
						break;
				default:
			        // Parent
	    			close(fd2[0]);
	    			write(fd2[1], name2, strlen(name2));
	    			close(fd2[1]);
	    			waitpid(pid, NULL, 0);
	    			waitpid(pid2, NULL, 0);
	    			puts("File1:");
					printFile(name1);
	    			puts("File2:");
	    			printFile(name2);
	    			break;
			}
            break;
	}
    return 0;
}