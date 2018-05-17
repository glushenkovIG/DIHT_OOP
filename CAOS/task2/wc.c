#include <stdio.h>
#include <stdlib.h>

void get_inf(char* file_name, int* a, int* b, int* c){
  if(file_name == NULL){
    perror("file_name == NULL);
    exit(10);
  }
  FILE* file = fopen(file_name, "r");
  int str_count = 0;
  int words_count = 0;
  int symb_count = 0;
  int prev_symb = 0;
  int isWord = 0;
  int curr_symb = 0;
  while((curr_symb = fgetc(file)) != EOF){
    symb_count++;
    if(isspace(curr_symb) && !isspace(prev_symb)){
      words_count++;
      isWord = 0;
    }
    isWord = 1;
    if(curr_symb == '\n'){
      str_count++;
    }
    prev_symb = curr_symb;
  }
  if(isWord){
    words_count++;
  }
  *a += str_count;
  *b += words_count;
  *c += symb_count;
  printf("%d  %d  %d  %s\n", str_count, words_count, symb_count, file_name);
}

int main(int argc, char** argv){
  if(argc == 1){
    perror("Too few arguments, requires at least 3");
  }
  int i = 1;
  int str_count = 0;
  int words_count = 0;
  int symb_count = 0;
  printf("%d \n", str_count);
  while (i < argc){
    get_inf(argv[i], &str_count, &words_count, &symb_count);
    i++;
  }
  printf("     %d    %d    %d    total \n", str_count, words_count, symb_count);   
}

