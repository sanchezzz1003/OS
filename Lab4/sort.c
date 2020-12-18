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
#include "qSort.h"

#define SIZE 300

typedef struct FileMapping {
  int fdin;
  size_t fsize;
  unsigned char* dataPtr;
} FileMapping;

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
	
//	printf("file is %s\n", input);

	int fdin = open(input, O_RDONLY, 0);
	if(fdin < 0) {
  		printf("fileMappingCreate - open failed");
  		return NULL;
	}

	struct stat st;
	if(fstat(fdin, &st) < 0) {
	  	printf("fileMappingCreate - fstat failed");
	  	close(fdin);
	  	return NULL;
	}

	size_t fsize = (size_t)st.st_size;

	unsigned char* dataPtr = (unsigned char*)mmap(NULL, fsize,
	                                              PROT_READ,
	                                              MAP_PRIVATE,
	                                              fdin, 0);
	if(dataPtr == MAP_FAILED) {
		printf("fileMappingCreate - mmap failed");
		close(fdin);
	  	return NULL;
	}

	FileMapping * mapping = (FileMapping *)malloc(sizeof(FileMapping));
	if(mapping == NULL) {
	  	printf("fileMappingCreate - malloc failed");
	 //           << fname << std::endl;
	  	munmap(dataPtr, fsize);
	  	close(fdin);
	  	return NULL;
	}

	mapping->fdin = fdin;
	mapping->fsize = fsize;
	mapping->dataPtr = dataPtr;

/*	fseek(ptrFile, 0, SEEK_END);
	long lSize = ftell(ptrFile);
	rewind(ptrFile);
*/
	char * buffer = (char*)malloc(sizeof(char) * fsize);
	if (buffer == NULL){
		fputs("Ошибка памяти", stderr);
		exit(2);
	}


	size_t result = read(fdin, buffer, fsize);
	if (result != fsize){
		fputs("Ошибка чтения", stderr);
		exit(3);
	}

	//printf("here\n");
	//printf("%d\n", result);
	//for (int i = 0; i < 10; i++){
	//	printf("%d\n", buffer[i]);
	//}

	//printf("here\n");
	qsortRecursive(buffer, result);

	int fdout = open(input, O_RDWR | O_CREAT | O_TRUNC);

	write(fdout,buffer, result);
	free(buffer);
	return 0;
}