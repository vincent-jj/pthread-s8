#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define MAX_LEN 100

int main(void){
  // for(int i = 0; i<argc; i++) printf("%d : %s\n",i, argv[i]);  
  FILE *file;
  file = fopen("fibo.dat","a+");
  if(file == NULL)  
    exit(-1);
  fprintf(file,"i time\n");
  struct timeval t1,t2;
  char temp[2];
  char c1[MAX_LEN];
  strcpy(c1,"./build/test51");
  char c2[MAX_LEN];
  strcpy(c2, "/build/pthtest51");
  strcat(c1, " ");
  strcat(c2, " ");
  for(int i = 1; i < 30; i++){
    sprintf(temp,"%d",i);
    c1[15] = temp[0];
    if(i>10) c1[16] = temp[1];
    gettimeofday(&t1, NULL);
    system(c1);
    gettimeofday(&t2, NULL);
    unsigned long microseconds = (t2.tv_sec-t1.tv_sec)*1000000 + (t2.tv_usec-t1.tv_usec);
    fprintf(file,"%d %u\n",i, microseconds);
    printf("\n\n ***time val %u ***\n\n\n",microseconds);
}
  //for(int i = 0; i<strlen(c); i++)
  //  printf("%d : %c\n",i, c[i]); 
  return EXIT_SUCCESS;
}
