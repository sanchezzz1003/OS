#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char ** argv){
	char *name = argv[0];
	FILE * ptrFile = fopen(name, "r");
	if (ptrFile == NULL){
		fputs("ошибка файла\n", stderr);
		_exit(1);
	}
	char n;
	while(!feof(ptrFile))
		{
		int res = fscanf(ptrFile,"%c",&n);
		if (res == EOF){
			printf("\n");
		    break;
		}
	printf("%c",n);
	}
	fclose(ptrFile);
	return 0;
}