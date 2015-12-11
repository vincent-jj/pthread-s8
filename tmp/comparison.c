#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define MAX_LEN 100

int main(int argc, char *argv[]){
  // for(int i = 0; i<argc; i++) printf("%d : %s\n",i, argv[i]);  
  struct timeval t1,t2;
  if(argc > 4){
    perror("Too much arguments\n");
    return EXIT_FAILURE;
  }
  char c[MAX_LEN];
  strcpy(c,"./build/");
  for(int i = 1; i < argc; i++){
    strcat(c,argv[i]);
    strcat(c, " ");
  }
	printf("%s %d", argv[2], argc);
  gettimeofday(&t1, NULL);
  system(c);
  gettimeofday(&t2, NULL);
  unsigned long microseconds = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
  FILE * file = NULL;
	file = fopen("data", "a+");
	if(file == NULL){
		perror("Impossible d'ouvrir le fichier correspondant\n");
		exit(-1);
	}
	fprintf(file, "%s %u\n",argv[2], microseconds);
	printf("time val %u\n",microseconds);
  return EXIT_SUCCESS;
}
