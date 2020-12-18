#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

#include "stringW.h"
#include "qSort.h"

#define SIZE 300

int main(int argc, char ** argv){
	if (argc != 2){
		perror("no descriptor");
		exit(-1);
	}
	char *input;
//	printf("%s\n", argv[0]);
	int fd[2], i = 0;
	fd[0] = charToInt(argv[0]);
	fd[1] = charToInt(argv[1]);
	input = (char*)malloc(SIZE);
	close(fd[1]);
	while ((read(fd[0], &input[i++], 1)) > 0){}
	close(fd[0]);
//	printf("here\n");
//	printf("file is %s\n", input);
	FILE *ptrFile = fopen(input, "r");
	if (ptrFile == NULL){
		fputs("ошибка файла\n", stderr);
		exit(1);
	}

	fseek(ptrFile, 0, SEEK_END);
	long lSize = ftell(ptrFile);
	rewind(ptrFile);

	char * buffer = (char*)malloc(sizeof(char) * lSize);
	if (buffer == NULL){
		fputs("Ошибка памяти", stderr);
		exit(2);
	}

	size_t result = fread(buffer, 1, lSize, ptrFile);
	if (result != lSize){
		fputs("Ошибка чтения", stderr);
		exit(3);
	}
	qsortRecursive(buffer, result);
	fclose(ptrFile);
	FILE *output = fopen(input, "w");
	fwrite(buffer, 1,result, output);
	fclose(output);
	free(buffer);
	return 0;
}